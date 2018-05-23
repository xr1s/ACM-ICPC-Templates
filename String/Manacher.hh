#include <iterator>

//   @tparam Input: requires InputIterator.
//   @tparam Output: does not require to be an OutputIterator, but instead a
// RandomAccessIterator.
//   @param padding: will be insert between each character in string, usually #.
template <typename Input, typename Output>
void manacher(Input begin, Input end, Output palindrome,
    const typename std::iterator_traits<Output>::value_type &padding = -1) {
  typedef typename std::iterator_traits<Output>::value_type ValueType;

  size_t diff = std::distance(begin, end);
  ValueType *s = new ValueType[diff * 2 + 1];
  for (size_t i = 0; i != diff; ++i) {
    s[i * 2 + 0] = padding;
    s[i * 2 + 1] = *begin++;
  }
  s[diff * 2] = padding;

  const size_t len = diff * 2 + 1;
  size_t rightmost = 0, symmetry = 0;
  for (size_t i = 0; i != len; ++i) {
    palindrome[i] = i < rightmost ? \
        std::min(palindrome[2 * symmetry - i], rightmost - i) : 1;
    while (i >= palindrome[i] && i + palindrome[i] < len && \
        s[i - palindrome[i]] == s[i + palindrome[i]]) ++palindrome[i];
    if (palindrome[i] + i >= rightmost) {
      rightmost = palindrome[i] + i - 1;
      symmetry = i;
    }
  }
  for (size_t i = 0; i != len; ++i) --palindrome[i];
  delete[] s;
}
