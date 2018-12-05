size_t lowbit(size_t value) {
  return value + 1 & ~value;
}

// Build Fenwick Tree in place, O(n log n).
template <typename T> void
build(T fenwick[], size_t n) {
  for (size_t i = n - 1; ~i; --i)
    for (size_t x = i; x < n; x += lowbit(x))
      if (x != i) fenwick[x] += fenwick[i];
}

// Array[I] += Value, O(log n).
template <typename T> void
update(T fenwick[], size_t n, size_t k, T value) {
  for (; k < n; k += lowbit(k))
    fenwick[k] = fenwick[k] + value;
}

// Return sum of index range [0, k)
template <typename T> T
query(T fenwick[], size_t k) {
  T result = 0;
  for (--k; ~k; k -= lowbit(k))
    result = result + fenwick[k];
  return result;
}
