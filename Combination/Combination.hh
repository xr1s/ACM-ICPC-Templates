/*
 * binomial(m, n) = m! / (n! * (m - n)!)
 * After preprocessing and storing n!, inv(n!).
 * We can have binomial(m, n) in O(1).
 */

template <typename T>
T binomial(T m, T n) {
  if (m < 0 || n < 0) return 0;
  T r = 1;
  if (n * 2 > m) n = m - n;
  for (int i = 1; i <= n; ++i)
    r = r * (m - n + i) / i;
  return r;
}

#include <utility>

template <typename T>
std::pair<T, std::pair<T, T> > exgcd(T m, T n) {
  using std::make_pair;
  T x = 0, y = 1, xp = 1, yp = 0, xt, yt;
  for (T r = m % n, q = m / n; r; ) {
    xt = xp - q * x, xp = x, x = xt;
    yt = yp - q * y, yp = y, y = yt;
    m = n, n = r, r = m % n, q = m / n;
  }
  return make_pair(n, make_pair(x, y));
}

template <typename T>
T modinv(T m, T n) {
  std::pair<T, std::pair<T, T> > gxy = exgcd(m, n);
  T loop = n / gxy.first;
  T inv = gxy.second.first;
  inv = inv % loop;
  return inv < 0 ? inv + loop : inv;
}

template <typename T>
T binomial(T m, T n, T p) {
  if (m < 0 || n < 0) return 0;
  T r = 1;
  do {
    T mp = m % p, np = n % p;
    if (mp < np) return 0;
    if (np * 2 > mp) np = mp - np;
    for (T i = 1; i <= np; ++i)
      r = r * (mp - np + i) % p * modinv(i, p) % p;
  } while ((m /= p) && (n /= p));
  return r;
}

#include <vector>

template <typename T>
std::vector<std::vector<T> > initializeBinomial(size_t m) {
  std::vector<std::vector<T> > r(m);
  for (size_t i = 0; i != m; ++i) {
    r[i].resize(i + 1);
    r[i][0] = r[i][i] = 1;
    for (size_t j = 1; j < i; ++j)
      r[i][j] = r[i - 1][j] + r[i - 1][j - 1];
  }
  return r;
}
