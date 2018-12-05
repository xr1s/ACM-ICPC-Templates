template <typename T> T
binomial(T n, T m) {
  if (m < 0 || m > n) return 0;
  if (2 * m > n) m = n - m;
  T r = 1;
  for (T k = 1; k <= m; ++k)
    r = r * (n - m + k) / k;
  return r;
}
