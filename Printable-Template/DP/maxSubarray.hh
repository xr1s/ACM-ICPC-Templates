template <typename InputIt>
auto maxSubarray(InputIt s, InputIt t) {
  using T = typename iterator_traits<InputIt>::value_type;
  T result = 0, sum = 0;
  for (; s != t; ++s) {
    if ((sum += *s) < 0) sum = 0;
    result = max(result, sum);
  }
  return result;
}
