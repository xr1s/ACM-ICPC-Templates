#pragma once

#include "Matrix.hh"

template <typename T> bool
Matrix<T>::triangularize_(T **matrix, const size_t row, const size_t col) {
  using std::abs; using std::swap;
  bool swapped = true;
  for (size_t r = 0, c = 0; r != row && c != col; ++c) {
    size_t pivot = r;
    for (size_t i = r; i != row; ++i)
      if (abs(matrix[pivot][c]) < abs(matrix[i][c])) pivot = i;
    if (isZero(matrix[pivot][c])) continue;
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
Matrix<T>::canonicalize_(T **matrix, const size_t row, const size_t col) {
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

template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col> &
operator+=(Matrix<T, Row, Col> &lhs, const Matrix<T, Row, Col> &rhs) {
  for (size_t r = 0; r != Row; ++r)
    for (size_t c = 0; c != Col; ++c)
      lhs[r][c] += rhs[r][c];
  return lhs;
}

template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col>
operator+(const Matrix<T, Row, Col> &lhs, const Matrix<T, Row, Col> &rhs) {
  Matrix<T, Row, Col> ret = lhs;
  return ret += rhs;
}

template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col> &
operator-=(Matrix<T, Row, Col> &lhs, const Matrix<T, Row, Col> &rhs) {
  for (size_t r = 0; r != Row; ++r)
    for (size_t c = 0; c != Col; ++c)
      lhs[r][c] -= rhs[r][c];
  return lhs;
}

template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col>
operator-(const Matrix<T, Row, Col> &lhs, const Matrix<T, Row, Col> &rhs) {
  Matrix<T, Row, Col> ret = lhs;
  return ret -= rhs;
}

template <typename T, size_t Row, size_t Col, size_t Mid> Matrix<T, Row, Col>
operator*(const Matrix<T, Row, Mid> &lhs, const Matrix<T, Mid, Col> &rhs) {
  Matrix<T, Row, Col> ret;
  for (size_t r = 0; r != Row; ++r)
    for (size_t c = 0; c != Col; ++c) {
      ret[r][c] = 0;
      for (size_t k = 0; k != Mid; ++k)
        ret[r][c] += lhs[r][k] * rhs[k][c];
    }
  return ret;
}

template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col>
operator*=(Matrix<T, Row, Col> &lhs, const Matrix<T, Col, Col> &rhs) {
  Matrix<T, Row, Col> ret = lhs;
  return lhs = ret * rhs;
}
