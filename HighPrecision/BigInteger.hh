#pragma once

#include <iostream>
// cstdint is a C++11 header. Weired.
#include <stdint.h>
#include <vector>

// Arbitrary precision integer class.
// Each cell of std::vector save 32 bits.
class BigInteger {
  // Only std::basic_{i,o}stream<char, std::char_traits<char> >
  // (a.k.a. std::istream, std::ostream) are useful in ACM-ICPC.
  friend std::ostream & operator<<(std::ostream &, const BigInteger &);
  friend std::istream &operator>>(std::istream &, BigInteger &);
 private:
  std::vector<uint32_t> value;
  bool negative;
  const static uint64_t base;

  // Time complexity of converting between bases is O(log²n).
  void getDec_(std::istream &);
  void putDec_(std::ostream &) const;
  void getHex_(std::istream &);
  void putHex_(std::ostream &) const;
  void getOct_(std::istream &);
  void putOct_(std::ostream &) const;
  void getBin_(std::istream &);
  void putBin_(std::ostream &) const;

};
const uint64_t BigInteger::base = static_cast<uint64_t>(1) << 32;

std::istream &operator>>(std::istream &is, BigInteger &self) {
  char cursor;
  self.negative = false;
  // Discards spaces.
  is >> cursor;
  // Negative or positive sign.
  if (cursor == '-' || cursor == '+') {
    self.negative = cursor == '-';
    cursor = is.get();
  }
  is.unget();
  switch (is.flags() & std::ios::basefield) {
   case std::ios::hex:
    // Not implemented.
    break;
   case std::ios::dec:
    self.getDec_(is);
    break;
   case std::ios::oct:
    // Not implemented.
    break;
   default:  // case 0:
    // Not implemented.
    break;
  }
  return is;
}

std::ostream &operator<<(std::ostream &os, const BigInteger &self) {
  if (self.negative) os.put('-');
  switch (os.flags() & std::ios::basefield) {
   case std::ios::hex:
    // Not implemented.
    break;
   case std::ios::dec:
    self.putDec_(os);
    break;
   case std::ios::oct:
    // Not implemented.
    break;
   default:  // case 0:
    // Not implemented.
    break;
  }
  return os;
}

void BigInteger::getDec_(std::istream &is) {
  typedef std::vector<uint32_t>::iterator Iter;
  typedef std::vector<uint32_t>::reverse_iterator RevIter;
  const static uint_fast32_t logBase = 9, pow10[] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000,
  };
  this->value.clear();
  if (!isdigit(is.peek())) {
    this->negative = false;
    this->value.push_back(0);
    is.clear(std::ios::failbit);
    return;
  }
  std::vector<uint32_t> decimal;
  // Discard leading zeros.
  while (is.peek() == '0') is.get();
  // Read from most significant digit.
  uint64_t value = 0, logLen = 0;
  while (isdigit(is.peek())) {
    int cursor = is.get();
    value = value * 10 + cursor - '0';
    if (++logLen == logBase) {
      decimal.push_back(value);
      value = 0; logLen = 0;
    }
  }
  // Save in least significant digit first, adjust.
  if (logLen != 0) {
    decimal.push_back(value);
    const uint_fast32_t shift = pow10[logBase - logLen];
    const uint_fast32_t len = pow10[logLen];
    RevIter next = decimal.rbegin(), prev = next++;
    while (next != decimal.rend()) {
      *prev++ += *next % shift * len;
      *next++ /= shift;
    }
  }
  if (decimal.empty()) {
    this->negative = false;
    this->value.push_back(0);
    return;
  }
  this->value.reserve(decimal.size());
  // Convert from base-1000000000 to base-4294967294.
  Iter head = decimal.begin();
  while (head + 1 != decimal.end()) {
    value = *head++;
    for (Iter i = head; i != decimal.end(); ++i) {
      value = value * pow10[logBase] + *i;
      *i = value / BigInteger::base;
      value %= BigInteger::base;
    }
    this->value.push_back(value);
  }
  if (*head) this->value.push_back(*head);
}

void BigInteger::putDec_(std::ostream &os) const {
  typedef std::vector<uint32_t>::iterator Iter;
  typedef std::vector<uint32_t>::reverse_iterator RevIter;
  const static uint_fast32_t logBase = 9, pow10[] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000,
  };
  if (this->value.size() == 1 && this->value.front() == 0) {
    os.put('0');
    return;
  }
  // Convert from base-4294967294 to base-1000000000.
  std::vector<uint32_t> binary = this->value, decimal;
  RevIter head = binary.rbegin();
  while (head + 1 != binary.rend()) {
    uint64_t value = 0;
    for (RevIter i = head++; i != binary.rend(); ++i) {
      value = value * BigInteger::base | *i;
      *i = value / pow10[logBase];
      value %= pow10[logBase];
    }
    decimal.push_back(value);
  }
  if (*head) decimal.push_back(*head);
  os << decimal.back();
  decimal.pop_back();
  for (RevIter i = decimal.rbegin(); i != decimal.rend(); ++i) {
    for (int_fast8_t j = logBase - 1; j != 0; --j)
      if (*i < pow10[j]) os.put('0'); else break;
    os << *i;
  }
}
