#pragma once

#include <stdexcept>
#include "MatrixDecl.hh"

template <typename T> Matrix<T> &
operator+=(Matrix<T> &, const Matrix<T> &);
template <typename T> Matrix<T> &
operator-=(Matrix<T> &, const Matrix<T> &);
template <typename T> Matrix<T> &
operator*=(Matrix<T> &, const Matrix<T> &);
template <typename T> bool
operator==(const Matrix<T> &, const Matrix<T> &);
template <typename T> bool
operator!=(const Matrix<T> &, const Matrix<T> &);

// Matrix<T, 0, 0> is a variable-sized matrix,
// which has the ability to change size at runtime.
// Some functions will throw std::invalid_argument
// if the column and row of argument(s) do not satisfy the requirement.
// For example, the operator+(A, B) requires A and B are of the same size.
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
  template <typename Head, typename ...Tail, size_t Col>
  Matrix(const Head (&)[Col], const Tail (&...tail)[Col]);
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
  friend bool operator==<>(const Matrix &, const Matrix &);
  friend bool operator!=<>(const Matrix &, const Matrix &);

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
};

template <typename T>
Matrix<T>::Matrix()
    : row_(0), col_(0), value(NULL) {
}

template <typename T>
Matrix<T>::Matrix(const Matrix &that)
    : row_(0), col_(0), value(NULL) {
  this->copyFrom(that);
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix &that) {
  this->copyFrom(that);
  return *this;
}

