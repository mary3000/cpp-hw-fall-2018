#include "gtest/gtest.h"
#include "../src/Processor.h"

#include <vector>
#include <cmath>
#include <fstream>

/**
 * Fixture for testing Polynomial class
 */
class ProcessorTest : public ::testing::Test {
protected:
    void AssertErrorByFile(const std::string &file_name, const std::string& expected) {
      std::ifstream file;
      file.open("../Processor/data/" + file_name + ".txt");
      Processor p(file, 100);
      file.close();
      std::stringstream stream;
      p.Run(nullptr, stream);
      ASSERT_EQ(expected + "\n", stream.str());
    }
};

TEST_F(ProcessorTest, Sum) {
  AssertErrorByFile("sum", "3");
}

TEST_F(ProcessorTest, Mul) {
  AssertErrorByFile("mul", "30");
}

TEST_F(ProcessorTest, Euqlid) {
    AssertErrorByFile("euclid", "4");
}

TEST_F(ProcessorTest, Mov) {
    AssertErrorByFile("mov", "12");
}

TEST_F(ProcessorTest, Pop) {
    AssertErrorByFile("pop", "12");
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}