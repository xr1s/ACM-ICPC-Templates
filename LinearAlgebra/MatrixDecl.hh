#pragma once

#include <algorithm>  // std::abs

template <typename T, size_t Row = 0, size_t Col = 0> class Matrix;

// Declare but not define, = delete.
template <typename T, size_t Row> class Matrix<T, Row, 0>;
template <typename T, size_t Col> class Matrix<T, 0, Col>;

template <typename T>
inline bool isZero(T value) {
  return value == T(0);
}

template <>
inline bool isZero<float>(float value) {
  return std::abs(value) <= 1e-4;
}

template <>
inline bool isZero<double>(double value) {
  return std::abs(value) <= 1e-8;
}

template <>
inline bool isZero<long double>(long double value) {
  return std::abs(value) <= 1e-10;
}

template <typename InputIter>
void ifCpp11MoveElseCopy(InputIter begin, InputIter end, InputIter dest) {
  std::
#if __cplusplus >= 201103L
       move
#else
       copy
#endif
           (begin, end, dest);
}

// The following two functions should be private and never invoked directly.
// They were originally static member function of Matrix<T>.
// However I tries to decoupling Matrix<T> (variable-sized matrix) and
// Matrix<T, Row, Col> (fix-sized matrix).
// To prevent inclusion (and specialization) from fix-sized matrix to
// variable-sized matrix, I declare and defined the two functions here.
template <typename T> bool
triangularize_(T **matrix, const size_t row, const size_t col) {
  using std::swap;
  bool swapped = true;
  for (size_t r = 0, c = 0; r != row && c != col; ++c) {
    size_t pivot = ~static_cast<size_t>(0);
    for (size_t i = r; !~pivot && i != row; ++i)
      if (!isZero(matrix[i][c])) pivot = i;
    if (pivot == ~static_cast<size_t>(0)) continue;
    swap(matrix[pivot], matrix[r++]); swapped ^= 1;
    for (size_t i = r; i != row; ++i) {
      T f = matrix[i][c] / matrix[r - 1][c];
      for (size_t j = c + 1; j != col; ++j)
        matrix[i][j] -= matrix[r - 1][j] * f;
      matrix[i][c] = 0;
    }
  }
  return swapped;
}

template <typename T> void
canonicalize_(T **matrix, const size_t row, const size_t col) {
  for (size_t r = row - 1, c; ~r; --r) {
    for (c = 0; c != col && isZero(matrix[r][c]); ++c);
    if (c == col) continue;
    for (size_t i = c + 1; i != col; ++i)
      matrix[r][i] /= matrix[r][c];
    matrix[r][c] = 1;
    for (size_t i = 0; i != r; ++i) {
      for (size_t j = c + 1; j != col; ++j)
        matrix[i][j] -= matrix[r][j] * matrix[i][c];
      matrix[i][c] = 0;
    }
  }
}
