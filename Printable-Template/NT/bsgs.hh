template <typename T> T
bsgs(T g, T y, T p) {
  T m = ceil(sqrt(p - 1));
  unordered_map<T, T> s;
  T bs = 1, gs = modinv(modpow(g, m, p));
  for (T j = 0; j != m; ++j)
    s.emplace(bs, j), bs = bs * g % p;
  for (T i = 0; i != m; ++i) {
    auto it = s.find(y);
    if (it != s.end())
      return i * m + it->second;
    y = y * gs % p;
  }
  return -1;
}
