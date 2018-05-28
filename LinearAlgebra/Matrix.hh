#pragma once

#include <algorithm>
#include <stdexcept>

#include "Util.hh"

template <typename T, size_t Row = 0, size_t Col = 0> class Matrix;

// Declare but do not define = delete.
template <typename T, size_t Row> class Matrix<T, Row, 0>;
template <typename T, size_t Col> class Matrix<T, 0, Col>;

// The size of matrix is fixed at runtime.
template <typename T, size_t Row, size_t Col>
class Matrix {
 public:
  Matrix();
  Matrix(const Matrix &);
#if __cplusplus >= 201103L
  template <typename Head, typename ...Tail>
  Matrix(const Head (&)[Col], const Tail (&...tail)[Col]);
#endif  // __cplusplus >= 201103L

  Matrix &operator=(const Matrix &);
  T *operator[](size_t row);
  const T *operator[](size_t row) const;

  size_t col() const;
  size_t row() const;


  Matrix &copyFrom(const Matrix &);
  Matrix &copyFromArray(const T (&arr)[Row][Col]);

  Matrix &triangularize();
  Matrix &eliminate();

 private:
  T value[Row][Col];
};

template <typename T, size_t Row, size_t Col>
Matrix<T, Row, Col>::Matrix() {
}

template <typename T, size_t Row, size_t Col>
Matrix<T, Row, Col>::Matrix(const Matrix &that) {
  this->copyFrom(that);
}

#if __cplusplus >= 201103L
template <typename T, size_t Row, size_t Col>
template <typename Head, typename ...Tail>
Matrix<T, Row, Col>::Matrix(const Head (&head)[Col],
                            const Tail (&...tail)[Col]) {
  static_assert(sizeof...(tail) == Row - 1, "Size not match");
  const Head *array[Row] = { head, tail... };
  for (size_t r = 0; r != Row; ++r)
    std::copy(array[r], array[r] + Col, this->value[r]);
}
#endif  // __cplusplus >= 201103L

template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col> &
Matrix<T, Row, Col>::operator=(const Matrix<T, Row, Col> &that) {
  return this->copyFrom(that);
}

template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col> &
Matrix<T, Row, Col>::copyFrom(const Matrix<T, Row, Col> &that) {
  for (size_t r = 0; r != Row; ++r)
    std::copy(that.value[r], that.value[r] + Col, this->value[r]);
  return *this;
}

template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col> &
Matrix<T, Row, Col>::copyFromArray(const T (&array)[Row][Col]) {
  for (size_t r = 0; r != Row; ++r)
    std::copy(array[r], array[r] + Col, this->value[r]);
  return *this;
}

template <typename T, size_t Row, size_t Col> T *
Matrix<T, Row, Col>::operator[](size_t row) {
  return this->value[row];
}

template <typename T, size_t Row, size_t Col> const T *
Matrix<T, Row, Col>::operator[](size_t row) const {
  return this->value[row];
}

template <typename T, size_t Row, size_t Col> size_t
Matrix<T, Row, Col>::row() const {
  return Row;
}

template <typename T, size_t Row, size_t Col> size_t
Matrix<T, Row, Col>::col() const {
  return Col;
}

template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col> &
Matrix<T, Row, Col>::triangularize() {
  T tria[Row][Col];
  for (size_t r = 0; r != Row; ++r)
#if __cplusplus >= 201103L
    std::move(this->value[r], this->value[r] + Col, tria[r]);
#else
    std::copy(this->value[r], this->value[r] + Col, tria[r]);
#endif
  T *matrix[Row];
  for (size_t r = 0; r != Row; ++r) matrix[r] = tria[r];
  triangularize_(matrix, Row, Col);
  canonicalize_(matrix, Row, Col);
  for (size_t r = 0; r != Row; ++r)
#if __cplusplus >= 201103L
    std::move(matrix[r], matrix[r] + Col, this->value[r]);
#else
    std::copy(matrix[r], matrix[r] + Col, this->value[r]);
#endif
  return *this;
}

