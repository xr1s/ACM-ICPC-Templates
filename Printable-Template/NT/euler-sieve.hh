template <size_t N> void
sieve(int mu[], int phi[], int sigma[], int k) {
  int facpow[N];
  bitset<N> is_comp;
  vector<int> prime;
  mu[1] = phi[1] = sigma[1] = 1;
  for (int p = 2; p != N; ++p) {
    if (!is_comp.test(p)) {  // 参数为素数
      prime.push_back(p);
      mu[p] = -1;
      phi[p] = p - 1;
      sigma[p] = power(p, k) + 1;
      facpow[p] = p;
    }
    for (int q: prime) {
      if (p * q >= N) break;
      is_comp.set(p * q);
      if (p % q == 0) {      // 参数为合数且因子中包含枚举到的素数
        mu[p * q] = 0;
        phi[p * q] = phi[p] * q;
        sigma[p * q] = sigma[facpow[p]] * (sigma[q] - 1) + 1;  // 素数幂次
        sigma[p * q] = sigma[p * q] * sigma[p / facpow[p]];    // 互素合并
        facpow[p * q] = facpow[p] * q;
        break;
      } else {               // 参数为合数且因子中不包含枚举到的素数
        mu[p * q] = -mu[p];
        phi[p * q] = phi[p] * (q - 1);
        sigma[p * q] = sigma[p] * sigma[q];
        facpow[p * q] = q;
      }
    }
  }
}
