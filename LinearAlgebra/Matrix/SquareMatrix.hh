#pragma once

#include "Matrix.hh"

#include <algorithm>  // std::swap

// Square matrix is a special case of static matrix.
template <typename T, size_t Len>
class Matrix<T, Len, Len> {
 public:
  Matrix();
  explicit Matrix(const T &);
  Matrix(const Matrix &);
#if __cplusplus >= 201103L
  template <typename ...Ta>
  Matrix(const Ta (&...arr)[Len]);
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
template <typename T, size_t Len> template <typename ...Ta>
Matrix<T, Len, Len>::Matrix(const Ta (&...arr)[Len])
    : row{Len}, col{Len} {
  static_assert(sizeof...(arr) == Len, "Size not match");
  const T *array[Len] = { arr... };
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
  Matrix<T>::triangularize_(matrix, Len, Len);
  Matrix<T>::canonicalize_(matrix, Len, Len);
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
  Matrix<T>::triangularize_(matrix, Len, Len);
  Matrix<T>::canonicalize_(matrix, Len, Len);
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
  T result = Matrix<T>::triangularize_(matrix, Len, Len) ? 1 : -1;
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
  Matrix<T>::triangularize_(matrix, Len, Len * 2);

  // Give up if not inversible
  for (size_t i = 0; i != Len; ++i)
    if (isZero(matrix[i][i]))
      throw std::invalid_argument("Matrix::inverse");

  Matrix<T>::canonicalize_(matrix, Len, Len * 2);
  for (size_t i = 0; i != Len; ++i) {
#if __cplusplus >= 201103L
    std::move(matrix[i] + Len, matrix[i] + Len * 2, this->value[i]);
#else
    std::copy(matrix[i] + Len, matrix[i] + Len * 2, this->value[i]);
#endif
  }
  return *this;
}
