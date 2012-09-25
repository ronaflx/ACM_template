//f(n * m) = f(n) * f(m)
//f(n) = n * (1 - 1 / p1) *(1 - 1 / p2) (piis n's prim factor)
int euler(int n) {
    int e = n, i, j;
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            e = e / i * (i - 1);
            while (n % i == 0)
                n = n / i;
        }
    }
    if (n > 1) e = e / n * (n - 1);
    return e;
}

