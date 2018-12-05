template <typename T, size_t N, size_t M> size_t
largestSubmatrix(const T (&matrix)[N][M], size_t n, size_t m, T color) {
  size_t result = 0, h[N], l[M], r[M];
  fill(h, 1[&h], 0); fill(l, 1[&l], 0); fill(r, 1[&r], m - 1);
  for (size_t i = 0; i != n; ++i) {
    for (size_t j = 0; j != m; ++j)
      if (matrix[i][j] == color) ++h[j]; else h[j] = 0;
    size_t lmost = 0, rmost = m - 1;  // leftmost, rightmost
    for (size_t j = 0; j != m; ++j)
      if (matrix[i][j] == color) l[j] = max(l[j], lmost);
      else l[j] = 0, lmost = j + 1;
    for (size_t j = m - 1; ~j; --j)
      if (matrix[i][j] == color) r[j] = min(r[j], rmost);
      else r[j] = m - 1, rmost = j - 1;
    for (size_t j = 0; j != m; ++j)
      result = max(result, h[j] * (r[j] - l[j] + 1));
  }
  return result;
}
