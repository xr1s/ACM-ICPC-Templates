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

// Matrix<T, 0, 0> is able to change size at runtime.
// This will throw std::invalid_argument if the numbers of column and row do not
// satisfy the requirement. For example, operator+(A, B) requires A and B are
// of the same size.
template <typename T>
class Matrix<T, 0, 0> {
 public:

  Matrix();
  Matrix(const Matrix &);
  Matrix &operator=(const Matrix &);
#if __cplusplus >= 201103L
  Matrix(Matrix &&);
  Matrix &operator=(Matrix &&);
#endif  // __cplusplus >= 201103L
  ~Matrix(); 
  Matrix(size_t, size_t);

#if __cplusplus >= 201103L
  template <typename ...Ta, size_t Col>
  Matrix(const Ta (&...arr)[Col]);
#endif  // __cplusplus >= 201103L

  Matrix &copyFrom(const Matrix &);
  Matrix &moveFrom(Matrix &);
  template <size_t Row, size_t Col>
  Matrix &copyFromArray(const T (&arr)[Row][Col]);

  T *operator[](size_t);
  const T *operator[](size_t) const;
  friend Matrix &operator+=<>(Matrix &, const Matrix &);
  friend Matrix &operator-=<>(Matrix &, const Matrix &);
  friend Matrix &operator*=<>(Matrix &, const Matrix &);

  size_t row() const;
  size_t col() const;

  Matrix &transpose();
  Matrix &triangularize();
  Matrix &eliminate();
  Matrix &inverse();
  T determinant() const;

 private:
  size_t row_, col_;
  T *value;

  static bool triangularize_(T **, const size_t, const size_t);
  static void canonicalize_(T **, const size_t, const size_t);

  template <typename, size_t, size_t> friend class Matrix;
};

