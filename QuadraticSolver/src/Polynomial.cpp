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
 * Compares two double values with epsilon
 *
 * @param [in] epsilon Allowed distance
 * @return True if the distance between them less then epsilon
 */
bool Equal(double first, double second, double epsilon) {
  return std::abs(first - second) < epsilon;
}

/**
 * Solves the linear equation \f$ ax + b = 0 \f$
 *
 * @param [in] a
 * @param [in] b
 */
Roots LinearSolver(double a, double b) {
  if(Equal(a, 0, EPSILON) && Equal(b, 0, EPSILON)) {
    return Roots(0, INFINITE);
  }
  if (Equal(a, 0, EPSILON)) {
    return Roots(0, FINITE);
  }
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
  assert(!Equal(a, 0, EPSILON));
  return b * b - 4 * a * c;
}

/**
 * Solves the quadratic equation \f$ ax^2 + bx + c = 0 \f$
 *
 * @param [in] a
 * @param [in] b
 * @param [in] c
 */
Roots QuadraticSolver(double a, double b, double c) {
  if (Equal(a, 0, EPSILON)) {
    return LinearSolver(b, c);
  }
  double d = Determinant(a, b, c);
  if (Equal(d, 0, EPSILON)) {
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
  if (coefficients_size_ <= 3) {
    return QuadraticSolver(GetCoefficient(2), GetCoefficient(1), GetCoefficient(0));
  }
  return Roots(0, UNKNOWN);
}

Polynomial::Polynomial(const double params[], size_t params_size) {
  for (size_t i = 0; i < params_size; i++) {
    if (!std::isfinite(params[i])) {
      throw std::invalid_argument("Parameters must have finite values");
    }
  }
  size_t first_non_zero = params_size;
  for (size_t i = 0; i < params_size; i++) {
    if (!Equal(params[i], 0, EPSILON)) {
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

double Polynomial::GetCoefficient(size_t i) {
  return i < coefficients_size_ ? coefficients_[coefficients_size_ - i - 1] : 0;
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
