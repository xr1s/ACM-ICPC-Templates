int LIS(char s[], char t[], int slen, int tlen) {
  int lcs[slen][tlen] = {{0}};
  for (int i = 0; i != slen; ++i)
    for (int j = 0; j != tlen; ++j) {
      if (s[i] == t[j])
        if (i == 0 || j == 0) lcs[i][j] = 1;
        else lcs[i][j] = lcs[i - 1][j - 1] + 1;
      if (i) lcs[i][j] = max(lcs[i][j], lcs[i - 1][j]);
      if (j) lcs[i][j] = max(lcs[i][j], lcs[i][j - 1]);
    }
  return lcs[slen - 1][tlen - 1];
}
