double surreal(char s[], int n) {
  double r = 0;
  int q = 0;
  while (q < n && s[q] == s[0])
    if (s[q++] == 'L') r += 1.; else r -= 1.;
  for (double k = .5; q < n; k /= 2.)
    if (s[q++] == 'L') r += k; else r -= k;
  return r;
}
