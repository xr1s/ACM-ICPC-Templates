bool isPrime(uintmax_t n) {
  static const uintmax_t p[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
  if (n < 2) return false;  // {2,3,7,11,13} ensures i32.
  uintmax_t d = n - 1, s = 0;
  for (; ~d & 1; d >>= 1) ++s;
  for (uintmax_t i = 0, a, r; i != sizeof p / sizeof *p; ++i) {
    if (n == p[i]) return true;
    if ((a = modpow(p[i], d, n)) != 1) {
      for (r = 0; r != s && a != n - 1; ++r)
        a = a * a % n;
      if (r == s) return false;
    }
  }
  return true;
}
