#pragma once

#include <algorithm>
#include <numeric>
#include "Matrix.hh"

// The only use of this Vector is to multiply with Matrix.
// The 3-dimension vector should be a specialization of this class,
// and be defined at ComputationalGeometry (if I write. Well, OK, TODO).
template <typename T, size_t Len = 0> class Vector;
template <typename T, size_t Len> Vector<T, Len> &
operator+=(Vector<T, Len> &, const Vector<T, Len> &);
template <typename T, size_t Len> Vector<T, Len> &
operator-=(Vector<T, Len> &, const Vector<T, Len> &);
template <typename T, size_t Len> T
operator*(const Vector<T, Len> &, const Vector<T, Len> &);

// The fix-sized vector.
// Sometimes I thought variable-sized vector is useless.
template <typename T, size_t Len>
class Vector {
 public:
  Vector();
  Vector(const Vector &);
#if __cplusplus >= 201103L
  template <typename ...Ts>
  Vector(const Ts &...);
#endif  // __cplusplus >= 201103L

  Vector &operator=(const Vector &);
  T &operator[](size_t);
  const T &operator[](size_t) const;

  Vector &copyFrom(const Vector &);
  Vector &copyFromArray(const T (&)[Len]);

  size_t size() const;

  friend Vector &operator+=<>(Vector &, const Vector &);
  friend Vector &operator-=<>(Vector &, const Vector &);
  // Inner product of two vector, see implementation below.
  // The 3-dimension vector should have cross product,
  // should I use * or something else? Missing @ in Python (>=3.5)
  friend T operator*<>(const Vector &, const Vector &);

 private:
   T value[Len];
};

template <typename T, size_t Len>
Vector<T, Len>::Vector() {
  std::fill(this->value, this->value + Len, 0);
}

template <typename T, size_t Len>
Vector<T, Len>::Vector(const Vector &that) {
  this->copyFrom(that);
}

#if __cplusplus >= 201103L
template <typename T, size_t Len> template <typename ...Ts>
Vector<T, Len>::Vector(const Ts &...t)
    : value{static_cast<T>(t)...} {
  static_assert(sizeof...(t) == Len, "Size not match");
}
#endif  // __cplusplus >= 201103L

template <typename T, size_t Len> Vector<T, Len> &
Vector<T, Len>::operator=(const Vector &that) {
  this->copyFrom(that);
}

template <typename T, size_t Len> T &
Vector<T, Len>::operator[](size_t index) {
  return this->value[index];
}

template <typename T, size_t Len> const T &
Vector<T, Len>::operator[](size_t index) const {
  return this->value[index];
}

template <typename T, size_t Len> Vector<T, Len> &
Vector<T, Len>::copyFrom(const Vector &that) {
  return this->copyFromArray(that.value);
}

template <typename T, size_t Len> Vector<T, Len> &
Vector<T, Len>::copyFromArray(const T (&array)[Len]) {
  std::copy(array, array + Len, this->value);
  return *this;
}

template <typename T, size_t Len> size_t
Vector<T, Len>::size() const {
  return Len;
}

template <typename T, size_t Len> Vector<T, Len> &
operator+=(Vector<T, Len> &lhs, const Vector<T, Len> &rhs) {
  using std::transform; using std::plus;
  std::transform(lhs.value, lhs.value + Len,
                 rhs.value, lhs.value, std::plus<T>());
  return lhs;
}

template <typename T, size_t Len> Vector<T, Len>
operator+(const Vector<T, Len> &lhs, const Vector<T, Len> &rhs) {
  Vector<T, Len> copy = lhs;
  return copy += rhs;
}

template <typename T, size_t Len> Vector<T, Len> &
operator-=(Vector<T, Len> &lhs, const Vector<T, Len> &rhs) {
  std::transform(lhs.value, lhs.value + Len,
                 rhs.value, lhs.value, std::minus<T>());
  return lhs;
}

template <typename T, size_t Len> Vector<T, Len>
operator-(const Vector<T, Len> &lhs, const Vector<T, Len> &rhs) {
  Vector<T, Len> copy = lhs;
  return copy -= rhs;
}

template <typename T, size_t Len> T
operator*(const Vector<T, Len> &lhs, const Vector<T, Len> &rhs) {
  return std::inner_product(lhs.value, lhs.value + Len, rhs.value, 0);
}

template <typename T, size_t Len> Vector<T, Len>
operator*(const Vector<T, Len> &lhs, const Matrix<T, Len, Len> &rhs) {
  Vector<T, Len> result;
  for (size_t i = 0; i != Len; ++i)
    for (size_t j = 0; j != Len; ++j)
      result[i] += lhs[j] * rhs[j][i];
  return result;
}

template <typename T, size_t Len> Vector<T, Len> &
operator*=(Vector<T, Len> &lhs, const Matrix<T, Len, Len> &rhs) {
  return lhs = lhs * rhs;
}

template <typename T, size_t Len> Vector<T, Len>
operator*(const Matrix<T, Len, Len> &lhs, const Vector<T, Len> &rhs) {
  Vector<T, Len> result;
  for (size_t i = 0; i != Len; ++i)
    for (size_t j = 0; j != Len; ++j)
      result[i] += lhs[i][j] * rhs[j];
  return result;
}