#if __cplusplus >= 201103L
template <typename T>
Matrix<T>::Matrix(Matrix &&that)
    : value{nullptr} {
  this->moveFrom(that);
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(Matrix &&that) {
  this->moveFrom(that);
  return *this;
}
#endif  // __cplusplus >= 201103L

template <typename T>
Matrix<T>::~Matrix() {
  delete[] this->value;
  this->value = NULL;
}

template <typename T>
Matrix<T>::Matrix(size_t row, size_t col)
    : row_(row), col_(col) {
  this->value = new T[row * col];
}

#if __cplusplus >= 201103L
template <typename T> template <typename Head, typename ...Tail, size_t Col>
Matrix<T>::Matrix(const Head (&head)[Col], const Tail (&...tail)[Col])
    : row_{sizeof...(tail) + 1}, col_{Col} {
  const Head *array[sizeof...(tail) + 1] = { head, tail... };
  this->value = new T[this->row_ * this->col_];
  for (size_t r = 0; r != this->row_; ++r)
    std::copy(array[r], array[r] + Col, this->value + r * Col);
}
#endif  // __cplusplus >= 201103L

template <typename T> Matrix<T> &
Matrix<T>::copyFrom(const Matrix &that) {
  const size_t new_size = that.row_ * that.col_;
  const size_t old_size = this->row_ * this->col_;
  this->row_ = that.row_;
  this->col_ = that.col_;
  if (new_size != old_size) {
    delete[] this->value;
    this->value = new T[new_size];
  }
  std::copy(that.value, that.value + new_size, this->value);
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
  std::copy(*arr, *arr + size, this->value);
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
  std::transform(lhs.value, lhs.value + size,
                 rhs.value, lhs.value, std::plus<T>());
  return lhs;
}

template <typename T> Matrix<T>
operator+(const Matrix<T> &lhs, const Matrix<T> &rhs) {
  if (lhs.row() != rhs.row() || lhs.col() != rhs.col())
    throw std::invalid_argument("Matrix<T>::operator+");
  Matrix<T> result = lhs;
  return result += rhs;
}

template <typename T> Matrix<T> &
operator-=(Matrix<T> &lhs, const Matrix<T> &rhs) {
  if (lhs.row_ != rhs.row_ || lhs.col_ != rhs.col_)
    throw std::invalid_argument("Matrix<T>::operator-=");
  const size_t size = lhs.row_ * lhs.col_;
  std::transform(lhs.value, lhs.value + size,
                 rhs.value, lhs.value, std::minus<T>());
  return lhs;
}

template <typename T> Matrix<T>
operator-(const Matrix<T> &lhs, const Matrix<T> &rhs) {
  if (lhs.row() != rhs.row() || lhs.col() != rhs.col())
    throw std::invalid_argument("Matrix<T>::operator-");
  Matrix<T> result = lhs;
  return result -= rhs;
}

template <typename T> Matrix<T> &
operator*=(Matrix<T> &lhs, const Matrix<T> &rhs) {
  if (lhs.col_ != rhs.row_)
    throw std::invalid_argument("Matrix<T>::operator*=");
  Matrix<T> result(lhs.row_, rhs.col_);
  std::fill(result.value, result.value + result.row_ * result.col_, 0);
  for (size_t r = 0; r != result.row_; ++r)
    for (size_t c = 0; c != result.col_; ++c)
      for (size_t i = 0; i != lhs.col_; ++i)
        result[r][c] += lhs[r][i] * rhs[i][c];
  return lhs.moveFrom(result);
}

template <typename T> Matrix<T>
operator*(const Matrix<T> &lhs, const Matrix<T> &rhs) {
  if (lhs.col() != rhs.row())
    throw std::invalid_argument("Matrix<T>::operator*");
  Matrix<T> result = lhs;
  return result *= rhs;
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
  triangularize_(matrix, row, col);
  T *value = new T[row * col];
  for (size_t r = 0; r != row; ++r)
    ifCpp11MoveElseCopy(matrix[r], matrix[r] + col, value + r * col);
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
  triangularize_(matrix, row, col);
  canonicalize_(matrix, row, col);
  T *value = new T[row * col];
  for (size_t r = 0; r != row; ++r)
    ifCpp11MoveElseCopy(matrix[r], matrix[r] + col, value + r * col);
  delete[] matrix, delete[] this->value;
  this->value = value;
  return *this;
}

template <typename T> Matrix<T> &
Matrix<T>::inverse() {
  const size_t row = this->row_, col = this->col_;
  T *const value = this->value;
  if (row != col)
    throw std::invalid_argument("Matrix<T>::inverse");

  T *concat = new T[row * col * 2];
  T **matrix = new T *[row];

  for (size_t i = 0; i != row; ++i) {
    matrix[i] = concat + i * col * 2;
    ifCpp11MoveElseCopy(value + i * col, value + i * col + col, matrix[i]);
    std::fill(matrix[i] + col, matrix[i] + col * 2, 0);
    matrix[i][col + i] = 1;
  }
  triangularize_(matrix, row, col * 2);

  for (size_t i = 0; i != row; ++i)
    if (isZero(matrix[i][i])) {
      delete[] matrix, delete[] concat;
      throw std::invalid_argument("Matrix::inverse");
    }
  canonicalize_(matrix, row, col * 2);
  for (size_t i = 0; i != row; ++i)
    ifCpp11MoveElseCopy(matrix[i] + col, matrix[i] + col * 2, value + i * col);

  delete[] matrix, delete[] concat;
  return *this;
}

template <typename T> T
Matrix<T>::determinant() const {
  const size_t row = this->row_, col = this->col_;
  if (row != col)
    throw std::invalid_argument("Matrix<T>::determinant");
  Matrix temporary = *this;
  T **matrix = new T *[row];
  for (size_t i = 0; i != row; ++i)
    matrix[i] = temporary.value + i * col;
  T result = triangularize_(matrix, row, col) ? 1 : -1;
  for (size_t i = 0; i != row; ++i)
    if (!isZero(result)) result *= matrix[i][i];
  delete[] matrix;
  return isZero(result) ? 0 : result;
}

template <typename T> bool
operator==(const Matrix<T> &lhs, const Matrix<T> &rhs) {
  if (lhs.row_ != rhs.row_) return false;
  if (lhs.col_ != rhs.col_) return false;
  return std::equal(lhs.value, lhs.value + lhs.row_ * lhs.col_, rhs.value);
}
