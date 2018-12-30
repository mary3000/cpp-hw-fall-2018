#include "gtest/gtest.h"
#define private public
#include "../src/stack.h"

#include <vector>
#include <cmath>

/**
 * Fixture for testing Polynomial class
 */
class ProtectedStackTest : public ::testing::Test {
protected:
    template <typename T>
    void AssertErrorByFile(Stack<T> *stack, const std::string &error) {
      std::ifstream error_file;
      error_file.open("../ProtectedStack/test/stack_error_" + error + ".txt");
      std::stringstream buffer;
      buffer << error_file.rdbuf();
      error_file.close();
      ASSERT_EXIT(stack->IsEmpty(), ::testing::ExitedWithCode(1), "(" + buffer.str() + ".*?)");
    }
};

TEST_F(ProtectedStackTest, Normal1) {
  Stack<int> stack;
  const int c = 1000;
  for (int i = 0; i < c; i++) {
    stack.Push(i);
  }
  int a = 0;
  for (int i = 0; i < c; i++) {
    bool popped = stack.Pop(a);
    ASSERT_TRUE(popped);
    ASSERT_EQ(a, c - i - 1);
  }
  int b = 1000;
  for (int i = 0; i < b; i++) {
    stack.Push(i);
  }
  for (int i = 0; i < b; i++) {
    bool popped = stack.Pop(a);
    ASSERT_TRUE(popped);
    ASSERT_EQ(a, b - i - 1);
  }
  ASSERT_FALSE(stack.Pop(a));
  ASSERT_FALSE(stack.Top(a));
}

TEST_F(ProtectedStackTest, Outside) {
  Stack<int> stack;
  const int c = 100;
  for (int i = 0; i < c; i++) {
    stack.Push(i);
  }

  // Attack from outside
  *((int*)(&stack)) = 0;

  AssertErrorByFile(&stack, "checksum");
}

TEST_F(ProtectedStackTest, Inside) {
  Stack<int> stack;
  const int c = 5;
  for (int i = 0; i < c; i++) {
    stack.Push(i);
  }

  // Simulate overflowing
  stack.offset_ += 10;
  stack.UpdateAllCheckSum();

  AssertErrorByFile(&stack, "overflow");
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}