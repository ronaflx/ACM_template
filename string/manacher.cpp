string longestPalindrome(string s) {
	int n = s.size() * 2 + 1;
	char* str = new char[n + 1];
	str[0] = '#';
	for(int i = 0;i < n / 2;i++) {
		str[(i << 1) + 1] = s[i];
		str[(i << 1) + 2] = '#';
	}
	vector<int> p(n);
	int id = 0, maxn = 0;
	for(int i = 1;i < n;i++) {
		p[i] = maxn > i ? min(p[id * 2 - i], maxn - i) : 0;
		while(i - p[i] > 0 && i + p[i] + 1 < n && str[i + p[i] + 1] == str[i - p[i] - 1]) {
			p[i]++;
		}
		if(i + p[i] > maxn) {
			maxn = i + p[i];
			id = i;
		}
	}
	delete[] str;
	id = max_element(p.begin(), p.end()) - p.begin();
	return s.substr((id - p[id]) / 2, p[id]);
}