template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col> &
Matrix<T, Row, Col>::eliminate() {
  T elim[Row][Col];
  for (size_t r = 0; r != Row; ++r)
#if __cplusplus >= 201103L
    std::move(this->value[r], this->value[r] + Col, elim[r]);
#else
    std::copy(this->value[r], this->value[r] + Col, elim[r]);
#endif
  T *matrix[Row];
  for (size_t r = 0; r != Row; ++r) matrix[r] = elim[r];
  triangularize_(matrix, Row, Col);
  canonicalize_(matrix, Row, Col);
  for (size_t r = 0; r != Row; ++r)
#if __cplusplus >= 201103L
    std::move(matrix[r], matrix[r] + Col, this->value[r]);
#else
    std::copy(matrix[r], matrix[r] + Col, this->value[r]);
#endif
  return *this;
}

// Square matrix is a special case of static matrix.
template <typename T, size_t Len>
class Matrix<T, Len, Len> {
 public:
  Matrix();
  explicit Matrix(const T &);
  Matrix(const Matrix &);
#if __cplusplus >= 201103L
  template <typename Head, typename ...Tail>
  Matrix(const Head (&)[Len], const Tail (&...tail)[Len]);
#endif

  Matrix &operator=(const Matrix &);
  T *operator[](size_t row);
  const T *operator[](size_t row) const;

  size_t row() const;
  size_t col() const;
  Matrix &copyFrom(const Matrix &);
  Matrix &copyFromArray(const T (&)[Len][Len]);

  Matrix &transpose();
  Matrix &triangularize();
  Matrix &eliminate();
  Matrix &inverse();
  T determinant() const;

 private:
  T value[Len][Len];
};

template <typename T, size_t Len>
Matrix<T, Len, Len>::Matrix() {
}

template <typename T, size_t Len>
Matrix<T, Len, Len>::Matrix(const T &value) {
  for (size_t i = 0; i != Len; ++i)
    this->value[i][i] = value;
}

template <typename T, size_t Len>
Matrix<T, Len, Len>::Matrix(const Matrix &that) {
  this->copyFrom(that);
}

#if __cplusplus >= 201103L
template <typename T, size_t Len> template <typename Head, typename ...Tail>
Matrix<T, Len, Len>::Matrix(const Head (&head)[Len],
                            const Tail (&...tail)[Len]) {
  static_assert(sizeof...(tail) == Len - 1, "Size not match");
  const Head *array[Len] = { head, tail... };
  for (size_t i = 0; i != Len; ++i)
    std::copy(array[i], array[i] + Len, this->value[i]);
}
#endif

template <typename T, size_t Len> Matrix<T, Len, Len> &
Matrix<T, Len, Len>::operator=(const Matrix<T, Len, Len> &that) {
  return this->copyFrom(that);
}

template <typename T, size_t Len> T *
Matrix<T, Len, Len>::operator[](size_t row) {
  return this->value[row];
}

template <typename T, size_t Len> const T *
Matrix<T, Len, Len>::operator[](size_t row) const {
  return this->value[row];
}

template <typename T, size_t Len> size_t
Matrix<T, Len, Len>::row() const {
  return Len;
}

template <typename T, size_t Len> size_t
Matrix<T, Len, Len>::col() const {
  return Len;
}

template <typename T, size_t Len> Matrix<T, Len, Len> &
Matrix<T, Len, Len>::copyFrom(const Matrix<T, Len, Len> &that) {
  for (size_t i = 0; i != Len; ++i)
    std::copy(that.value[i], that.value[i] + Len, this->value[i]);
  return *this;
}

template <typename T, size_t Len> Matrix<T, Len, Len> &
Matrix<T, Len, Len>::copyFromArray(const T (&array)[Len][Len]) {
  for (size_t i = 0; i != Len; ++i)
    std::copy(array[i], array[i] + Len, this->value[i]);
  return *this;
}

