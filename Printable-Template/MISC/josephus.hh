int josephus(int n, int m, int k) {
  for (k = k * m + m - 1; k >= n; k -= n - (k - n) / (m - 1));
  return k;
}
