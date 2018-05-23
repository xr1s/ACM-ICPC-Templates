#pragma once

#include "Matrix.hh"

// The size of Static matrix is fixed at runtime.
template <typename T, size_t Row, size_t Col>
class Matrix {
 public:
  Matrix();
  Matrix(const Matrix &);
#if __cplusplus >= 201103L
  template <typename ...Ta>
  Matrix(const Ta (&...arr)[Col]);
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
template <typename T, size_t Row, size_t Col> template <typename ...Ta>
Matrix<T, Row, Col>::Matrix(const Ta (&...arr)[Col])
    : row{Row}, col{Col} {
  static_assert(sizeof...(arr) == Row, "Size not match");
  const T *array[Col] = { arr... };
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
  Matrix<T>::triangularize_(matrix, Row, Col);
  Matrix<T>::canonicalize_(matrix, Row, Col);
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
  Matrix<T>::triangularize_(matrix, Row, Col);
  Matrix<T>::canonicalize_(matrix, Row, Col);
  for (size_t r = 0; r != Row; ++r)
#if __cplusplus >= 201103L
    std::move(matrix[r], matrix[r] + Col, this->value[r]);
#else
    std::copy(matrix[r], matrix[r] + Col, this->value[r]);
#endif
  return *this;
}
