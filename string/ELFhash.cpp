class hash_table {
public:
	unsigned int ELFhash(char *key) {
		unsigned int h = 0, g;
		while (*key) {
			h = (h << 4) + *key++;
			g = h & 0xf0000000L;
			if (g) h ^= g >> 24;
			h &= ~g;
		}
		return h % MOD;
	}
	int find(char * str, int judge = 0) {
		int t = ELFhash(str);
		for (hashCell * i = g[t]; i != NULL; i = i->next)
			if (!strcmp(i->str, str))
				return i->p = judge ? i->p + 1: i->p;
		return 0;
	}
	void insert(char* str, int p) {
		if(find(str, 1)) return;
		unsigned t = ELFhash(str);
		strcpy(pp->str, str);
		pp->p = p;
		pp->next = g[t];
		g[t] = pp++;
	}
private:
	const static int MOD = 387173;
	const static int SIZE = 380001;
	struct hashCell {
		char str[20];
		int p;
		hashCell * next;
	} pool[SIZE], *g[MOD], *pp;
};
