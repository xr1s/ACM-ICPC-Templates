#include <stdexcept>
#include <bitset>
using namespace std;

template <size_t N>
struct LinearBasis {
  template <typename ForwardIt>
  bool insert(ForwardIt begin, ForwardIt end) {
    if (distance(begin, end) != N) throw invalid_argument("");
  }
 private:
  double matrix[N][N];
  bitset<N> alloc;
};
