typedef long long LL;
LL gcd(LL a, LL b) {
    if(b == 0) return a;
    return gcd(b, a % b);
}
LL extended_euclid(LL a,LL b,LL &x,LL &y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    LL ret = extended_euclid(b ,a % b, x, y), t = x;
    x = y;
    y = t - a / b * y;
    return ret;
}
bool modular_linear(LL a,LL b,LL c) {
    LL x, y;
    LL d = extended_euclid(a,b,x,y);
    if (c%d) return 0;
    return true;
}
LL linearModularSystem(LL* m, LL* r, int n) {
    LL M = accumulate(m, m + n, 1, multiplies<LL>());
    LL ans = 0;
    for(int i = 0; i < n; i++) {
        LL Mi = M / m[i], pi, qi;
        LL gcd = extended_euclid(Mi, m[i], pi, qi);
        if(Mi % gcd) return -1;
        ans = (ans + Mi * pi * r[i]) % M;
    }
    return ans <= 0 ? ans + M : ans;
}//minimum non-negative answer
LL linearModularSystemP(LL* m, LL* r, int n) {
    LL m0 = m[0], r0 = r[0];
    LL m1, r1;
    LL x, y, t;
    for(int i = 1; i < n; i++)
    {
        r1 = r[i], m1 = m[i];
        long long gcd = extended_euclid(m0, m1, x, y);
        LL c = r1 - r0;
        if(c % gcd != 0) return -1;
        //m0 * x - m1 * y = r1 - r0
        t = m1 / gcd;
        x = (c / gcd * x % t + t) % t;
        r0 = r0 + x * m0;
        m0 = m0 * m1 / gcd;
    }
    return r0;
}
