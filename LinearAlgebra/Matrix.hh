#pragma once

#include <algorithm>
#include <stdexcept>

#include "MatrixDecl.hh"

template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col> &
operator+=(Matrix<T, Row, Col> &, const Matrix<T, Row, Col> &);
template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col> &
operator-=(Matrix<T, Row, Col> &, const Matrix<T, Row, Col> &);
template <typename T, size_t Row, size_t Col> bool
operator==(const Matrix<T, Row, Col> &, const Matrix<T, Row, Col> &);
template <typename T, size_t Row, size_t Col> bool
operator!=(const Matrix<T, Row, Col> &, const Matrix<T, Row, Col> &);

// Matrix<T, Row, Col> is fixed-sized at runtime.
// Only the Matrix<T, Len, Len>, a.k.a. square matrix
// defines member function inverse and determinant.
// No function except inverse throws exception.
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

  friend Matrix &operator+=<>(Matrix &, const Matrix &);
  friend Matrix &operator-=<>(Matrix &, const Matrix &);
  friend bool operator==<>(const Matrix &, const Matrix &);
  friend bool operator!=<>(const Matrix &, const Matrix &);

 private:
  T value[Row][Col];
};

template <typename T, size_t Row, size_t Col>
Matrix<T, Row, Col>::Matrix() {
  std::fill(*this->value, *this->value + Row * Col, 0);
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
  // Well, std::begin and std::end are redundancy for arrays.
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
  return this->copyFromArray(that.value);
}

template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col> &
Matrix<T, Row, Col>::copyFromArray(const T (&array)[Row][Col]) {
  std::copy(*array, *array + Row * Col, *this->value);
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
  T tria[Row][Col], *matrix[Row];
  ifCpp11MoveElseCopy(*this->value, *this->value + Row * Col, *tria);
  std::copy(tria, tria + Row, matrix);
  triangularize_(matrix, Row, Col);
  for (size_t r = 0; r != Row; ++r)
    ifCpp11MoveElseCopy(matrix[r], matrix[r] + Col, this->value[r]);
  return *this;
}

template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col> &
Matrix<T, Row, Col>::eliminate() {
  T elim[Row][Col], *matrix[Row];
  ifCpp11MoveElseCopy(*this->value, *this->value + Row * Col, *elim);
  std::copy(elim, elim + Row, matrix);
  triangularize_(matrix, Row, Col);
  canonicalize_(matrix, Row, Col);
  for (size_t r = 0; r != Row; ++r)
    ifCpp11MoveElseCopy(matrix[r], matrix[r] + Col, this->value[r]);
  return *this;
}

// Matrix<T, Len, Len> is square matrix,
// which is a special case of fix-sized matrix.
template <typename T, size_t Len>
class Matrix<T, Len, Len> {
 public:
  Matrix();
  Matrix(const T &);
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

  friend Matrix &operator+=<>(Matrix &, const Matrix &);
  friend Matrix &operator-=<>(Matrix &, const Matrix &);

 private:
  T value[Len][Len];
};

template <typename T, size_t Len>
Matrix<T, Len, Len>::Matrix() {
  std::fill(*this->value, *this->value + Len, 0);
}

template <typename T, size_t Len>
Matrix<T, Len, Len>::Matrix(const T &init) {
  for (size_t i = 0; i != Len; ++i) this->value[i][i] = init;
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
  return this->copyFromArray(that.value);
}

template <typename T, size_t Len> Matrix<T, Len, Len> &
Matrix<T, Len, Len>::copyFromArray(const T (&array)[Len][Len]) {
  std::copy(*array, *array + Len * Len, *this->value);
  return *this;
}

template <typename T, size_t Len> Matrix<T, Len, Len> &
Matrix<T, Len, Len>::transpose() {
  for (size_t i = 0; i != Len; ++i)
    for (size_t j = i + 1; j != Len; ++j)
      std::swap(this->value[i][j], this->value[j][i]);
  return *this;
}

template <typename T, size_t Len> Matrix<T, Len, Len> &
Matrix<T, Len, Len>::triangularize() {
  T tria[Len][Len], *matrix[Len];
  ifCpp11MoveElseCopy(*this->value, *this->value + Len * Len, *tria);
  std::copy(tria, tria + Len, matrix);
  triangularize_(matrix, Len, Len);
  for (size_t i = 0; i != Len; ++i)
    ifCpp11MoveElseCopy(matrix[i], matrix[i] + Len, this->value[i]);
  return *this;
}

