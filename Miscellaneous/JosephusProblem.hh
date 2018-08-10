/**
 * There are n people, suicide every m count, return the k-th death's id.
 * counting starts from 0, both id and k-th.
 * Complexity: O(log n).
 */
int josephus(int n, int m, int k) {
  if (m == 1) return k;
  for (k = k * m + m - 1; k >= n; k -= n - (k - n) / (m - 1));
  return k;
}
