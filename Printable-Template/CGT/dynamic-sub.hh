vector<int> dynSubPPos(int n, int k) {
  vector<int> p{0, 1};  // P-Positions
  for (int i = 0; p.back() < n; ) {
    while (p[i + 1] * k < p.back()) ++i;
    p.push_back(p.back() + p[i + 1]);
  }
  return p;
}

int dynSubPick(int n, int k) {
  vector<int> p = getPPosition(n, k);
  int pick = -1;
  if (n != p.back())
    for (; n; p.pop_back())
      if (n >= p.back())
        n -= pick = p.back();
  return pick;
}
