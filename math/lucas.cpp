LL extended_gcd (LL a, LL b, LL &x, LL &y)
{
    if (!b)
        return x = 1, y = 0, a;
    LL ret = extended_gcd (b, a % b, x, y), tmp = x;
    x = y, y = tmp - (a / b) * y;
    return ret;
}
LL combination_Mod_p (LL n, LL m, LL p)
{
    if (m > n) return 0;
    m = (n - m < m) ? n - m : m;
    LL a = 1, b = 1, x, y, pcnt = 0;
    for (int i = 1; i <= m; i++)
    {
        a *= n - i + 1, b *= i;
        while (a % p == 0) a /= p, pcnt++;
        while (b % p == 0) b /= p, pcnt--;
        b %= p, a %= p;
    }
    if (pcnt) return 0;
    extended_gcd(b, p, x, y);
    if (x < 0) x += p;
    x *= a, x %= p;
    return x;
}
LL lucas (LL m, LL n, LL p)
{
    LL result = 1;
    while (m && n && result)
        result *= combination_Mod_p (m % p, n % p, p), result %= p, m /= p, n /= p;
    return result;
}
