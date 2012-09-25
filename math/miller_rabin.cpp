/*miller-rabin algorithm do it 5 times or more*/
long long qmod(long long a,long long b,long long c) {
    long long res = 1,temp = a % c;
    while(b) {
        if(b & 1)
            res = (res * temp) % c;
        b>>= 1;
        temp = (temp * temp) % c;
    }
    return res;
}
bool miller(long long n,int t) {
    if(n == 1) return false;
    else if(n == 2) return true;
    for(int i = 0;i < t;i++) {
        srand(time(NULL));
        long long a = rand() % (n - 2) + 1;
        int b = qmod(a,n - 1,n);
        if(b != 1 && b != n - 1)
            return false;
    }
    return true;
}
int main() {
    long long a;
    while(scanf("%lld",&a) == 1) {
        if(miller(a,4)) printf("YES\n");
        else printf("NO\n");
    }
    return 0;
}
