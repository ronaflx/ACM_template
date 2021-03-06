#include <cmath>
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
void get_suffix(const char* sub, int len, int next[]) {
	//extend[i] = len(lcp(sub, sub.substr(i)))
	int pos = 1, j = 0;
	while(sub[j + 1] == sub[j]) j++;
	next[0] = len, next[pos] = j;
	for(int i = 2;i < len;i++) {
		int ll = pos + next[pos], cur = next[i - pos];
		if(ll > i + cur) {
			next[i] = cur;
		} else {
			j = max(ll - i, 0);
			while(sub[i + j] == sub[j] && i + j < len) j++;
			next[i] = j;
			pos = i;
		}
	}
}
void extend_kmp(const char* str, int n, const char* sub, int m, int extend[], int next[]) {
	get_suffix(sub, m, next);
	int j = 0, pos = 0;
	while(str[j] == sub[j] && j < n && j < m) j++;
	extend[0] = j;
	for(int i = 1;i < n;i++) {
		int ll = pos + extend[pos], cur = next[i - pos];
		if(ll > i + cur) {
			extend[i] = cur;
		} else {
			j = max(ll - i, 0);
			while(str[i + j] == sub[j] && i + j < n && j < m) j++;
			extend[i] = j;
			pos = i;
		}
	}
}
