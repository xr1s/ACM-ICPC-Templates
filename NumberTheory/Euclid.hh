#include <algorithm>  // std::swap
#include <utility>  // std::pair, std::make_pair

// Stein's Algorithm, find the greatest common divisor (GCD) of m and n.
// benchmark random data 1 time faster than std::gcd (C++17).
// @param m: m above.
// @param n: n above.
// @return: GCD(m, n)
template <typename T>
T gcd(T m, T n) {
  if (!m || !n) return m | n;
  if (m < 0) m = -m;
  if (n < 0) n = -n;
  int p = 0;
  while (!(m & 1) && !(n & 1))
    m >>= 1, n >>= 1, ++p;
  while (n) {
    while (!(m & 1)) m >>= 1;
    while (!(n & 1)) n >>= 1;
    if (m >= n) std::swap(m, n);
    n = (n - m) >> 1;
  }
  return m << p;
}

// As for equation:
//   mx + ny = gcd(m, n)
// @param m: m above.
// @param n: n above.
// @return: r = (gcd(m, n), (x, y))
//          r.first is gcd
//          r.second.first  is x
//          r.second.second is y
// C++98 has no std::tuple, just put up with it.
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

// As for congruence equation
//   mx = gcd(m, n) (mod n)
// @param m: m above.
// @param n: n above.
// @return: x above.
template <typename T>
T modinv(T m, T n) {
  typedef std::pair<T, std::pair<T, T> > triple;
  triple gxy = exgcd(m, n);
  T loop = n / gxy.first;
  T inv = gxy.second.first;
  inv = inv % loop;
  return inv < 0 ? inv + loop : inv;
}