template <typename T, size_t Len> Matrix<T, Len, Len> &
Matrix<T, Len, Len>::transpose() {
  for (size_t i = 0; i != Len; ++i)
    for (size_t j = i + 1; j != Len; ++j)
      std::swap(this->value[i][j], this->value[j][i]);
  return *this;
}

template <typename T> bool
triangularize_(T **matrix, const size_t row, const size_t col) {
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

template <typename T, size_t Len> Matrix<T, Len, Len> &
Matrix<T, Len, Len>::triangularize() {
  T tria[Len][Len];
  for (size_t i = 0; i != Len; ++i)
#if __cplusplus >= 201103L
    std::move(this->value[i], this->value[i] + Len, tria[i]);
#else
    std::copy(this->value[i], this->value[i] + Len, tria[i]);
#endif
  T *matrix[Len];
  for (size_t i = 0; i != Len; ++i) matrix[i] = tria[i];
  triangularize_(matrix, Len, Len);
  canonicalize_(matrix, Len, Len);
  for (size_t i = 0; i != Len; ++i)
#if __cplusplus >= 201103L
    std::move(matrix[i], matrix[i] + Len, this->value[i]);
#else
    std::copy(matrix[i], matrix[i] + Len, this->value[i]);
#endif
  return *this;
}

template <typename T, size_t Len> Matrix<T, Len, Len> &
Matrix<T, Len, Len>::eliminate() {
  T elim[Len][Len];
  for (size_t i = 0; i != Len; ++i)
#if __cplusplus >= 201103L
    std::move(this->value[i], this->value[i] + Len, elim[i]);
#else
    std::copy(this->value[i], this->value[i] + Len, elim[i]);
#endif
  T *matrix[Len];
  for (size_t i = 0; i != Len; ++i) matrix[i] = elim[i];
  triangularize_(matrix, Len, Len);
  canonicalize_(matrix, Len, Len);
  for (size_t i = 0; i != Len; ++i)
#if __cplusplus >= 201103L
    std::move(matrix[i], matrix[i] + Len, this->value[i]);
#else
    std::copy(matrix[i], matrix[i] + Len, this->value[i]);
#endif
  return *this;
}

template <typename T, size_t Len> T
Matrix<T, Len, Len>::determinant() const {
  Matrix copy = *this;
  T *matrix[Len];
  for (size_t i = 0; i != Len; ++i) matrix[i] = copy.value[i];
  T result = triangularize_(matrix, Len, Len) ? 1 : -1;
  for (size_t i = 0; i != Len; ++i)
    if (isZero(result *= matrix[i][i])) return 0;
  return isZero(result) ? 0 : result;
}

// Will throw std::invalid_argument if not inversible.
template <typename T, size_t Len> Matrix<T, Len, Len> &
Matrix<T, Len, Len>::inverse() {
  T concat[Len][Len * 2];
  T *matrix[Len];
  for (size_t i = 0; i != Len; ++i) {
    std::fill(concat[i], concat[i] + Len * 2, 0);
#if __cplusplus >= 201103L
    std::move(this->value[i], this->value[i] + Len, concat[i]);
#else
    std::copy(this->value[i], this->value[i] + Len, concat[i]);
#endif
    concat[i][Len + i] = 1;
    matrix[i] = concat[i];
  }
  triangularize_(matrix, Len, Len * 2);

  // Give up if not inversible
  for (size_t i = 0; i != Len; ++i)
    if (isZero(matrix[i][i]))
      throw std::invalid_argument("Matrix::inverse");

  canonicalize_(matrix, Len, Len * 2);
  for (size_t i = 0; i != Len; ++i) {
#if __cplusplus >= 201103L
    std::move(matrix[i] + Len, matrix[i] + Len * 2, this->value[i]);
#else
    std::copy(matrix[i] + Len, matrix[i] + Len * 2, this->value[i]);
#endif
  }
  return *this;
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
