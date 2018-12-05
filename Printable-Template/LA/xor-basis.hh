template <size_t N>
struct XorLinearBasis {
  XorLinearBasis(): has0{false} {}
  bool insert(bitset<N> vec) {
    for (size_t i = N - 1; ~i; --i)
      if (vec.test(i))
        if (!this->basis[i].test(i)) {
          for (size_t j = 0; j != i; ++j)
            if (vec.test(j)) vec ^= this->basis[j];
          for (size_t j = i + 1; j != N; ++j)
            if (this->basis[j].test(i)) this->basis[j] ^= vec;
          this->basis[i] = vec;
          this->span.clear();
          return true;
        } else vec ^= this->basis[i];
    this->has0 = true;
    return false;
  }
  bitset<N> maximum() const {
    bitset<N> result;
    for (size_t i = 0; i != N; ++i)
      result ^= this->basis[i];
    return result;
  }
  bitset<N> kth_minimum(size_t k) const {
    if (!this->has0) ++k;
    if (this->span.empty())
      for (size_t i = 0; i != N; ++i)
        if (this->basis[i].test(i))
          this->span.push_back(this->basis[i]);
    if (k >= uintmax_t{1} << this->span.size())
      throw invalid_argument{"XorLinearBasis::kth"};
    bitset<N> result;
    for (size_t i = 0; i != this->span.size(); ++i)
      if (k >> i & 1) result ^= this->span[i];
    return result;
  }
  bitset<N> &operator[](size_t index) {
    return this->basis[index];
  }
  const bitset<N> &operator[](size_t index) const {
    return this->basis[index];
  }
 private:
  bitset<N> basis[N];
  mutable vector<bitset<N>> span;
  mutable bool has0;
};