template <typename T>
Matrix<T>::Matrix()
    : row_(0), col_(0), value(NULL) {
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
    : row_(row), col_(row) {
  this->value = new T[row * col];
}

#if __cplusplus >= 201103L
template <typename T> template <typename ...Ta, size_t Col>
Matrix<T>::Matrix(const Ta (&...arr)[Col])
    : row_{sizeof...(arr)}, col_{Col} {
  const T *array[sizeof...(arr)] = { arr... };
  this->value = new T[this->row_ * this->col_];

  size_t i = 0;
  for (size_t r = 0; r != this->row_; ++r)
    for (size_t c = 0; c != this->col_; ++c)
      this->value[i++] = array[r][c];
}
#endif  // __cplusplus >= 201103L

template <typename T> Matrix<T> &
Matrix<T>::copyFrom(const Matrix &that) {
  const size_t size = that.row_ * that.col_;
  this->row_ = that.row_;
  this->col_ = that.col_;
  this->value = new T[size];
  std::copy(that.value, that.value + size, this->value);
  return *this;
}

template <typename T> Matrix<T> &
Matrix<T>::moveFrom(Matrix &that) {
  this->row_ = that.row_;
  this->col_ = that.col_;
  delete[] this->value;
  this->value = that.value;
  that.row_ = that.col_ = 0;
  that.value = NULL;
  return *this;
}

template <typename T> template <size_t Row, size_t Col> Matrix<T> &
Matrix<T>::copyFromArray(const T (&arr)[Row][Col]) {
  const size_t size = Row * Col;
  this->row_ = Row; this->col_ = Col;
  this->value = new T[size];
  for (size_t i = 0; i != Row; ++i)
    std::copy(arr[i], arr[i] + Col, this->value + i * Col);
  return *this;
}

template <typename T> T *
Matrix<T>::operator[](size_t row) {
  return &this->value[row * this->col_];
}

template <typename T> const T *
Matrix<T>::operator[](size_t row) const {
  return &this->value[row * this->col_];
}


template <typename T> Matrix<T, 0, 0> &
operator+=(Matrix<T, 0, 0> &lhs, const Matrix<T, 0, 0> &rhs) {
  if (lhs.row_ != rhs.row_ || lhs.col_ != rhs.col_)
    throw std::invalid_argument("Matrix<T>::operator+=");
  const size_t size = lhs.row_ * lhs.col_;
  for (size_t i = 0; i != size; ++i)
    lhs.value[i] += rhs.value[i];
  return lhs;
}

template <typename T> Matrix<T>
operator+(const Matrix<T> &lhs, const Matrix<T> &rhs) {
  if (lhs.row() != rhs.row() || lhs.col() != rhs.col())
    throw std::invalid_argument("Matrix<T>::operator+");
  Matrix<T> ret = lhs;
  return ret += rhs;
}

template <typename T> Matrix<T> &
operator-=(Matrix<T> &lhs, const Matrix<T> &rhs) {
  if (lhs.row_ != rhs.row_ || lhs.col_ != rhs.col_)
    throw std::invalid_argument("Matrix<T>::operator-=");
  const size_t size = lhs.row_ * lhs.col_;
  for (size_t i = 0; i != size; ++i)
    lhs.value[i] -= rhs.value[i];
  return lhs;
}

template <typename T> Matrix<T>
operator-(const Matrix<T> &lhs, const Matrix<T> &rhs) {
  if (lhs.row() != rhs.row() || lhs.col() != rhs.col())
    throw std::invalid_argument("Matrix<T>::operator-");
  Matrix<T> ret = lhs;
  return ret -= rhs;
}

template <typename T> Matrix<T> &
operator*=(Matrix<T> &lhs, const Matrix<T> &rhs) {
  if (lhs.col_ != rhs.row_)
    throw std::invalid_argument("Matrix<T>::operator*=");
  Matrix<T> ret(lhs.row_, rhs.col_);
  std::fill(ret.value, ret.value + ret.row_ * ret.col_, 0);
  for (size_t r = 0; r != ret.row_; ++r)
    for (size_t c = 0; c != ret.col_; ++c) {
      for (size_t i = 0; i != lhs.col_; ++i)
        ret[r][c] += lhs[r][i] * rhs[i][c];
    }
  return lhs.moveFrom(ret);
}

template <typename T> Matrix<T>
operator*(const Matrix<T> &lhs, const Matrix<T> &rhs) {
  if (lhs.col() != rhs.row())
    throw std::invalid_argument("Matrix<T>::operator*");
  Matrix<T> ret = lhs;
  return ret *= rhs;
}

template <typename T> size_t
Matrix<T>::row() const {
  return this->row_;
}

template <typename T> size_t
Matrix<T>::col() const {
  return this->col_;
}

template <typename T> Matrix<T> &
Matrix<T>::transpose() {
  if (this->row_ != this->col_)
    throw std::invalid_argument("Matrix<T>::transpose");
  const size_t row = this->row_, col = this->col;
  T *const &value = this->value;
  for (size_t i = 0; i != row; ++i)
    for (size_t j = i + 1; j != col; ++j)
      std::swap(value[i * col + j], value[j * col + i]);
  return *this;
}

template <typename T> Matrix<T> &
Matrix<T>::triangularize() {
  const size_t row = this->row_, col = this->col_;
  T **matrix = new T *[row];
  for (size_t i = 0; i != row; ++i)
    matrix[i] = this->value + i * col;
  Matrix<T>::triangularize_(matrix, row, col);
  T *value = new T[row * col];
  for (size_t r = 0; r != row; ++r)
#if __cpluslus >= 201103L
    std::move(matrix[r], matrix[r] + col, value + r * col);
#else
    std::copy(matrix[r], matrix[r] + col, value + r * col);
#endif
  delete[] matrix, delete[] this->value;
  this->value = value;
  return *this;
}

template <typename T> Matrix<T> &
Matrix<T>::eliminate() {
  const size_t row = this->row_, col = this->col_;
  T **matrix = new T *[row];
  for (size_t i = 0; i != row; ++i)
    matrix[i] = this->value + i * col;
  Matrix<T>::triangularize_(matrix, row, col);
  Matrix<T>::canonicalize_(matrix, row, col);
  T *value = new T[row * col];
  for (size_t r = 0; r != row; ++r)
#if __cpluslus >= 201103L
    std::move(matrix[r], matrix[r] + col, value + r * col);
#else
    std::copy(matrix[r], matrix[r] + col, value + r * col);
#endif
  delete[] matrix, delete[] this->value;
  this->value = value;
  return *this;
}

template <typename T> Matrix<T> &
Matrix<T>::inverse() {
  const size_t row = this->row_, col = this->col_;
  if (row != col)
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

  for (size_t i = 0; i != row; ++i)
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
  const size_t row = this->row_, col = this->col_;
  if (row != col)
    throw std::invalid_argument("Matrix<T>::determinant");
  Matrix copy = *this;
  T **matrix = new T *[row];
  for (size_t i = 0; i != row; ++i)
    matrix[i] = copy.value + i * col;
  T result = Matrix<T>::triangularize_(matrix, row, col) ? 1 : -1;
  for (size_t i = 0; i != row; ++i)
    if (!isZero(result)) result *= matrix[i][i];
  delete[] matrix;
  return isZero(result) ? 0 : result;
}
