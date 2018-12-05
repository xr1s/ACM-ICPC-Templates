template <typename T> T
exbsgs(T g, T y, T n) {
  T sum = 0;
  for (T gcd; (gcd = stein(g, n)) != 1; ++sum) {
    if (y % gcd != 0) return -1;
    n = n / gcd, y = y / gcd * modinv(g / gcd, n) % n;
  }
  T x = bsgs(g, y, n);
  return x == -1 ? -1 : x + sum;
}
