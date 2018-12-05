template <typename T> T
euclidean(T n, T m) {
  if (!m || !n) return m | n;
  if (m < 0) m = -m;
  if (n < 0) n = -n;
  while (T r = m % n)
    m = n, n = r;
  return n;
}

template <typename T> T
stein(T n, T m) {
  if (!m || !n) return m | n;
  if (m < 0) m = -m;
  if (n < 0) n = -n;
  int p = 0;
  while (!(m & 1) && !(n & 1))
    m >>= 1, n >>= 1, ++p;
  while (n) {
    while (!(m & 1)) m >>= 1;
    while (!(n & 1)) n >>= 1;
    if (m >= n) swap(m, n);
    n = (n - m) >> 1;
  }
  return m << p;
}
