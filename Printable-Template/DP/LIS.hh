template <typename BidirIt, typename Compare>
auto LIS(BidirIt s, BidirIt t, Compare comp) {
  using T = typename iterator_traits<BidirIt>::value_type;
  T *lis = new T[distance(s, t)], *r;
  size_t len = 0;
  for (BidirIt it = s; it != t; *r = *it++) {
    r = lower_bound(lis, lis + len, *it, comp);
    if (r == lis + len) ++len;
  }
  // Here, the value of `len' is the length of LIS.
  for (r = lis + len - 1; *--t != *r; );
  vector<T> result = {*t}; result.reserve(len);
  for (; r-- != lis; result.push_back(*t))
    while (comp(*--t, *r) && *t != *r || !comp(*t, *(r + 1)));
  reverse(result.begin(), result.end());
  delete[] lis;
  return result;
}
