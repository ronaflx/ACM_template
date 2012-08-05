#define MOD 1000000007
using namespace std;
const int maxn = 256;
struct Matrix {
	LL A[maxn][maxn];
	int size;
	Matrix() {
		memset(this, 0, sizeof (*this));
	}
};
LL mymod(LL x) {
	return (x % MOD + MOD) % MOD;
}
Matrix operator+(const Matrix& m1, const Matrix& m2) {
    Matrix ret;
    ret.size = m1.size;
    for (int i = 0; i < ret.size; ++i)
        for (int j = 0; j < ret.size; ++j)
            ret.A[i][j] = mymod(m1.A[i][j] + m2.A[i][j]);
    return ret;
}
Matrix operator-(const Matrix& m1, const Matrix& m2) {
    Matrix ret;
    ret.size = m1.size;
    for (int i = 0; i < ret.size; ++i)
        for (int j = 0; j < ret.size; ++j)
            ret.A[i][j] = mymod(m1.A[i][j] - m2.A[i][j]);
    return ret;
}
Matrix operator*(const Matrix& m1, const Matrix& m2) {
	Matrix ret;
	ret.size = m1.size;
	for (int i = 0; i < ret.size; ++i)
		for (int j = 0; j < ret.size; ++j) {
			ret.A[i][j] = 0;
			for (int k = 0; k < ret.size; ++k) {
				ret.A[i][j] += m1.A[i][k] * m2.A[k][j];
				ret.A[i][j] = mymod(ret.A[i][j]);
			}
		}
	return ret;
}
Matrix mypower(const Matrix& m, int n) {
	Matrix ret, tmp;
	ret.size = m.size;
	if (n == 0) {
		for (int i = 0; i < ret.size; ++i)
			ret.A[i][i] = 1;
		return ret;
	}
	tmp = mypower(m, n / 2);
	if (n & 1)
		return tmp * tmp * m;
	else return tmp * tmp;
}
