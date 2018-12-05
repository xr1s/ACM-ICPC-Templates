template <typename T> vector<Rational<T>>
farey(T n) {
  vector<Rational<T>> r = {0, {1, n}};
  while (r.back().denominator() != 1) {
    const T k = (n + r.rbegin()[1].denominator()) / r.back().denominator();
    const T p = k * r.back().numerator() - r.rbegin()[1].numerator();
    const T q = k * r.back().denominator() - r.rbegin()[1].denominator();
    r.emplace_back(p, q);
  }
  return r;
}
