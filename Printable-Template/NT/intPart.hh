template <typename RandomAccessIt> void
initIntPart(RandomAccessIt p, size_t n) {
  using T = typename iterator_traits<RandomAccessIt>::value_type;
  vector<T> q = {0, 1, 2, 5};
  while (q.back() < n)
    q.push_back(3 + q.end()[-2] * 2 - q.end()[-4]);
  *p = 1; fill(p + 1, p + n, 0);
  for (size_t i = 1; i != n; ++i)
    for (size_t j = 1; q[j] <= i; ++j)
      p[i] += (j + 1 & 2 ? p[i - q[j]] : -p[i - q[j]]);
}
