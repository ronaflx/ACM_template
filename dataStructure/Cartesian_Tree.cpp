#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <cstring>
using namespace std;
const int N = 100000;
struct node {
	int key, value, id;
	bool operator < (const node& oth) const {
		return key < oth.key;
	}
}nodes[N];
/*lt[i] is nodes[i]'s left son, shouldn't sort again*/
int lt[N], rt[N], parent[N];
void rotate(int i) {
	while(parent[i]!=-1&&nodes[i].value<nodes[parent[i]].value) {
		rt[parent[i]] = lt[i];
		if(lt[i] != -1) parent[lt[i]] = parent[i];
		lt[i] = parent[i];
		int ff = parent[parent[i]];
		if(ff != -1) {
			parent[i] == lt[ff] ? lt[ff] = i : rt[ff] = i;
		}
		parent[i] = ff;
		parent[lt[i]] = i;
	}
}
int key[N], value[N], pos[N];
void build(int n) {
	sort(nodes, nodes + n);
	int rightmost = 0;
	for(int i = 1;i < n;i++) {
		pos[nodes[i].id] = i;
		rt[rightmost] = i;
		parent[i] = rightmost;
		rightmost = i;
		rotate(i);
	}
}
#define V(i) (i == -1 ? 0 : nodes[i].id + 1)
int main() {
	int n;
	while(scanf("%d", &n) == 1) {
		for(int i = 0;i < n;i++) {
			scanf("%d %d", &nodes[i].key, &nodes[i].value);
			nodes[i].id = i;
			key[i] = nodes[i].key;
			value[i] = nodes[i].value;
			lt[i] = rt[i] = parent[i] = -1;
		}
		build(n);
		printf("YES\n");
		for(int i = 0;i < n;i++) {
			printf("%d %d %d\n", V(parent[pos[i]]), 
				V(lt[pos[i]]), V(rt[pos[i]]));
		}
	}
	return 0;
}
