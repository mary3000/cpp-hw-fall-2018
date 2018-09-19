/**
 * MIPT | DIHT | 3 course | Industrial programming <br>
 * Purpose: solve polynomial equation up to 2 degree
 *
 * @file
 * @author Mary Feofanova
 */

#include <utility>

#include <tuple>
#include <vector>
#include <assert.h>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include "Polynomial.h"

const double EPSILON = 0.0000000000001;

/**
 * Solves the linear equation \f$ ax + b = 0 \f$
 *
 * @param [in] a
 * @param [in] b
 *
 * @note \p a must not be zero
 */
Roots LinearSolver(double a, double b) {
  assert(a != 0);
  Roots roots(1, FINITE);
  *roots.roots_ = -b / a;
  return roots;
}

/**
 * Calculates determinant for the quadratic equation \f$ ax^2 + bx + c = 0 \f$
 *
 * @param [in] a
 * @param [in] b
 * @param [in] c
 * @return determinant value
 *
 * @note \p a must not be zero
 */
double Determinant(double a, double b, double c) {
  assert (a != 0);
  return b * b - 4 * a * c;
}

/**
 * Compares two double values with epsilon
 *
 * @param [in] epsilon Allowed distance
 * @return True if the distance between them less then epsilon
 */
bool DoubleEqual(double first, double second, double epsilon) {
  return std::abs(first - second) < epsilon;
}

/**
 * Solves the quadratic equation \f$ ax^2 + bx + c = 0 \f$
 *
 * @param [in] a
 * @param [in] b
 * @param [in] c
 *
 * @note \p a must not be zero
 */
Roots QuadraticSolver(double a, double b, double c) {
  assert(a != 0);
  double d = Determinant(a, b, c);
  if (DoubleEqual(d, 0, EPSILON)) {
    Roots roots(1, FINITE);
    *roots.roots_ = -b / (2 * a);
    return roots;
  } else if (d > 0) {
    double sqrt_d = sqrt(d);
    Roots roots(2, FINITE);
    roots.roots_[0] = (-b - sqrt_d) / (2 * a);
    roots.roots_[1] = (-b + sqrt_d) / (2 * a);
    return roots;
  }
  return Roots(0, FINITE);
}

Polynomial::Polynomial(const std::vector<double> &params)
    : Polynomial(params.data(), params.size()) {}

Roots Polynomial::Solve() {
  switch (coefficients_size_) {
    case 0:
      return Roots(0, INFINITE);
    case 1:
      return Roots(0, FINITE);
    case 2:
      return LinearSolver(coefficients_[0], coefficients_[1]);
    case 3:
      return QuadraticSolver(coefficients_[0], coefficients_[1], coefficients_[2]);
    default:
      return Roots(0, UNKNOWN);
  }
}

Polynomial::Polynomial(const double params[], size_t params_size) {
  for (size_t i = 0; i < params_size; i++) {
    if (!std::isfinite(params[i])) {
      throw std::invalid_argument("Parameters must have finite values");
    }
  }
  size_t first_non_zero = params_size;
  for (size_t i = 0; i < params_size; i++) {
    if (params[i] != 0) {
      first_non_zero = i;
      break;
    }
  }
  coefficients_size_ = params_size - first_non_zero;
  coefficients_ = new double[coefficients_size_];
  if (coefficients_size_ != 0) {
    memcpy(coefficients_, params + first_non_zero,
           sizeof(*params) * (coefficients_size_));
  }
}

Polynomial::~Polynomial() {
  delete[] coefficients_;
}

Roots::Roots(size_t number, RootDescription description) :
    number_(number),
    description_(description),
    roots_(new double[number]) {}

Roots::~Roots() {
  delete[] roots_;
}

bool Roots::operator==(const Roots &other) const {
  if (this == &other) {
    return true;
  }
  if (number_ != other.number_) {
    return false;
  }
  if (description_ != other.description_) {
    return false;
  }
  return std::equal(roots_, roots_ + number_, other.roots_);
}

Roots::Roots(const Roots &other) : Roots(other.number_, other.description_) {
  memcpy(roots_, other.roots_, sizeof(*roots_) * number_);
}
