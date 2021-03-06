LL a[N], f[N];// mod must be prime
LL qmod(LL a, LL b, LL c) {
	LL res = 1, tmp = a % c;
	while (b != 0) {
		if ((b & 1) != 0)
			res = (res * tmp) % c;
		b >>= 1;
		tmp = (tmp * tmp) % c;
	}
	return res;
}
LL inverse(LL x, LL mod) {
	return qmod(x, MOD -2, MOD);
}
LL comb(int n, int m, LL mod) {
	if(m > n) return 0;
	return f[n] * inverse(f[m], mod) % mod * inverse(f[n - m], mod) % mod;
}
