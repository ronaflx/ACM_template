typedef long long LL;
LL min;
LL multi(LL a, LL b, LL n)
{
    LL tmp = a % n, s = 0;
    while(b)
    {
        if(b & 1) s = (s + tmp) % n;
        tmp = (tmp + tmp) % n;
        b >>= 1;
    }
    return s;
}
LL gcd(LL a, LL b)
{
    return b ? gcd(b, a % b) : a;
}
LL pollard_rho(LL n, LL c)
{
    LL x, y, d, i = 1, k = 2;
    srand((LL)(0));
    x = ((LL) rand()) % (n - 1) + 1;
    y = x;
    while(1)
    {
        i ++;
        x = (multi(x, x, n) + c) % n;
        d = gcd(y - x + n, n);
        if(d != 1 && d != n) return d;
        if(y == x) return n;
        if(i == k) y = x, k <<= 1;
    }
}
void find(LL n, LL c)
{
    LL r;
    if(n <= 1) return;
    if(test(n))
    {
        if(min > n) min = n;
        return;
    }
    r = pollard_rho(n, c--);
    find(n / r, c);
    find(r, c);
}
LL MaxPrimeFactor(LL n)
{
    if(test(n)) return n;
    LL k = -1, g;
    min = n;
    find(n, C);
    g = MaxPrimeFactor(min);
    k = g > k ? g : k;
    g = MaxPrimeFactor(n / min);
    k = g > k ? g : k;
    return k;
}
int main()
{
    LL n;
    while(scanf("%lld", &n) == 1)
    {
        if(test(n))                 //test(n) is miller robin
            printf("Yes\n");
        else
        {
            min = n;                   //min is the min factor of n
            find(n, C);
            printf("No %lld\n",min);
            //printf("%lld\n",MaxPrimeFactor(n));
        }
    }
    return 0;
}
