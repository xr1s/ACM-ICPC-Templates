#include <iterator>

// @brief generate partial match table of [begin, end) to pmt
template <typename Input, typename Output>
void generatePMT(Input begin, Input end, Output pmt) {
  typedef typename std::iterator_traits<Output>::value_type ValueType;

  if (begin == end) return;
  size_t len = std::distance(begin, end);
  *pmt = -1;
  for (ValueType i = 0, j = -1; i < len - 1; ) {
    if (j == -1 || begin[i] == begin[j]) {
      if (begin[++i] != begin[++j]) {
        pmt[i] = j;
      } else {
        pmt[i] = pmt[j];
      }
    } else j = pmt[j];
  }
}

// TODO: KMP match function
// TODO: extended KMP
