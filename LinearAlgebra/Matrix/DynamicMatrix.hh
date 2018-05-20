#pragma once

#include "Matrix.hh"
#include <algorithm>  // std::swap
#include <stdexcept>  // std::invalid_argument

template <typename T> Matrix<T> &
operator+=(Matrix<T> &, const Matrix<T> &);
template <typename T> Matrix<T> &
operator-=(Matrix<T> &, const Matrix<T> &);
template <typename T> Matrix<T> &
operator*=(Matrix<T> &, const Matrix<T> &);

template <typename T>
class Matrix<T, 0, 0> {
 public:
  size_t row, col;

  Matrix();
  Matrix(const Matrix &);
  Matrix &operator=(const Matrix &);
#if __cplusplus >= 201103L
  Matrix(Matrix &&);
  Matrix &operator=(Matrix &&);
#endif  // __cplusplus >= 201103L
  ~Matrix(); 
  Matrix(size_t row, size_t col);

#if __cplusplus >= 201103L
  template <typename ...Ta, size_t Col>
  Matrix(const Ta (&...arr)[Col]);
#endif  // __cplusplus >= 201103L

  Matrix &copyFrom(const Matrix &);
  Matrix &moveFrom(Matrix &);
  template <size_t Row, size_t Col>
  Matrix &copyFromArray(const T (&arr)[Row][Col]);

  T *operator[](size_t row);
  const T *operator[](size_t row) const;
  friend Matrix &operator+=<>(Matrix &, const Matrix &);
  friend Matrix &operator-=<>(Matrix &, const Matrix &);
  friend Matrix &operator*=<>(Matrix &, const Matrix &);

  Matrix &transpose();
  Matrix &triangularize();
  Matrix &eliminate();
  Matrix &inverse();
  T determinant() const;

 private:
  T *value;

  static bool triangularize_(T **, const size_t, const size_t);
  static void canonicalize_(T **, const size_t, const size_t);

  template <typename, size_t, size_t> friend class Matrix;
};

template <typename T>
Matrix<T>::Matrix()
    : row(0), col(0), value(NULL) {
}

template <typename T>
Matrix<T>::Matrix(const Matrix &that) {
  this->copyFrom(that);
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix &that) {
  this->~Matrix();
  this->copyFrom(that);
  return *this;
}

