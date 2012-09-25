//countbit from [a, b]
int countbit(int x) {
	int res = 0;
	while(x) {
		res++;
		x &= (x - 1);
	}
	return res;
}
LL solve(int x) {
	if(x == 0) return 0;
	if((x & 1) == 0) {
		return countbit(x) + solve(x - 1);
	} else {
		return (x + 1LL) / 2 + 2 * solve(x / 2);
	}
}
LL solve(LL a, LL b) {
	if(a > 0) {
		return solve(b) - solve(a - 1);
	}
	else if(a == 0) {
		return solve(b);
	} else {
		LL res = 32 * -a - solve(~a);
		if(b >= 0) {
			res += solve(b);
		} else {
			res -= 32 * -(b + 1) - solve(~(b + 1));
		}
		return res;
	}
}
