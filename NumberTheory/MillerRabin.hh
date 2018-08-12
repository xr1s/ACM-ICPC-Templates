#include <stdint.h>

template <typename B, typename E>
B modpow(B base, E expo, B mod) {
  B retn = 1;
  do {
    if (expo & 1) retn = retn * base % mod;
    if (expo ^ 1) base = base * base % mod;
  } while (expo >>= 1);
  return retn;
}

bool isPrime(uintmax_t n) {
  static const uintmax_t p[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41,
  };
  if (n < 2) return false;
  uintmax_t d = n - 1, s = 0;
  for (; ~d & 1; d >>= 1) ++s;
  for (uintmax_t i = 0; i != sizeof p / sizeof *p; ++i) {
    if (n == p[i]) return true;
    uintmax_t a = modpow(p[i], d, n);
    if (a != 1) {
      uintmax_t r = 0;
      for (; r != s; ++r) {
        if (a == n - 1) break;
        a = a * a % n;
      }
      if (r == s) return false;
    }
  }
  return true;
}
