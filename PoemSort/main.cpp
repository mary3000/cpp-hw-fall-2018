/**
 * MIPT | DIHT | 3 course | Industrial programming <br>
 * Purpose: sort poem lexicographically
 *
 * @file
 * @author Mary Feofanova
 */

#include <fstream>

/**
 * List of some parts of directories to construct new files
 */
namespace DirPrefixes {
const std::string DIR_PREFIX = "../PoemSort/";
const std::string OUT_DIR = DIR_PREFIX + "out/";
const std::string SORT_PREFIX = "sorted_";
const std::string REVERSE_SORT_PREFIX = "reverse_sorted_";
}

/// Special character for recognizing string end
const char END = '\0';
/// Offset for upper chars
const char UPPER_OFFSET = 'A' - 'a';

/**
 * RAII class for safety resource management of arrays
 *
 * @tparam T type of array elements
 */
template<class T>
class Array {
 public:
  explicit Array(size_t size_) : size(size_) {
    data = new T[size_];
  }

  ~Array() {
    delete[] data;
  }

  T &operator[](size_t i) const {
    return data[i];
  };

 public:
  T *data;
  size_t size;
};

/**
 * Writes to the 'out/' dir three versions of file:
 * <ul>
 * <li> Same version with significant strings;
 * <li> With strings sorted lexicographically;
 * <li> With strings sorted lexicographically from end to beginning;
 * </ul>
 *
 * @param [in] buffer Char array with data
 * @param [in] file_name Suffix of output files
 *
 * @note the file must be in the same directory as this source file
 */
void SortPoemStrings(Array<char> &buffer, const char *file_name);

int main() {
  const std::string file_name = "RomeoAndJuliet.txt";
  std::string path = DirPrefixes::DIR_PREFIX + file_name;
  std::ifstream is(path, std::ios::binary | std::ios::ate);
  std::streamsize size = is.tellg();
  is.seekg(0, std::ios::beg);

  Array<char> buffer(size + 1);
  if (is.read(buffer.data, size)) {
    is.close();
    buffer.data[size] = '\n';
    SortPoemStrings(buffer, file_name.data());
  } else {
    throw "Unable to read the file. Check whether it exists and program has permission for reading.";
  }
}

/**
 * Initializes array with pointers to strings beginnings of the text
 *
 * @param [out] ptr_arr Array which will be initialized
 * @param [in] text Char array of strings
 */
void InitializeWithStringPointers(Array<char *> &ptr_arr, const Array<char> &text) {
  size_t num = 1;
  ptr_arr[0] = text.data;
  for (size_t i = 1; i < text.size; i++) {
    if (text[i - 1] == END) {
      ptr_arr[num++] = text.data + i;
    }
  }
}

bool IsUpperLetter(char ch) {
  return ch >= 'A' && ch <= 'Z';
}

bool IsSmallLetter(char ch) {
  return ch >= 'a' && ch <= 'z';
}

/**
 * Checks whether the char is insignificant
 */
bool IsToSkip(char ch) {
  return !IsSmallLetter(ch) && !IsUpperLetter(ch) && ch != ' ';
}

/**
 * Checks whether string should be shown or not
 */
bool IsSignificant(const char* string) {
  bool hasCaps = false;
  bool isOneWord = true;
  while (*string != END) {
    if (IsUpperLetter(*string) && IsUpperLetter(*(string + 1))) {
      hasCaps = true;
    }
    if (!IsToSkip(*string) && *(string + 1) == ' ' && !IsToSkip(*(string + 1))) {
      isOneWord = false;
    }
    ++string;
  }
  return !hasCaps && !isOneWord;
}

/**
 * Writes strings of text in order corresponding to the ptr_arr to the file
 *
 * @param [in] ptr_arr Array of pointers of strings - in that order they will be written
 * @param [in] file_name Name of the output file
 */
