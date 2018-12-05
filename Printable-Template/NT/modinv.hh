template <typename T> T
modinv(T n, T m) {
  T x = get<1>(exgcd(n, m));
  return x < 0 ? x + m : x;
}
