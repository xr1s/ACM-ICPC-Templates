#pragma once

#include <algorithm>  // std::abs

template <typename T>
inline bool isZero(T value) {
  return value == 0;
}

template <>
inline bool isZero<float>(float value) {
  return std::abs(value) <= 1e-4;
}

template <>
inline bool isZero<double>(double value) {
  return std::abs(value) <= 1e-8;
}

template <>
inline bool isZero<long double>(long double value) {
  return std::abs(value) <= 1e-10;
}
