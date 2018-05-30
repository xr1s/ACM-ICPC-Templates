#pragma once

#include <utility>  // std::pair, std::make_pair
#include <ostream>  // std::ostream

template <typename T>
T gcd(T m, T n) {
  if (!m || !n) return m | n;
  if (m < 0) m = -m;
  if (n < 0) n = -n;
  int p = 0;
  while (!(m & 1) && !(n & 1))
    m >>= 1, n >>= 1, ++p;
  while (n) {
    while (!(m & 1)) m >>= 1;
    while (!(n & 1)) n >>= 1;
    if (m >= n) std::swap(m, n);
    n = (n - m) >> 1;
  }
  return m << p;
}

template <typename T> class Rational;
template <typename T> std::ostream &
operator<<(std::ostream &, const Rational<T> &);
template <typename T> Rational<T> &
operator+=(Rational<T> &, const Rational<T> &);
template <typename T> Rational<T> &
operator-=(Rational<T> &, const Rational<T> &);
template <typename T> Rational<T> &
operator*=(Rational<T> &, const Rational<T> &);
template <typename T> Rational<T> &
operator/=(Rational<T> &, const Rational<T> &);

template <typename T> Rational<T> &
operator+=(Rational<T> &, const T &);
template <typename T> Rational<T> &
operator-=(Rational<T> &, const T &);
template <typename T> Rational<T> &
operator*=(Rational<T> &, const T &);
template <typename T> Rational<T> &
operator/=(Rational<T> &, const T &);

// Rational, represent as p/q, for which p and q are primes or 1.
// Integers are represented as i/1, includes 0 and 1.
// If T is unsigned type, the Rational will be unsigned. (Well, I don't know).
template <typename T>  // To suppoer BigInteger, possibly one day.
class Rational {
 public:
  Rational();
  Rational(const Rational &);
  Rational &operator=(const Rational &);
#if __cplusplus >= 201103L
  Rational(Rational &&);
  Rational &operator=(Rational &&);
#endif
  Rational(const T &);
  Rational &operator=(const T &);
  Rational(const T &, const T &);

  T numerator() const;
  T denominator() const;

  friend std::ostream &operator<<<>(std::ostream &, const Rational &);
  Rational<T> operator+() const;
  Rational<T> operator-() const;
  friend Rational &operator+=<>(Rational &, const Rational &);
  friend Rational &operator-=<>(Rational &, const Rational &);
  friend Rational &operator*=<>(Rational &, const Rational &);
  friend Rational &operator/=<>(Rational &, const Rational &);
  friend Rational &operator+=<>(Rational &, const T &);
  friend Rational &operator-=<>(Rational &, const T &);
  friend Rational &operator*=<>(Rational &, const T &);
  friend Rational &operator/=<>(Rational &, const T &);

  std::pair<T, T> toFraction() const;
  operator float() const;
  operator double() const;
  operator long double() const;

 private:
  T numerator_, denominator_;
  Rational<T> &adjustNegative_();
};

template <typename T>
Rational<T>::Rational()
    : numerator_(0), denominator_(1) {
}

template <typename T>
Rational<T>::Rational(const Rational &that)
    : numerator_(that.numerator_), denominator_(that.denominator_) {
}

template <typename T> Rational<T> &
Rational<T>::operator=(const Rational &that) {
  this->numerator_ = that.numerator_;
  this->denominator_ = that.denominator_;
  return *this;
}

#if __cplusplus >= 201103L
template <typename T>
Rational<T>::Rational(Rational &&that)
    : numerator_{std::move(that.numerator_)}
    , denominator_{std::move(that.denominator_)} {
}

template <typename T> Rational<T> &
Rational<T>::operator=(Rational &&that) {
  this->numerator_ = std::move(that.numerator_);
  this->denominator_ = std::move(that.denominator_);
  return *this;
}
#endif

template <typename T>
Rational<T>::Rational(const T &value)
    : numerator_(value), denominator_(1) {
}

template <typename T> Rational<T> &
Rational<T>::operator=(const T &value) {
  this->numerator_ = value;
  this->denominator_ = 1;
  return *this;
}

template <typename T>
Rational<T>::Rational(const T &numerator, const T &denominator)
    : numerator_(numerator), denominator_(denominator) {
  if (denominator < 0) {
    this->numerator_ = -this->numerator_;
    this->denominator_ = -this->denominator_;
  }
}

template <typename T> T
Rational<T>::numerator() const {
  return this->numerator_;
}

template <typename T> T
Rational<T>::denominator() const {
  return this->denominator_;
}

