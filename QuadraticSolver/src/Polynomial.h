/**
 * MIPT | DIHT | 3 course | Industrial programming <br>
 * Purpose: solve polynomial equation up to 2 degree
 *
 * @file
 * @author Mary Feofanova
 */

#ifndef SOLVER_POLYNOMIAL_H
#define SOLVER_POLYNOMIAL_H

#include <vector>

/**
 * Enum type to specify what kind of roots Root is representing.
 */
enum RootDescription {
  INFINITE, FINITE, UNKNOWN
};

/**
 * Roots with all needed additional info.
 */
struct Roots {
  Roots(size_t number, RootDescription description);
  Roots(const Roots& other);
  ~Roots();

  bool operator==(const Roots &other) const;

  /**
   * Array of roots.
   * @note makes sense if description_ == FINITE
   */
  double *const roots_;
  /**
   * Number of roots.
   * @note makes sense if description_ == FINITE
   */
  const size_t number_;
  /**
   * If algorithm doesn't know how to find roots, it is UNKNOWN. <br>
   * In other case, it says whether the number of roots if finite or not.
   */
  const RootDescription description_;
};

class Polynomial {
 public:

  /**
   * Creates polynomial with coefficients in the field of the rational numbers. <br>
   * E.g., Polynomial({0, 1, 2, 3}) creates \f$ x^2 + 2x + 3 \f$
   *
   * @param [in] params Coefficients in the descending order
   * @throw invalid_argument If some coefficient is inf or NaN
   */
  explicit Polynomial(const std::vector<double> &params);

  /**
   * C-style constructor.
   * @see Polynomial::Polynomial
   */
  explicit Polynomial(const double params[], std::size_t params_size);

  ~Polynomial();

  /**
   * Solves polynomial equation. <br>
   * Now implementation can only solve up to 2 degree. <br>
   * For more information about answer form, check Roots.
   *
   * E.g., Polynomial({1, -2, 1}).Solve() is equal to solving \f$ x^2 - 2x + 1 = 0 \f$
   *
   * @return Roots with additional info
   */
  Roots Solve();

 private:
  double *coefficients_ = nullptr;
  size_t coefficients_size_ = 0;
};

#endif //SOLVER_POLYNOMIAL_H
