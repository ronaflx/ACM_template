const int N = 129;
const double eps = 1e-6;
double A[N][N],b[N],x[N];
bool gauss(int n) {
	int p,i,j;
	double **cof = new double *[n];
	for(int i = 0; i < n; i++) {
		cof[i] = new double[n];
		memcpy(cof[i],A[i],n * sizeof(double));
	}
	for(p = 0; p < n; p++) {
		int max = p;
		for(j = p + 1; j < n; j++) {
			if(fabs(cof[max][p]) < fabs(cof[j][p]))
				max = j;
		}
		if(fabs(cof[max][p]) < eps)
			return false;
		if(max != p) {
			swap(cof[max],cof[p]);
			swap(b[max],b[p]);
		}
		for(i = p + 1; i < n; i++) {
			double rate = cof[i][p] / cof[p][p];
			for(int j = p; j < n; j++)
				cof[i][j] -= rate * cof[p][j];
			b[i] -= rate * b[p];
		}
	}
	for(p = n - 1; p >= 0; p--) {
		x[p] = b[p] / cof[p][p];
		for(int j = 1; j <= p; j++) {
			b[p - j] -= cof[p - j][p] * x[p];
			cof[p - j][p] = 0.0;
		}
	}
	return true;
}
int gauss (int n, int m) {
	int i, j;
	for(i = 0, j = 0; i < n && j < m; i++, j++) {
		for(int k = i; k < n; k++) {
			if(!A[k][j]) continue;
			for(int s = j; s < m; s++)
				swap(A[i][s], A[k][s]);
			swap(b[k], b[i]);
			break;
		}
		if(!A[i][j]) {
			i--;
			continue;
		}
		for(int k = i + 1; k < n; k++) {
			if(A[k][j] == 0) continue;
			for(int s = j; s < m; s++)
				A[k][s] ^= A[i][s];
			b[k] ^= b[i];
		}
	}
	for(int k = i; k < n; k++)
		if(b[k]) return -1;
	for(int k = i - 1;k >= 0;k--) {
		int pos = 0;
		for(int i = 0;i < n;i++) {
			if(A[k][i]) {
				pos = i;
				break;
			}
		}
		x[pos] ^= b[k];
		for(int i = pos + 1;i < n;i++) {
			x[pos] ^= A[k][i] * x[i];
		}
	}
	return m - i;
}
