
#ifndef PROTECTEDSTACK_STACK_H
#define PROTECTEDSTACK_STACK_H

#include <cstddef>
#include <cstdlib>
#include <utility>
#include <sstream>
#include <iostream>
#include <fstream>
#include <assert.h>

#define ASSERT_OK \
    { \
        StackError error = IsOk(this); \
        if (error != kNone) { \
            Dump(this, error); \
            if (this && data_ != nullptr) { \
                this->~Stack(); \
            } \
            exit(1); \
        } \
    } \

template <typename T>
class Stack {
public:
    Stack();
    ~Stack();
    Stack(const Stack& other) = delete;
    Stack(Stack&& other) = delete;
    Stack& operator=(const Stack& other) = delete;
    Stack& operator=(Stack&& other) = delete;

    void Push(T element);
    bool Pop();
    bool Pop(T& element);
    bool Top(T& element);
    bool IsEmpty();

private:
    using Canary = uint64_t;

    static const int kInitialDataSize = 4;
    static const int kGrowthFactor = 2;
    static const Canary kCanaryValue = 0xBADC0FFEE0DDF00D;
    static const uint32_t kPoisonValue = 0xDEADBEEF;

    enum StackError {
        kNone, kNullPtr, kWrongCheckSum, kWrongDataCheckSum, kOverFlow
    };

    static StackError IsOk(Stack* stack) {
        if (stack == nullptr) {
            return kNullPtr;
        }
        if (!stack->CheckSumOk()) {
            return kWrongCheckSum;
        }
        if (!stack->DataCheckSumOk()) {
            return kWrongDataCheckSum;
        }
        if (stack->size_ < stack->offset_) {
            return kOverFlow;
        }
        return kNone;
    }
    static void Dump(Stack *stack, StackError e);
    // Implementation of Adler-32 - checksum algorithm
    template<typename S>
    static void Adler32(S value, size_t len, uint32_t &a, uint32_t &b);
    static std::string getTextRepresentation(const Stack *stack, const Stack<T>::StackError &e, bool full);

    uint32_t ComputeCheckSum();
    uint32_t ComputeDataCheckSum();
    bool CheckSumOk();
    bool DataCheckSumOk();
    void UpdateAllCheckSum();
    void EnsureHasPlace();
    void Reallocate(size_t new_size);
    void PoisonData();

    Canary header_canary_;

    Canary* data_header_canary_;
    T* data_;
    Canary* data_footer_canary_;

    size_t offset_;
    size_t size_;

    uint32_t check_sum_;
    uint32_t data_check_sum_;

    Canary footer_canary_;
};

template<typename T>
void Stack<T>::Reallocate(size_t new_size) {
    size_ = new_size;
    void* ptr_ = nullptr;
    ptr_ = realloc(data_header_canary_, sizeof(T) * new_size + 2 * sizeof(Canary));
    data_header_canary_ = reinterpret_cast<Canary*>(ptr_);
    *data_header_canary_ = kCanaryValue;
    data_ = reinterpret_cast<T*>(data_header_canary_ + 1);
    PoisonData();
    data_footer_canary_ = reinterpret_cast<Canary*>(data_ + new_size);
    *data_footer_canary_ = kCanaryValue;
}

template<typename T>
uint32_t Stack<T>::ComputeDataCheckSum() {
    uint32_t a = 1, b = 0;
    Stack::Adler32(data_header_canary_, sizeof(Canary), a, b);
    Stack::Adler32(data_, sizeof(T) * size_, a, b);
    Stack::Adler32(data_footer_canary_, sizeof(Canary), a, b);
    return (b << 16) | a;
}

template<typename T>
bool Stack<T>::CheckSumOk() {
    return check_sum_ == ComputeCheckSum();
}

template<typename T>
void Stack<T>::EnsureHasPlace() {
    if (offset_ < size_) {
        return;
    }
    Reallocate(size_ * kGrowthFactor);
}

template<typename T>
Stack<T>::Stack() : offset_(0) {
    data_header_canary_ = nullptr;
    Reallocate(kInitialDataSize);
    header_canary_ = kCanaryValue;
    footer_canary_ = kCanaryValue;
    UpdateAllCheckSum();
}

template<typename T>
void Stack<T>::Push(T element) {
    ASSERT_OK
    EnsureHasPlace();
    data_[offset_++] = std::move(element);
    UpdateAllCheckSum();
    ASSERT_OK
}

template<typename T>
bool Stack<T>::Pop(T &element) {
    ASSERT_OK
    if (IsEmpty()) {
        return false;
    }
    element = std::move(data_[--offset_]);
    data_[offset_] = kPoisonValue;
    UpdateAllCheckSum();
    ASSERT_OK
    return true;
}

template<typename T>
bool Stack<T>::IsEmpty() {
    ASSERT_OK
    return offset_ == 0;
}

template<typename T>
void Stack<T>::Dump(Stack *stack, StackError e) {
    std::string to_stderr = getTextRepresentation(stack, e, false);
    fprintf(stderr, to_stderr.data());
    std::ofstream out_file;
    out_file.open("..ProtectedStack/stack_error.txt");
    std::string to_file = getTextRepresentation(stack, e, true);
    out_file << to_file;
    out_file.close();
}

