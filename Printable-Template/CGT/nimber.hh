size_t nimberMultTable[16][16] = {
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
  { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15},
  { 0,  2,  3,  1,  8, 10, 11,  9, 12, 14, 15, 13,  4,  6,  7,  5},
  { 0,  3,  1,  2, 12, 15, 13, 14,  4,  7,  5,  6,  8, 11,  9, 10},
  { 0,  4,  8, 12,  6,  2, 14, 10, 11, 15,  3,  7, 13,  9,  5,  1},
  { 0,  5, 10, 15,  2,  7,  8, 13,  3,  6,  9, 12,  1,  4, 11, 14},
  { 0,  6, 11, 13, 14,  8,  5,  3,  7,  1, 12, 10,  9, 15,  2,  4},
  { 0,  7,  9, 14, 10, 13,  3,  4, 15,  8,  6,  1,  5,  2, 12, 11},
  { 0,  8, 12,  4, 11,  3,  7, 15, 13,  5,  1,  9,  6, 14, 10,  2},
  { 0,  9, 14,  7, 15,  6,  1,  8,  5, 12, 11,  2, 10,  3,  4, 13},
  { 0, 10, 15,  5,  3,  9, 12,  6,  1, 11, 14,  4,  2,  8, 13,  7},
  { 0, 11, 13,  6,  7, 12, 10,  1,  9,  2,  4, 15, 14,  5,  3,  8},
  { 0, 12,  4,  8, 13,  1,  9,  5,  6, 10,  2, 14, 11,  7, 15,  3},
  { 0, 13,  6, 11,  9,  4, 15,  2, 14,  3,  8,  5,  7, 10,  1, 12},
  { 0, 14,  7,  9,  5, 11,  2, 12, 10,  4, 13,  3, 15,  1,  8,  6},
  { 0, 15,  5, 10,  1, 14,  4, 11,  2, 13,  7,  8,  3, 12,  6,  9},
};

template <typename T> T
nimberMultExpo(const T &lhs, const T &rhs) {
  if (lhs < 16) return nimberMultTable[lhs][rhs];
  int expo = 0;
  while (static_cast<T>(1) << (1 << expo) <= lhs) ++expo;
  const T base = static_cast<T>(1) << (1 << --expo);
  const T lh = lhs >> (1 << expo);
  const T rh = rhs >> (1 << expo), rl = rhs & (base - 1);
  const T deg2 = nimberMultExpo(lh, rh);
  const T deg1 = nimberMultExpo(lh, rl);
  return (deg2 ^ deg1) * base ^ nimberMultExpo(base >> 1, deg2);
}

template <typename T> T
nimberMultiply(const T &lhs, const T &rhs) {
  if (lhs < rhs) return nimberMultiply(rhs, lhs);
  if (lhs < 16) return nimberMultTable[lhs][rhs];
  int expo = 0;
  while (static_cast<T>(1) << (1 << expo) <= lhs) ++expo;
  const T base = static_cast<T>(1) << (1 << --expo);
  const T lh = lhs >> (1 << expo), ll = lhs & (base - 1);
  const T rh = rhs >> (1 << expo), rl = rhs & (base - 1);
  const T deg2 = nimberMultiply(lh, rh);
  const T deg1 = nimberMultiply(lh, rl) ^ nimberMultiply(ll, rh);
  const T deg0 = nimberMultiply(ll, rl);
  return (deg2 ^ deg1) * base ^ deg0 ^ nimberMultExpo(base >> 1, deg2);
}
