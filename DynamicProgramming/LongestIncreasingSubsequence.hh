#include <algorithm>
#include <iterator>

/**
 * ForwardIt requires ForwardIterator.
 * Output the size of LIS of [s, t) in O(n \log n).
 * The local variable mono saves LIS it self, return this if needed.
 */
template <typename ForwardIt>
size_t LongestIncreasingSubsequence(ForwardIt s, ForwardIt t) {
  typedef typename std::iterator_traits<ForwardIt>::value_type T;
  T *mono = new T[std::distance(s, t)];
  size_t size = 0;
  while (s != t) {
    T *repl = std::lower_bound(mono, mono + size, *s);
    if (repl == mono + size) ++size;
    *repl = *s++;
  }
  delete[] mono;
  return size;
}

/**
 * Calculate Longest (non-)Increasing or Decreasing Subsequence.
 * Order is defined by different Compare which was passed as 3rd argument.
 */
template <typename ForwardIt, typename Comp>
size_t LongestIncreasingSubequence(ForwardIt s, ForwardIt t, Comp comp) {
  typedef typename std::iterator_traits<ForwardIt>::value_type T;
  T *mono = new T[std::distance(s, t)];
  size_t size = 0;
  while (s != t) {
    T *repl = std::lower_bound(mono, mono + size, *s, comp);
    if (repl == mono + size) ++size;
    *repl = *s++;
  }
  delete[] mono;
  return size;
}