template <typename T> std::ostream &
operator<<(std::ostream &os, const Rational<T> &self) {
  os << self.numerator_;
  if (self.denominator_ != 1)
    os << '/' << self.denominator_;
  return os;
}

template <typename T> Rational<T>
Rational<T>::operator-() const {
  Rational<T> result = *this;
  result.numerator_ = -result.numerator_;
  return result;
}

template <typename T> Rational<T>
Rational<T>::operator+() const {
  return *this;
}

template <typename T> Rational<T> &
operator+=(Rational<T> &lhs, const Rational<T> &rhs) {
  T divisor = gcd(lhs.denominator_, rhs.denominator_);
  lhs.denominator_ /= divisor;
  lhs.numerator_ *= rhs.denominator_ / divisor;
  lhs.numerator_ += lhs.denominator_ * rhs.numerator_;
  lhs.denominator_ *= rhs.denominator_;
  divisor = gcd(lhs.numerator_, lhs.denominator_);
  lhs.numerator_ /= divisor;
  lhs.denominator_ /= divisor;
  return lhs;
}

template <typename T> Rational<T>
operator+(const Rational<T> &lhs, const Rational<T> &rhs) {
  Rational<T> result = lhs;
  return result += rhs;
}

template <typename T> Rational<T> &
operator+=(Rational<T> &lhs, const T &rhs) {
  lhs.numerator_ += rhs * lhs.denominator_;
  return lhs;
}

template <typename T> Rational<T>
operator+(const Rational<T> &lhs, const T &rhs) {
  Rational<T> result = lhs;
  return result += rhs;
}

template <typename T> Rational<T>
operator+(const T &lhs, const Rational<T> &rhs) {
  Rational<T> result = rhs;
  return result += lhs;
}

template <typename T> Rational<T> &
operator-=(Rational<T> &lhs, const Rational<T> &rhs) {
  T divisor = gcd(lhs.denominator_, rhs.denominator_);
  lhs.denominator_ /= divisor;
  lhs.numerator_ *= rhs.denominator_ / divisor;
  lhs.numerator_ -= lhs.denominator_ * rhs.numerator_;
  lhs.denominator_ *= rhs.denominator_;
  divisor = gcd(lhs.numerator_, lhs.denominator_);
  lhs.numerator_ /= divisor;
  lhs.denominator_ /= divisor;
  return lhs;
}

template <typename T> Rational<T>
operator-(const Rational<T> &lhs, const Rational<T> &rhs) {
  Rational<T> result = lhs;
  return result -= rhs;
}

template <typename T> Rational<T> &
operator-=(Rational<T> &lhs, const T &rhs) {
  lhs.numerator_ -= rhs * lhs.denominator_;
  return lhs;
}

template <typename T> Rational<T>
operator-(const Rational<T> &lhs, const T &rhs) {
  Rational<T> result = lhs;
  return result -= rhs;
}

template <typename T> Rational<T>
operator-(const T &lhs, const Rational<T> &rhs) {
  Rational<T> result = rhs;
  return result -= lhs;
}

template <typename T> Rational<T> &
operator*=(Rational<T> &lhs, const Rational<T> &rhs) {
  T divisor = 1;
  divisor *= gcd(lhs.numerator_, rhs.denominator_);
  divisor *= gcd(lhs.denominator_, rhs.numerator_);
  lhs.numerator_ *= rhs.numerator_;
  lhs.numerator_ /= divisor;
  lhs.denominator_ *= rhs.denominator_;
  lhs.denominator_ /= divisor;
  return lhs.adjustNegative_();
}

template <typename T> Rational<T>
operator*(const Rational<T> &lhs, const Rational<T> &rhs) {
  Rational<T> result = lhs;
  return result *= rhs;
}

template <typename T> Rational<T> &
operator*=(Rational<T> &lhs, const T &rhs) {
  T divisor = gcd(lhs.denominator_, rhs);
  lhs.numerator_ *= rhs / divisor;
  lhs.denominator_ /= divisor;
  return lhs.adjustNegative_();
}

template <typename T> Rational<T>
operator*(const Rational<T> &lhs, const T &rhs) {
  Rational<T> result = lhs;
  return result *= rhs;
}

template <typename T> Rational<T>
operator*(const T &lhs, const Rational<T> &rhs) {
  Rational<T> result = rhs;
  return result *= lhs;
}

