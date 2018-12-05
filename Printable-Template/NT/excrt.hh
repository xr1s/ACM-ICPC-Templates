template <typename T> T
excrt(const vector<T> &a, const vector<T> &m){
  T M = m.front(), A = a.front();
  for(size_t k = 1; k != a.size(); ++k) {
    T diff = a[k] - A, g, x;
    tie(g, x, ignore) = exgcd(M, m[k]);
    if (diff % g != 0) return -1;
    T t = m[k] / g, x0 = diff / g * x % t;
    if (x0 < 0) x0 += t;
    A += M * x0;
    M = m[k] / g * M;
  }
  return A;
}
