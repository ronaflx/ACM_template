const int SIZE = 12;
double guass(int n, double mat[][SIZE]) {
  for (int i = 1; i < n; i++) {
    for (int j = 0; j < i; j++) {
      if (mat[i][j] == 0) continue;
      double kk = mat[i][j] / mat[j][j];
      for (int k = 0; k < n; k++) mat[i][k] -= kk * mat[j][k];
    }
  }
  double res = 1.00;
  for (int i = 0; i < n; i++) res *= mat[i][i];
  return fabs(res) + 0.005;
}
double maps[SIZE][SIZE];
int main() {
  int n, t;
  scanf("%d", &t);
  while (t--) {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
      int cnt = 0;
      for (int j = 0; j < n; j++) {
        scanf("%lf", &maps[i][j]);
        if (i == j) maps[i][j] = 0;
        if (maps[i][j] == 1) cnt++;
        maps[i][j] = -maps[i][j];
      }
      maps[i][i] = cnt;
    }
    printf("%.0lf\n", guass(n - 1, maps));
  }
  return 0;
}

