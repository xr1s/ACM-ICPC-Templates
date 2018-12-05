#include <iostream>
#include <cstring>
#include <unordered_set>
using namespace std;

int mem[100][100];

int sg(int n, int m) {
  if (n == 0 && m == 0) return 0;
  if (mem[n][m] != -1) return mem[n][m];
  unordered_set<int> us;
  for (int i = 0; i <= n; ++i)
    for (int j = 0; j <= m; ++j) {
      if (i == n && j != m) us.insert(sg(n, j));
      if (i != n && j == m) us.insert(sg(i, m));
      if (i != n && j != m) us.insert(sg(i, m) ^ sg(n, j));
    }
  for (int i = 0; ; ++i) if (!us.count(i)) return mem[n][m] = i;
}

int main() {
  memset(mem, 0xff, sizeof mem);
  for (int i = 0; i != 20; ++i)
    for (int j = 0; j != 20; ++j)
      cout << sg(i, j) << "\t\n"[j == 19];
}
