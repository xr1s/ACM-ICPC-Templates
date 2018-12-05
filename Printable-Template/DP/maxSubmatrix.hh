template <typename T, size_t N, size_t M>
T maxSubmatrix(const T (&matrix)[N][M], size_t n, size_t m) {
  T sum[n], result = 0;
  for (size_t i = 0; i != m; ++i) {
    fill(sum, sum + n, 0);
    for (size_t j = i; j != m; ++j) {
      for (size_t k = 0; k != n; ++k) sum[k] += matrix[j][k];
      result = max(result, maxSubarray(sum, sum + n));
    }
  }
  return result;
}
