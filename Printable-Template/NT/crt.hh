template <typename T> T
crt(const vector<T> &a, const vector<T> &m) {
  if (a.size() != m.size()) throw invalid_argument{"CRT"};
  T M = accumulate(m.begin(), m.end(), 1, multiplies<>{}), r = 0;
  for (size_t k = 0; k != m.size(); ++k) {
    const T Mk = M / m[k], tk = modinv(Mk, m[k]);
    if ((r += a[k] * tk % M * Mk % M) >= M) r -= M;
  }
  return r;
}
