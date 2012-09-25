const int N = 100000;
int tag[N], p[N];
void get_prime() {
    int cnt = 0;
    for (int i = 2; i < N; i++) {
        if (!tag[i]) p[cnt++] = i;
        for (int j = 0; j < cnt && p[j] * i < N; j++) {
            tag[i*p[j]] = 1;
            if (i % p[j] == 0) break;
        }
    }
}
