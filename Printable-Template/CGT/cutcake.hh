int cutcake(int n, int m) {
  while (n != 1 && m != 1)
    n >>= 1, m >>= 1;
  return n - m;
}