void WriteStringsToFile(const Array<char *> &ptr_arr, const char *file_name) {
  std::ofstream os(DirPrefixes::OUT_DIR + file_name, std::ofstream::out);
  for (size_t i = 0; i < ptr_arr.size; i++) {
    // Skip heroes names and etc.
    if (!IsSignificant(ptr_arr[i])) {
      continue;
    }
    os << ptr_arr[i] << std::endl;
  }
  os.close();
}

/**
 * Compares two pointers to strings via their values lexicographically
 *
 * @note it skips all but alphabet chars, and ignores uppercase
 */
int StringPointerComparator(const void *first_ptr, const void *second_ptr) {
  char *first = *(char **) first_ptr;
  char *second = *(char **) second_ptr;
  char ch1 = *first;
  char ch2 = *second;
  while (ch1 != END && ch2 != END) {
    if (IsUpperLetter(ch1)) {
      ch1 = ch1 - UPPER_OFFSET;
    }
    if (IsUpperLetter(ch2)) {
      ch2 = ch2 - UPPER_OFFSET;
    }
    if (IsToSkip(ch1)) {
      ch1 = *(++first);
      continue;
    }
    if (IsToSkip(ch2)) {
      ch2 = *(++second);
      continue;
    }
    if (ch1 != ch2) {
      return ch1 > ch2;
    }
    ch1 = *(++first);
    ch2 = *(++second);
  }
  return ch1 != END;
}

/**
 * Compares two pointers to strings via their values from the end lexicographically
 *
 * @note it skips all but alphabet chars, and ignores uppercase
 */
int ReversedStringPointerComparator(const void *first_ptr, const void *second_ptr) {
  char *first = *(char **) first_ptr;
  char *second = *(char **) second_ptr;
  char ch1 = *(first);
  char ch2 = *(second);
  int first_len, second_len;
  first_len = second_len = 0;
  while (ch1 != END) {
    first_len++;
    ch1 = *(first + first_len);
  }
  while (ch2 != END) {
    second_len++;
    ch2 = *(second + second_len);
  }
  if (first_len == 0 || second_len == 0) {
    return false;
  }
  while (first_len >= 0 && second_len >= 0) {
    ch1 = *(first + first_len--);
    ch2 = *(second + second_len--);
    if (IsUpperLetter(ch1)) {
      ch1 = ch1 - UPPER_OFFSET;
    }
    if (IsUpperLetter(ch2)) {
      ch2 = ch2 - UPPER_OFFSET;
    }
    if (IsToSkip(ch1)) {
      second_len++;
      continue;
    }
    if (IsToSkip(ch2)) {
      first_len++;
      continue;
    }
    if (ch1 != ch2) {
      return ch1 > ch2;
    }
  }
  return first_len > second_len;
}

/**
 * Sorts strings with comparator and writes the result
 *
 * @param [in] ptr_arr Array of pointers that will be sorted
 * @param [in] file_name Name of the resulting file
 * @param [in] comp Comparator
 */
void SortAndWrite(const Array<char *> &ptr_arr,
                  const char *file_name,
                  int (*comp)(const void *, const void *)) {
  if (comp != nullptr) {
    std::qsort(ptr_arr.data, ptr_arr.size, sizeof(char *), comp);
  }
  WriteStringsToFile(ptr_arr, file_name);
}

/**
 * Replaces `\n` to END char, i.e., splits the text into c-style strings
 *
 * @return number of strings
 */
size_t SplitToStrings(Array<char> &text) {
  size_t ptr_num = 0;
  for (size_t i = 0; i < text.size; i++) {
    if (text[i] == '\n') {
      text[i] = END;
      ++ptr_num;
    }
  }
  return ptr_num;
}

void SortPoemStrings(Array<char> &buffer, const char *file_name) {
    Array<char *> ptr_arr(SplitToStrings(buffer));
    InitializeWithStringPointers(ptr_arr, buffer);
    SortAndWrite(ptr_arr, file_name, nullptr);
    SortAndWrite(ptr_arr, (DirPrefixes::SORT_PREFIX + file_name).data(),
                 StringPointerComparator);
    SortAndWrite(ptr_arr, (DirPrefixes::REVERSE_SORT_PREFIX + file_name).data(),
                 ReversedStringPointerComparator);
}