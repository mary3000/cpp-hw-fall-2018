/**
 * MIPT | DIHT | 3 course | Industrial programming <br>
 * Purpose: test the Polynomial::Solve() method
 *
 * @file
 * @author Mary Feofanova
 */

#include "gtest/gtest.h"
#include "../src/Polynomial.h"

#include <vector>
#include <cmath>

/**
 * Fixture for testing Polynomial class
 */
class PolynomialTest : public ::testing::Test {
 protected:
  /**
   * Checks whether the solver answer is equal to the given answer
   *
   * @param [in] params Coefficients of polynomial
   * @param [in] answer Expected answer
   * @param [in] description Expected number
   */
  void AssertEqualSolve(const std::vector<double> &params,
                        const std::vector<double> &answer,
                        RootDescription description) {
    Roots expected_roots(answer.size(), description);
    memcpy(expected_roots.roots_, answer.data(), answer.size() * sizeof(answer[0]));
    Polynomial polynomial(params);
    Roots actual_roots = Polynomial(params).Solve();
    ASSERT_EQ(expected_roots, actual_roots);
  }

  /**
   * Checks whether the solver answer is near by the given answer
   *
   * @param [in] params Coefficients of polynomial
   * @param [in] answer Expected answer
   * @param [in] number Expected number
   * @param [in] error Accepted error
   */
  void AssertNearSolve(const std::vector<double> &params,
                       const std::vector<double> &answer,
                       RootDescription description,
                       double error) {
    Roots expected_roots(answer.size(), description);
    memcpy(expected_roots.roots_, answer.data(), answer.size() * sizeof(answer[0]));
    Polynomial polynomial(params);
    Roots actual_roots = Polynomial(params).Solve();
    ASSERT_EQ(actual_roots.number_, expected_roots.number_);
    ASSERT_EQ(actual_roots.description_, expected_roots.description_);
    for (size_t i = 0; i < actual_roots.number_; i++) {
      ASSERT_NEAR(actual_roots.roots_[i], expected_roots.roots_[i], error);
    }
  }

  /**
   * Checks that Polynomial throws an exception in case of invalid arguments
   *
   * @param [in] params Coefficients of polynomial
   */
  void AssertThrowIsFinite(const std::vector<double> &params) {
    ASSERT_THROW(Polynomial polynomial(params), std::invalid_argument);
  }
};

/////////////// Simple ///////////////

TEST_F(PolynomialTest, Simple1) {
  AssertEqualSolve({0}, {}, INFINITE);
}

TEST_F(PolynomialTest, Simple2) {
  AssertEqualSolve({1}, {}, FINITE);
}

TEST_F(PolynomialTest, Simple3) {
  AssertEqualSolve({0, 0, 0, 0, 0, 5}, {}, FINITE);
}

/////////////// Linear ///////////////

TEST_F(PolynomialTest, Linear1) {
  AssertEqualSolve({1, -2}, {2}, FINITE);
}

TEST_F(PolynomialTest, Linear2) {
  AssertEqualSolve({2, 3}, {-1.5}, FINITE);
}

TEST_F(PolynomialTest, Linear3) {
  AssertEqualSolve({0, 0, 0, 0, 3, 3}, {-1}, FINITE);
}

TEST_F(PolynomialTest, Linear4) {
  AssertEqualSolve({0, 10, 0}, {0}, FINITE);
}

/////////////// Quadratic ///////////////

TEST_F(PolynomialTest, Quadratic1) {
  AssertEqualSolve({1, 2, 1}, {-1}, FINITE);
}

TEST_F(PolynomialTest, Quadratic2) {
  AssertEqualSolve({3, -12, 0}, {0, 4}, FINITE);
}

TEST_F(PolynomialTest, Quadratic3) {
  AssertEqualSolve({2, 5, -3}, {-3, 0.5}, FINITE);
}

TEST_F(PolynomialTest, Quadratic4) {
  AssertEqualSolve({4, -4, 13}, {}, FINITE);
}

TEST_F(PolynomialTest, Quadratic5) {
  AssertNearSolve({2, -3, -1}, {-0.2808, 1.7808}, FINITE, 0.0001);
}

/////////////// Can't solve ///////////////

TEST_F(PolynomialTest, Unknown1) {
  AssertEqualSolve({1, 1, 1, 1}, {}, UNKNOWN);
}

TEST_F(PolynomialTest, Unknown2) {
  AssertEqualSolve({1, 0, 0, 0, 0}, {}, UNKNOWN);
}

TEST_F(PolynomialTest, Unknown3) {
  AssertEqualSolve({0, 0, 5, 0, 0, 0}, {}, UNKNOWN);
}

/////////////// Invalid arguments ///////////////

TEST_F(PolynomialTest, Invalid1) {
  AssertThrowIsFinite({std::numeric_limits<double>::infinity(), 0});
}

TEST_F(PolynomialTest, Invalid2) {
  AssertThrowIsFinite({NAN, 0, 0, 0, 0});
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}