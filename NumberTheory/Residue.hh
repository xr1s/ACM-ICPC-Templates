#pragma once

#include "Euclid.hh"
#include <iostream>  // std::istream, std::ostream

template <typename T, T = 0> class Residue;

template <typename T, T Mod>
std::ostream &operator<<(std::ostream &, const Residue<T, Mod> &);
template <typename T, T Mod>
std::istream &operator>>(std::istream &, Residue<T, Mod> &);

template <typename T, T Mod> Residue<T, Mod> &
operator+=(Residue<T, Mod> &, const Residue<T, Mod> &);
template <typename T, T Mod> Residue<T, Mod> &
operator-=(Residue<T, Mod> &, const Residue<T, Mod> &);
template <typename T, T Mod> Residue<T, Mod> &
operator*=(Residue<T, Mod> &, const Residue<T, Mod> &);
template <typename T, T Mod> Residue<T, Mod> &
operator/=(Residue<T, Mod> &, const Residue<T, Mod> &);
template <typename T, T Mod> bool
operator==(const Residue<T, Mod> &, const Residue<T, Mod> &);

template <typename T, T Mod>
class Residue {
 public:
  Residue();
  Residue(const Residue &);
  Residue(const T &);

  friend std::ostream &operator<<<>(std::ostream &, const Residue &);
  friend std::istream &operator>><>(std::istream &, Residue &);
  friend Residue &operator+=<>(Residue &, const Residue &);
  friend Residue &operator-=<>(Residue &, const Residue &);
  friend Residue &operator*=<>(Residue &, const Residue &);
  friend Residue &operator/=<>(Residue &, const Residue &);
  friend bool operator==<>(const Residue &, const Residue &);

 private:
  T value;
};

template <typename T, T Mod>
Residue<T, Mod>::Residue()
    : value(0) {
}

template <typename T, T Mod>
Residue<T, Mod>::Residue(const T &value)
    : value(value % Mod) {
  if (this->value < 0)
    this->value += Mod;
}

template <typename T, T Mod>
Residue<T, Mod>::Residue(const Residue &that)
    : value(that.value) {
}

template <typename T, T Mod> std::ostream &
operator<<(std::ostream &os, const Residue<T, Mod> &self) {
  return os << self.value;
}

template <typename T, T Mod> std::istream &
operator>>(std::istream &is, Residue<T, Mod> &self) {
  is >> self.value;
  self.value %= Mod;
  if (self.value < 0)
    self.value += Mod;
  return is;
}

template <typename T, T Mod> Residue<T, Mod> &
operator+=(Residue<T, Mod> &lhs, const Residue<T, Mod> &rhs) {
  if ((lhs.value += rhs.value) >= Mod)
    lhs.value -= Mod;
  return lhs;
}

template <typename T, T Mod> Residue<T, Mod> &
operator+=(Residue<T, Mod> &lhs, const T &rhs) {
  return lhs += Residue<T, Mod>(rhs);
}

template <typename T, T Mod> Residue<T, Mod>
operator+(const Residue<T, Mod> &lhs, const Residue<T, Mod> &rhs) {
  Residue<T, Mod> copy = lhs;
  return copy += rhs;
}

template <typename T, T Mod> Residue<T, Mod>
operator+(const Residue<T, Mod> &lhs, const T &rhs) {
  return lhs + Residue<T, Mod>(rhs);
}

template <typename T, T Mod> Residue<T, Mod>
operator+(const T &lhs, const Residue<T, Mod> &rhs) {
  return Residue<T, Mod>(lhs) + rhs;
}

template <typename T, T Mod> Residue<T, Mod> &
operator-=(Residue<T, Mod> &lhs, const Residue<T, Mod> &rhs) {
  if ((lhs.value -= rhs.value) < 0)
    lhs.value += Mod;
  return lhs;
}