#if __cplusplus >= 201103L
template <typename T>
Matrix<T>::Matrix(Matrix &&that) {
  this->moveFrom(that);
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(Matrix &&that) {
  this->~Matrix();
  this->moveFrom(that);
  return *this;
}
#endif  // __cplusplus >= 201103L

template <typename T>
Matrix<T>::~Matrix() {
  delete[] this->value, this->value = NULL;
}

template <typename T>
Matrix<T>::Matrix(size_t row, size_t col)
    : row(row), col(row) {
  this->value = new T[row * col];
}

#if __cplusplus >= 201103L
template <typename T> template <typename ...Ta, size_t Col>
Matrix<T>::Matrix(const Ta (&...arr)[Col])
    : row{sizeof...(arr)}, col{Col} {
  const T *array[sizeof...(arr)] = { arr... };
  this->value = new T[this->row * this->col];

  size_t i = 0;
  for (size_t r = 0; r != this->row; ++r)
    for (size_t c = 0; c != this->col; ++c)
      this->value[i++] = array[r][c];
}
#endif  // __cplusplus >= 201103L

template <typename T> Matrix<T> &
Matrix<T>::copyFrom(const Matrix &that) {
  const size_t size = that.row * that.col;
  this->row = that.row;
  this->col = that.col;
  this->value = new T[size];
  std::copy(that.value, that.value + size, this->value);
  return *this;
}

template <typename T> Matrix<T> &
Matrix<T>::moveFrom(Matrix &that) {
  this->row = that.row;
  this->col = that.col;
  delete[] this->value;
  this->value = that.value;
  that.row = that.col = 0;
  that.value = NULL;
  return *this;
}

template <typename T> template <size_t Row, size_t Col> Matrix<T> &
Matrix<T>::copyFromArray(const T (&arr)[Row][Col]) {
  const size_t size = Row * Col;
  this->row = Row; this->col = Col;
  this->value = new T[size];
  for (size_t i = 0; i != Row; ++i)
    std::copy(arr[i], arr[i] + Col, this->value + i * Col);
  return *this;
}

template <typename T> T *
Matrix<T>::operator[](size_t row) {
  return &this->value[row * this->col];
}

template <typename T> const T *
Matrix<T>::operator[](size_t row) const {
  return &this->value[row * this->col];
}


template <typename T> Matrix<T, 0, 0> &
operator+=(Matrix<T, 0, 0> &lhs, const Matrix<T, 0, 0> &rhs) {
  if (lhs.row != rhs.row || lhs.col != rhs.col)
    throw std::invalid_argument("Matrix<T>::operator+=");
  const size_t size = lhs.row * lhs.col;
  for (size_t i = 0; i != size; ++i)
    lhs.value[i] += rhs.value[i];
  return lhs;
}

template <typename T> Matrix<T>
operator+(const Matrix<T> &lhs, const Matrix<T> &rhs) {
  if (lhs.row != rhs.row || lhs.col != rhs.col)
    throw std::invalid_argument("Matrix<T>::operator+");
  Matrix<T> ret = lhs;
  return ret += rhs;
}

template <typename T> Matrix<T> &
operator-=(Matrix<T> &lhs, const Matrix<T> &rhs) {
  if (lhs.row != rhs.row || lhs.col != rhs.col)
    throw std::invalid_argument("Matrix<T>::operator-=");
  const size_t size = lhs.row * lhs.col;
  for (size_t i = 0; i != size; ++i)
    lhs.value[i] -= rhs.value[i];
  return lhs;
}

template <typename T> Matrix<T>
operator-(const Matrix<T> &lhs, const Matrix<T> &rhs) {
  if (lhs.row != rhs.row || lhs.col != rhs.col)
    throw std::invalid_argument("Matrix<T>::operator-");
  Matrix<T> ret = lhs;
  return ret -= rhs;
}

template <typename T> Matrix<T> &
operator*=(Matrix<T> &lhs, const Matrix<T> &rhs) {
  if (lhs.col != rhs.row)
    throw std::invalid_argument("Matrix<T>::operator*=");
  Matrix<T> ret(lhs.row, rhs.col);
  for (size_t i = 0; i != ret.row * ret.col; ++i) ret.value[i] = 0;
  for (size_t r = 0; r != ret.row; ++r)
    for (size_t c = 0; c != ret.col; ++c) {
      for (size_t i = 0; i != lhs.col; ++i)
        ret[r][c] += lhs[r][i] * rhs[i][c];
    }
  return lhs.moveFrom(ret);
}

template <typename T> Matrix<T>
operator*(const Matrix<T> &lhs, const Matrix<T> &rhs) {
  if (lhs.col != rhs.row)
    throw std::invalid_argument("Matrix<T>::operator*");
  Matrix<T> ret = lhs;
  return ret *= rhs;
}

template <typename T> Matrix<T> &
Matrix<T>::transpose() {
  if (this->row != this->col)
    throw std::invalid_argument("Matrix<T>::transpose");
  const size_t &row = this->row, &col = this->col;
  T *const &value = this->value;
  for (size_t i = 0; i != row; ++i)
    for (size_t j = i + 1; j != col; ++j)
      std::swap(value[i * col + j], value[j * col + i]);
  return *this;
}

template <typename T> Matrix<T> &
Matrix<T>::triangularize() {
  T **matrix = new T *[row];
  for (size_t i = 0; i != this->row; ++i)
    matrix[i] = this->value + i * this->col;
  Matrix<T>::triangularize_(matrix, this->row, this->col);
  T *value = new T[row * col];
  for (size_t r = 0; r != this->row; ++r)
#if __cpluslus >= 201103L
    std::move(matrix[r], matrix[r] + col, value + r * this->col);
#else
    std::copy(matrix[r], matrix[r] + col, value + r * this->col);
#endif
  delete[] matrix, delete[] this->value;
  this->value = value;
  return *this;
}

template <typename T> Matrix<T> &
Matrix<T>::eliminate() {
  T **matrix = new T *[row];
  for (size_t i = 0; i != this->row; ++i)
    matrix[i] = this->value + i * this->col;
  Matrix<T>::triangularize_(matrix, this->row, this->col);
  Matrix<T>::canonicalize_(matrix, this->row, this->col);
  T *value = new T[row * col];
  for (size_t r = 0; r != this->row; ++r)
#if __cpluslus >= 201103L
    std::move(matrix[r], matrix[r] + col, value + r * this->col);
#else
    std::copy(matrix[r], matrix[r] + col, value + r * this->col);
#endif
  delete[] matrix, delete[] this->value;
  this->value = value;
  return *this;
}

template <typename T> Matrix<T> &
Matrix<T>::inverse() {
  if (this->row != this->col)
    throw std::invalid_argument("Matrix<T>::inverse");

  T *concat = new T[row * col * 2];
  T **matrix = new T *[row];
  std::fill(concat, concat + row * col * 2, 0);

  for (size_t i = 0; i != row; ++i) {
#if __cplusplus >= 201103L
    std::move(value + i * col, value + i * col + col, concat + i * col * 2);
#else
    std::copy(value + i * col, value + i * col + col, concat + i * col * 2);
#endif
    concat[(i * 2 + 1) * col + i] = 1;
    matrix[i] = concat + i * col * 2;
  }
  Matrix<T>::triangularize_(matrix, row, col * 2);

  for (size_t i = 0; i != this->row; ++i)
    if (isZero(matrix[i][i])) {
      delete[] matrix, delete[] concat;
      throw std::invalid_argument("Matrix::inverse");
    }

  Matrix<T>::canonicalize_(matrix, row, col * 2);

  for (size_t i = 0; i != row; ++i)
#if __cplusplus >= 201103L
    std::move(matrix[i] + col, matrix[i] + col * 2, value + i * col);
#else
    std::copy(matrix[i] + col, matrix[i] + col * 2, value + i * col);
#endif

  delete[] matrix, delete[] concat;
  return *this;
}

template <typename T> T
Matrix<T>::determinant() const {
  if (this->row != this->col)
    throw std::invalid_argument("Matrix<T>::determinant");
  Matrix copy = *this;
  T **matrix = new T *[this->row];
  for (size_t i = 0; i != this->row; ++i)
    matrix[i] = copy.value + i * this->col;
  T result = Matrix<T>::triangularize_(matrix, this->row, this->col) ? 1 : -1;
  for (size_t i = 0; i != this->row; ++i)
    if (!isZero(result)) result *= matrix[i][i];
  delete[] matrix;
  return isZero(result) ? 0 : result;
}
