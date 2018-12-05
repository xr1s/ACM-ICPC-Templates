template <typename T> tuple<T, T, T>
exgcd(T n, T m) {
  T x = 1, y = 0, xp = 0, yp = 1;
  for (T r = m % n, q = m / n; r; ) {
    xp = exchange(x, xp - q * x);
    yp = exchange(y, yp - q * y);
    m = n, n = r, r = m % n, q = m / n;
  }
  return make_tuple(n, x, y);
}
