template <typename T>
struct SegmentTree {
  SegmentTree(size_t n): size{n}
      , tree{n ? new T[boundary(n)]{} : nullptr}
      , lazy{n ? new T[boundary(n)]{} : nullptr} {
  }
  // Public proxy functions (range ctor, query, update) are omitted.
 private:
  size_t size; T *tree, *lazy;
  template <typename ForwardIterator> void
    build_internal(FowardIterator &p, size_t n, size_t l, size_t r) {
    if (l + 1 == r) return (void)(this->tree[n] = *p++);
    const size_t m = l + (r - l) / 2;
    build_internal(p, n * 2 + 0, l, m); build_internal(p, n * 2 + 1, m, r);
    this->tree[n] = this->tree[n * 2] + this->tree[n * 2 + 1];
  }
  T query_internal(size_t lq, size_t rq,
                   size_t n, size_t l, size_t r) {
    if (l + 1 == r) return this->tree[n];
    const size_t m = l + (r - l) / 2; T result = {0};
    this->push_down(n, l, m, r);
    if (lq < m) result = result + query_internal(lq, rq, n * 2 + 0, l, m);
    if (rq > m) result = result + query_internal(lq, rq, n * 2 + 1, m, r);
    return result;
  }
  void update_internal(size_t lq, size_t rq, const T &value,
                       size_t n, size_t l, size_t r) {
    if (l + 1 == r) {
      this->tree[n] = this->tree[n] + value;
      this->lazy[n] = this->lazy[n] + value;
      return;
    }
    const size_t m = l + (r - l) / 2;
    this->push_down(n, l, m, r);
    if (lq < m) update_internal(lq, rq, value, n * 2 + 0, l, m);
    if (rq > m) update_internal(lq, rq, value, n * 2 + 1, m, r);
    this->tree[n] = this->tree[n * 2] + this->tree[n * 2 + 1];
  }
  void push_down(size_t n, size_t l, size_t m, size_t r) {
    if (!this->lazy[n]) return;
    const size_t lc = n * 2 + 0, rc = n * 2 + 1;
    this->tree[lc] = this->tree[lc] + this->lazy[n] * (m - l);
    this->tree[rc] = this->tree[lc] + this->lazy[n] * (r - m);
    this->lazy[lc] = this->lazy[lc] + this->lazy[n];
    this->lazy[rc] = this->lazy[lc] + this->lazy[n];
  }
  static size_t boundary(size_t n) {
    if (n == 0 || n == 1) return n;
    return 2ULL << 64 - __builtin_clzll(n - 1);
  }
};
