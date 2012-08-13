struct Matrix {
	LL A[maxn][maxn];
	int size;
	Matrix() {
		memset(this, 0, sizeof (*this));
	}
	void set_E() {
		for(int i = 0;i < size;i++) {
			A[i][i] = 1;
		}
	}
};
Matrix operator+(Matrix m1, Matrix m2) {
    Matrix ret;
    ret.size = m1.size;
    for (int i = 0; i < ret.size; ++i)
        for (int j = 0; j < ret.size; ++j)
            ret.A[i][j] = (m1.A[i][j] + m2.A[i][j]) % MOD;
    return ret;
}
Matrix operator*(const Matrix& m1, const Matrix& m2) {
	Matrix ret;
	ret.size = m1.size;
	for (int i = 0; i < ret.size; ++i)
		for (int j = 0; j < ret.size; ++j) {
			for (int k = 0; k < ret.size; ++k) {
				ret.A[i][j] += m1.A[i][k] * m2.A[k][j];
			}
			ret.A[i][j] %= MOD;
		}
	return ret;
}
Matrix mypower(const Matrix& m, int n) {
	Matrix ret, tmp = m;
	ret.size = m.size;
	ret.set_E();
	while(n != 0) {
		if(n & 1) {
			ret = ret * tmp;
		}
		n >>= 1;
		tmp = tmp * tmp;
	}
	return ret;
}
Matrix sumpower(const Matrix& m, int n) {
    Matrix tmp;
    if (n == 1) return m;
    tmp = sumpower(m, n / 2);
    if (n & 1)
        return mypower(m, n / 2) * tmp + tmp + mypower(m, n);
    return mypower(m, n / 2) * tmp + tmp;
}

