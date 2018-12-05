template <typename T> T
lucas(T n, T m, T p) {
  if (n < 0 || n < m) return 0;
  T r = 1;
  do {
    T np = n % p, mp = m % p;
    for (T k = 1; k <= mp; ++k)
      r = r * (np - mp + k) % p * modinv(k, p) % p;
  } while ((n /= p) && (m /= p));
  return r;
}
