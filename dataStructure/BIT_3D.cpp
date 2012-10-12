/*3D BIT escape index 0*/
const int size = 201;
int c[size][size][size], n;
int lowbit(int k) {
    return k & (k ^(k - 1));
}
int sum(const int x1, const int y1, const int z1) {
    int s = 0;
    for(int i = x1;i > 0;i -=lowbit(i))
        for(int j = y1;j > 0;j -= lowbit(j))
            for(int k = z1;k > 0;k -= lowbit(k))
                s += c[i][j][k];
    return s;
}
void modify(const int x1, const int y1, const int z1, int val) {
    for(int i = x1;i < size;i +=lowbit(i))
        for(int j = y1;j < size;j += lowbit(j))
            for(int k = z1;k < size;k += lowbit(k))
                c[i][j][k] += val;
}
/* scanf("%d %d %d %d %d %d %d", &a, &b, &d, &x, &y, &z,&val);
 * modify(a, b, d, val); modify(x + 1, b, d, -val);
 * modify(a, y + 1, d, -val); modify(x + 1, y + 1, d, val);
 * modify(a, b, z + 1, -val); modify(a, y + 1, z + 1, val);
 * modify(x + 1, b, z + 1, val); modify(x + 1, y + 1, z + 1, -val);*/