template<typename T>
std::string Stack<T>::getTextRepresentation(const Stack *stack, const Stack<T>::StackError &e, bool full) {
    std::stringstream stream;
    stream << "Ouch! Your beautiful shiny stack is damaged!\n";
    stream << "Reason: ";
    switch(e) {
        case kNullPtr:
            stream << "stack pointer is null.\n";
            break;
        case kWrongCheckSum:
            stream << "total checksum of stack has unexpectedly changed.\n";
            break;
        case kWrongDataCheckSum:
            stream << "checksum of stack data has unexpectedly changed.\n";
            break;
        case kOverFlow:
            stream << "stack is overflowed.\n";
        default: break;
    }
    //First level: begin
    stream << "Stack<" << typeid(T).name() << "> [" << stack << "] {\n";
    if (e != kNullPtr) {
        stream << "\theader canary: " << std::hex << stack->header_canary_ << std::dec;
        if (stack->header_canary_ != kCanaryValue) {
            stream << " (FAILED!)";
        }
        stream << ";\n";
        //Second level: begin
        stream << "\tinternal buffer {\n";
        stream << "\t\theader canary: " << std::hex << *stack->data_header_canary_ << std::dec;
        if (*stack->data_header_canary_ != kCanaryValue) {
            stream << " (FAILED!)";
        }
        stream << ";\n";
        stream << "\t\tdata [" << stack->offset_ << "/" << stack->size_ << "] [" << std::hex << reinterpret_cast<void*>(stack->data_) << std::dec << "] {" << "\n";
        size_t bound = stack->size_;
        if (!full && bound > 32) {
            bound = 10;
        }
        for (size_t i = 0; i < bound; i++) {
            stream << "\t\t\t[" << i << "]: ";
            if (i >= stack->offset_) {
                stream << std::hex << stack->data_[i] << std::dec << " (reserved & poisoned)";
            } else {
                stream << stack->data_[i];
            }
            stream << ";\n";
        }
        if (bound != stack->size_) {
            stream << "\t\t\t...\n";
            for (size_t i = stack->size_ - bound; i < stack->size_; i++) {
                stream << "\t\t\t[" << i << "]: ";
                if (i >= stack->offset_) {
                    stream << std::hex << stack->data_[i] << std::dec << " (reserved & poisoned)";
                } else {
                    stream << stack->data_[i];
                }
                stream << ";\n";
            }
        }
        stream << "\t\t}\n";
        stream << "\t\tfooter canary: " << std::hex << *stack->data_footer_canary_ << std::dec;
        if (*stack->data_footer_canary_ != kCanaryValue) {
            stream << " (FAILED!)";
        }
        stream << ";\n";
        stream << "\t}\n";
        //Second level: end
        stream << "\toffset: " << stack->offset_;
        if (e == kOverFlow) {
            stream << " (FAILED!)";
        }
        stream << ";\n";
        stream << "\tsize: " << stack->size_ << ";\n";
        stream << "\tchecksum: " << stack->check_sum_;
        if (e == kWrongCheckSum) {
            stream << " (FAILED!)";
        }
        stream << ";\n";
        stream << "\tdata checksum: " << stack->data_check_sum_;
        if (e == kWrongDataCheckSum) {
            stream << " (FAILED!)";
        }
        stream << ";\n";
        stream << "\tfooter canary: " << std::hex << stack->footer_canary_ << std::dec;
        if (stack->footer_canary_ != kCanaryValue) {
            stream << " (FAILED!)";
        }
        stream << ";\n";
    } else {
        stream << "\tNULL\n";
    }
    stream << "}\n";
    //First level: end
    return stream.str();
}

template<typename T>
template<typename S>
void Stack<T>::Adler32(S value, size_t len, uint32_t &a, uint32_t &b) {
    auto * data = reinterpret_cast<uint8_t*>(value);
    const uint32_t mod_adler = 65521;

    for (size_t index = 0; index < len; ++index) {
        a = (a + data[index]) % mod_adler;
        b = (b + a) % mod_adler;
    }
}

template<typename T>
uint32_t Stack<T>::ComputeCheckSum() {
    uint32_t a = 1, b = 0;
    Adler32(&header_canary_, sizeof(Canary), a, b);
    Adler32(&data_header_canary_, sizeof(data_header_canary_), a, b);
    Adler32(&data_, sizeof(data_), a, b);
    Adler32(&data_footer_canary_, sizeof(data_footer_canary_), a, b);
    Stack::Adler32(&offset_, sizeof(offset_), a, b);
    Stack::Adler32(&size_, sizeof(size_), a, b);
    Adler32(&footer_canary_, sizeof(Canary), a, b);
    return (b << 16) | a;
}

template<typename T>
bool Stack<T>::DataCheckSumOk() {
    return data_check_sum_ == ComputeDataCheckSum();
}

template<typename T>
void Stack<T>::UpdateAllCheckSum() {
    data_check_sum_ = ComputeDataCheckSum();
    check_sum_ = ComputeCheckSum();
}

template<typename T>
Stack<T>::~Stack() {
    free(data_header_canary_);
}

template<typename T>
void Stack<T>::PoisonData() {
    auto * data = reinterpret_cast<uint32_t*>(data_);
    for (size_t i = offset_ * sizeof(T) / sizeof(uint32_t); i < size_ * sizeof(T) / sizeof(uint32_t); i++) {
        data[i] = kPoisonValue;
    }
}

template<typename T>
bool Stack<T>::Top(T &element) {
    ASSERT_OK
    if (offset_ != 0) {
        element = data_[offset_ - 1];
        return true;
    }
    return false;
}

template<typename T>
bool Stack<T>::Pop() {
    ASSERT_OK
    T tmp;
    return Pop(tmp);
}

#endif //PROTECTEDSTACK_STACK_H
