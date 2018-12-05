template <typename T> vector<vector<T>>
initBinomial(size_t n) {
  vector<vector<T>> binom{n};
  for (size_t i = 0; i != n; ++i) {
    binom[i].resize(i + 1);
    binom[i][0] = binom[i][i] = 1;
    for (size_t j = 1; j != i; ++j)
      binom[i][j] = binom[i - 1][j] + binom[i - 1][j - 1];
  }
  return binom;
}