template <typename T, T Mod> Residue<T, Mod> &
operator-=(Residue<T, Mod> &lhs, const T &rhs) {
  return lhs -= Residue<T, Mod>(rhs);
}

template <typename T, T Mod> Residue<T, Mod>
operator-(const Residue<T, Mod> &lhs, const Residue<T, Mod> &rhs) {
  Residue<T, Mod> copy = lhs;
  return copy -= rhs;
}

template <typename T, T Mod> Residue<T, Mod>
operator-(const Residue<T, Mod> &lhs, const T &rhs) {
  return lhs - Residue<T, Mod>(rhs);
}

template <typename T, T Mod> Residue<T, Mod>
operator-(const T &lhs, const Residue<T, Mod> &rhs) {
  return Residue<T, Mod>(lhs) - rhs;
}

template <typename T, T Mod> Residue<T, Mod> &
operator*=(Residue<T, Mod> &lhs, const Residue<T, Mod> &rhs) {
  lhs.value *= rhs.value;
  lhs.value %= Mod;
  return lhs;
}

template <typename T, T Mod> Residue<T, Mod> &
operator*=(Residue<T, Mod> &lhs, const T &rhs) {
  return lhs *= Residue<T, Mod>(rhs);
}

template <typename T, T Mod> Residue<T, Mod>
operator*(const Residue<T, Mod> &lhs, const Residue<T, Mod> &rhs) {
  Residue<T, Mod> copy = lhs;
  return copy *= rhs;
}

template <typename T, T Mod> Residue<T, Mod>
operator*(const Residue<T, Mod> &lhs, const T &rhs) {
  return lhs * Residue<T, Mod>(rhs);
}

template <typename T, T Mod> Residue<T, Mod>
operator*(const T &lhs, const Residue<T, Mod> &rhs) {
  return Residue<T, Mod>(lhs) * rhs;
}

template <typename T, T Mod> Residue<T, Mod> &
operator/=(Residue<T, Mod> &lhs, const Residue<T, Mod> &rhs) {
  lhs.value *= modinv(rhs.value, Mod);
  lhs.value %= Mod;
  return lhs;
}

template <typename T, T Mod> Residue<T, Mod> &
operator/=(Residue<T, Mod> &lhs, const T &rhs) {
  return lhs /= Residue<T, Mod>(rhs);
}

template <typename T, T Mod> Residue<T, Mod>
operator/(const Residue<T, Mod> &lhs, const Residue<T, Mod> &rhs) {
  Residue<T, Mod> copy = lhs;
  return copy /= rhs;
}

template <typename T, T Mod> Residue<T, Mod>
operator/(const Residue<T, Mod> &lhs, const T &rhs) {
  return lhs / Residue<T, Mod>(rhs);
}

template <typename T, T Mod> Residue<T, Mod>
operator/(const T &lhs, const Residue<T, Mod> &rhs) {
  return Residue<T, Mod>(lhs) / rhs;
}

template <typename T, T Mod> bool
operator==(const Residue<T, Mod> &lhs, const Residue<T, Mod> &rhs) {
  return lhs.value == rhs.value;
}

template <typename T, T Mod> bool
operator==(const Residue<T, Mod> &lhs, const T &rhs) {
  return lhs == Residue<T, Mod>(rhs);
}

template <typename T, T Mod> bool
operator==(const T &lhs, const Residue<T, Mod> &rhs) {
  return Residue<T, Mod>(lhs) == rhs;
}

template <typename T, T Mod> bool
operator!=(const Residue<T, Mod> &lhs, const Residue<T, Mod> &rhs) {
  return !(lhs == rhs);
}

template <typename T, T Mod> bool
operator!=(const Residue<T, Mod> &lhs, const T &rhs) {
  return !(lhs == Residue<T>(rhs));
}

template <typename T, T Mod> bool
operator!=(const T &lhs, const  Residue<T, Mod> &rhs) {
  return !(Residue<T>(lhs) == rhs);
}
