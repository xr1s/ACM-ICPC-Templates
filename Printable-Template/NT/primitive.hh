long getPrimitiveRoot(long p) {
  vector<long> factor;
  for (long k = 2; k * k < p - 1; ++k)
    if (p % k == 1) factor.insert(factor.end(), {k, p / k});
  for (long k = 2; ; ++k) {
    bool flag = false;
    for (auto fact: factor)
      if ((flag |= modpow(k, fact, p) == 1)) break;
    if (!flag) return k;
  }
}
