template <typename T>
T legendre(T a, T p) {
    return modpow(a, p - 1 >> 1, p);
}

template <typename T>
T tonelli_shanks(T n, T p) {
    if (n == 0) return 0;
    if (p == 2) return n & 1;
    if (legendre(n, p) != 1) return -1;
    T q = p - 1, s = 0, z = 2;
    while (~q & 1) q >>= 1, ++s;
    if (s == 1)
        return modpow(n, p + 1 >> 2, p);
    while (legendre(z, p) != p - 1) ++z;
    T c = modpow(z, q, p);
    T r = modpow(n, q + 1 >> 1, p);
    T t = modpow(n, q, p);
    for (T i = 0, m = s; t != 1; m = i, i = 0) {
        for (z = t; z != 1; z = z * z % p) ++i;
        while (i < --m) c = c * c % p;
        r = r * c % p, c = c * c % p, t = t * c % p;
    }
    return r;
}
