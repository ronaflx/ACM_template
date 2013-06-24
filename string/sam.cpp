namespace SAM {
const int MAXN = 600000;
struct Node {
    Node *ch[26], *f; int l;
} a[MAXN], *root, *acc, *ptr;
void Initial() {
    memset(a, 0, sizeof(a));
    acc = root = a, ptr = a + 1;
}
void AddSuffix(int x) {
    using namespace std;
    Node * cur = ptr++, *fail = acc;
    cur->l = acc->l + 1; acc  = cur;
    for(;fail && !fail->ch[x];fail = fail->f)
        fail->ch[x] = cur;
    if(!fail) {
        cur->f = root;
    } else if(fail->l + 1 == fail->ch[x]->l) {
        cur->f = fail->ch[x];
    } else {
        Node* r = ptr++, * q = fail->ch[x];
        *r = *q, r->l = fail->l + 1;
        cur->f = q->f = r;
        for(;fail && fail->ch[x] == q;fail = fail->f)
            fail->ch[x] = r;
    }
}
int lcs(const char * src, const char * dest) {
    Initial();
    int n = strlen(src), m = strlen(dest), ans = 0, mid = 0;
    Node * acc = root;
    for(int i = 0;i < n;i++) {
        SAM::AddSuffix(src[i] - 'a');
    }
    for(int i = 0;i < m;++i) {
        int v = dest[i] - 'a';
        if(acc->ch[v]) {
            ++mid;
            acc = acc->ch[v];
        } else {
            for(;acc && !acc->ch[v];acc = acc->f);
            mid = acc ? acc->l + 1 : 0;
            acc = acc ? acc->ch[v] : root;
        }
        ans = max(ans, mid);
    }
    return ans;
}
}