template <typename T> Rational<T> &
operator/=(Rational<T> &lhs, const Rational<T> &rhs) {
  T divisor = 1;
  divisor *= gcd(lhs.numerator_, rhs.numerator_);
  divisor *= gcd(lhs.denominator_, rhs.denominator_);
  lhs.numerator_ *= rhs.denominator_;
  lhs.numerator_ /= divisor;
  lhs.denominator_ *= rhs.numerator_;
  lhs.denominator_ /= divisor;
  return lhs.adjustNegative_();
}

template <typename T> Rational<T>
operator/(const Rational<T> &lhs, const Rational<T> &rhs) {
  Rational<T> result = lhs;
  return result /= rhs;
}

template <typename T> Rational<T> &
operator/=(Rational<T> &lhs, const T &rhs) {
  T divisor = gcd(lhs.numerator_, rhs);
  lhs.denominator_ *= rhs / divisor;
  lhs.numerator_ /= divisor;
  return lhs.adjustNegative_();
}

template <typename T> Rational<T>
operator/(const Rational<T> &lhs, const T &rhs) {
  Rational<T> result = lhs;
  return result /= rhs;
}

template <typename T> Rational<T>
operator/(const T &lhs, const Rational<T> &rhs) {
  Rational<T> result = rhs;
  return result /= lhs;
}

template <typename T> std::pair<T, T>
Rational<T>::toFraction() const {
  return std::make_pair(this->numerator_, this->denominator_);
}

template <typename T> bool
operator==(const Rational<T> &lhs, const Rational<T> &rhs) {
  return lhs.numerator() == rhs.numerator() \
      && lhs.denominator() == rhs.denominator();
}

template <typename T> bool
operator==(const Rational<T> &lhs, const T &rhs) {
  return lhs.denominator() == 1 && lhs.numerator() == rhs;
}

template <typename T> bool
operator==(const T &lhs, const Rational<T> &rhs) {
  return rhs.denominator() == 1 && rhs.numerator() == lhs;
}

template <typename T> bool
operator!=(const Rational<T> &lhs, const Rational<T> &rhs) {
  return !(lhs == rhs);
}

template <typename T> bool
operator!=(const Rational<T> &lhs, const T &rhs) {
  return !(lhs == rhs);
}

template <typename T> bool
operator!=(const T &lhs, const Rational<T> &rhs) {
  return !(lhs == rhs);
}

template <typename T> bool
operator<(const Rational<T> &lhs, const Rational<T> &rhs) {
  const T divisor = gcd(lhs.denominator(), rhs.denominator());
  const T l = rhs.denominator() / divisor * lhs.numerator();
  const T r = lhs.denominator() / divisor * rhs.numerator();
  return l < r;
}

template <typename T> bool
operator<(const Rational<T> &lhs, const T &rhs) {
  return lhs.numerator() < rhs * lhs.denominator();
}

template <typename T> bool
operator<(const T &lhs, const Rational<T> &rhs) {
  return lhs * rhs.denominator() < rhs.numerator();
}

template <typename T> bool
operator>=(const Rational<T> &lhs, const Rational<T> &rhs) {
  return !(lhs < rhs);
}

template <typename T> bool
operator>=(const Rational<T> &lhs, const T &rhs) {
  return !(lhs < rhs);
}

template <typename T> bool
operator>=(const T &lhs, const Rational<T> &rhs) {
  return !(lhs < rhs);
}

template <typename T> bool
operator>(const Rational<T> &lhs, const Rational<T> &rhs) {
  return rhs < lhs;
}

template <typename T> bool
operator>(const Rational<T> &lhs, const T &rhs) {
  return rhs < lhs;
}

template <typename T> bool
operator>(const T &lhs, const Rational<T> &rhs) {
  return rhs < lhs;
}

template <typename T> bool
operator<=(const Rational<T> &lhs, const Rational<T> &rhs) {
  return !(lhs > rhs);
}

template <typename T> bool
operator<=(const Rational<T> &lhs, const T &rhs) {
  return !(lhs > rhs);
}

template <typename T> bool
operator<=(const T &lhs, const Rational<T> &rhs) {
  return !(lhs > rhs);
}

template <typename T>
Rational<T>::operator float() const {
  return static_cast<float>(this->numerator_) / this->denominator_;
}

template <typename T>
Rational<T>::operator double() const {
  return static_cast<double>(this->numerator_) / this->denominator_;
}

template <typename T>
Rational<T>::operator long double() const {
  return static_cast<long double>(this->numerator_) / this->denominator_;
}

template <typename T> Rational<T> &
Rational<T>::adjustNegative_() {
  if (this->denominator_ < 0) {
    this->numerator_ = -this->numerator_;
    this->denominator_ = -this->denominator_;
  }
  return *this;
}