template <typename T, size_t Len> Matrix<T, Len, Len> &
Matrix<T, Len, Len>::eliminate() {
  T elim[Len][Len], *matrix[Len];
  ifCpp11MoveElseCopy(*this->value, *this->value + Len * Len, *elim);
  std::copy(elim, elim + Len, matrix);
  triangularize_(matrix, Len, Len);
  canonicalize_(matrix, Len, Len);
  for (size_t i = 0; i != Len; ++i)
    ifCpp11MoveElseCopy(matrix[i], matrix[i] + Len, this->value[i]);
  return *this;
}

template <typename T, size_t Len> T
Matrix<T, Len, Len>::determinant() const {
  Matrix temporary = *this;
  T *matrix[Len];
  for (size_t i = 0; i != Len; ++i) matrix[i] = temporary.value[i];
  T result = triangularize_(matrix, Len, Len) ? 1 : -1;
  for (size_t i = 0; i != Len; ++i)
    if (isZero(result *= matrix[i][i])) return 0;
  return result;
}

// Throws std::invalid_argument if not inversible.
template <typename T, size_t Len> Matrix<T, Len, Len> &
Matrix<T, Len, Len>::inverse() {
  T concat[Len][Len * 2], *matrix[Len];
  for (size_t i = 0; i != Len; ++i) {
    ifCpp11MoveElseCopy(this->value[i], this->value[i] + Len, concat[i]);
    std::fill(concat[i] + Len, concat[i] + Len * 2, 0);
    concat[i][Len + i] = 1;
  }
  std::copy(concat, concat + Len, matrix);
  triangularize_(matrix, Len, Len * 2);
  // Give up if not inversible
  for (size_t i = 0; i != Len; ++i)
    if (isZero(matrix[i][Len + i]))
      throw std::invalid_argument("Matrix::inverse");
  canonicalize_(matrix, Len, Len * 2);
  for (size_t i = 0; i != Len; ++i)
    ifCpp11MoveElseCopy(matrix[i] + Len, matrix[i] + Len * 2, this->value[i]);
  return *this;
}

template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col> &
operator+=(Matrix<T, Row, Col> &lhs, const Matrix<T, Row, Col> &rhs) {
  std::transform(*lhs.value, *lhs.value + Row * Col,
                 *rhs.value, *lhs.value, std::plus<T>());
  return lhs;
}

template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col>
operator+(const Matrix<T, Row, Col> &lhs, const Matrix<T, Row, Col> &rhs) {
  Matrix<T, Row, Col> result = lhs;
  return result += rhs;
}

template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col> &
operator-=(Matrix<T, Row, Col> &lhs, const Matrix<T, Row, Col> &rhs) {
  std::transform(*lhs.value, *lhs.value + Row * Col,
                 *rhs.value, *lhs.value, std::minus<T>());
  return lhs;
}

template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col>
operator-(const Matrix<T, Row, Col> &lhs, const Matrix<T, Row, Col> &rhs) {
  Matrix<T, Row, Col> result = lhs;
  return result -= rhs;
}

template <typename T, size_t Row, size_t Col, size_t Mid> Matrix<T, Row, Col>
operator*(const Matrix<T, Row, Mid> &lhs, const Matrix<T, Mid, Col> &rhs) {
  Matrix<T, Row, Col> result;
  for (size_t r = 0; r != Row; ++r)
    for (size_t c = 0; c != Col; ++c) {
      result[r][c] = 0;
      for (size_t k = 0; k != Mid; ++k)
        result[r][c] += lhs[r][k] * rhs[k][c];
    }
  return result;
}

template <typename T, size_t Row, size_t Col> Matrix<T, Row, Col>
operator*=(Matrix<T, Row, Col> &lhs, const Matrix<T, Col, Col> &rhs) {
  Matrix<T, Row, Col> result = lhs;
  return lhs = result * rhs;
}

template <typename T, size_t Row, size_t Col> bool
operator==(const Matrix<T, Row, Col> &lhs, const Matrix<T, Row, Col> &rhs) {
  return std::equal(*lhs.value, *lhs.value + Row * Col, *rhs.value);
}

template <typename T, size_t Row, size_t Col> bool
operator!=(const Matrix<T, Row, Col> &lhs, const Matrix<T, Row, Col> &rhs) {
  return !(lhs == rhs);
}
