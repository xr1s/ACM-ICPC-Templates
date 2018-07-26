// Sinple implementation of std::hash specialization for std::pair and
// std::tuple, the helper functor class for unordered associative
// containers, a.k.a. std::unordered_{multi,}{set,map}.

// Unordered associative containers in STL are introduced since C++11,
// so no C++98 compatible is need here.

#if __cplusplus >= 201103L
#include <numeric>
#include <tuple>
#include <utility>

namespace std {
// std::pair is more frequently used in ACM-ICPC.
template <typename Fst, typename Snd>
struct hash<pair<Fst, Snd>> {
  size_t operator()(const pair<Fst, Snd> &p) const {
    // FIXME: Using operations that satisfy commutative is easy to collide,
    // try other hash methods.
    return hash<Fst>{}(p.first) ^ hash<Snd>{}(p.second);
  }
};
}  // namespace std

// Then std::tuple, no C++14, no std::make_index_sequece.
// Implement it in my own, not so hard.
template <size_t ...I>
struct IndexSequence {
};

template <typename, typename>
struct MakeIndexSequenceHelper;

template <size_t ...I, size_t ...J>
struct MakeIndexSequenceHelper<IndexSequence<I...>, IndexSequence<J...>> {
  using type = IndexSequence<I..., sizeof...(I) + J...>;
};

template <size_t N>
struct MakeIndexSequence {
  using type = typename MakeIndexSequenceHelper<
    typename MakeIndexSequence<N / 2>::type,
    typename MakeIndexSequence<N - N / 2>::type
  >::type;
};

template <>
struct MakeIndexSequence<1> {
  using type = IndexSequence<0>;
};

template <>
struct MakeIndexSequence<0> {
  using type = IndexSequence<>;
};

namespace std {
template <typename ...T>
struct hash<tuple<T...>> {
 private:
  template <size_t ...I>
  static size_t hash_impl(const tuple<T...> &t, IndexSequence<I...>) {
    // FIXME: Using operations that satisfy commutative is easy to collide,
    // try other hash methods.
    size_t value[] = { hash<T>{}(get<I>(t))... };
    return std::accumulate(value, value + sizeof...(T),
                           static_cast<size_t>(0), bit_xor<size_t>{});
  }
 public:
  size_t operator()(const tuple<T...> &t) const {
    return hash_impl(t, typename MakeIndexSequence<sizeof...(T)>::type{});
  }
};
}  // namespace std

#endif  // __cplusplus >= 201103L
