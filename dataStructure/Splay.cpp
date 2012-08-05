//be careful with pushDown
template<typename T = int>
class Splay
{
private:
#define SIZE(x) ((x) ? (x)->size : 0)
#define SUM(x)  ((x) ? (x)->sum  : 0)
#define VAL(x)  ((x) ? (x)->val  : 0)
#define CENTRE  (root->ch[1]->ch[0])
    struct node
    {
        int size, sum, add;
        node* ch[2], *pre;
        T v;
        node(T v = T(), int size = 1, node* l = NULL,
             node* r = NULL, node* pre = NULL)
        {
            this->v = v;
            sum = add = 0;
            this->size = size;
            this->pre = pre;
            ch[0] = l, ch[1] = r;
        }
    };
    node * root;
    void pushDown(node*& x)
    {
//	    node* y = x->ch[0];
//	    if(y)
//	    {
//		    y->add += x->add;
//		    y->sum += SIZE(y) * x->add;
//	    }
//	    y = x->ch[1];
//	    if(y)
//	    {
//		    y->add += x->add;
//		    y->sum += SIZE(y) * x->add;
//	    }
//	    x->add = 0;
    }

    void pushUp(node*& x)
    {
        x->size = SIZE(x->ch[0]) + SIZE(x->ch[1]) + 1;
        x->sum = SUM(x->ch[0]) + SUM(x->ch[1]) + x->add;
    }
    void rotate(node* x, int type)
    {
        node *y = x->pre;
        pushDown(y);
        pushDown(x);
        y->ch[!type] = x->ch[type];
        if (x->ch[type] != NULL) x->ch[type]->pre = y;
        x->pre = y->pre;
        if (y->pre != NULL)
        {
            if (y->pre->ch[0] == y) y->pre->ch[0] = x;
            else y->pre->ch[1] = x;
        }
        x->ch[type] = y, y->pre = x;
        if (y == root) root = x;
        pushUp(y);
        pushUp(x);
    }
    void insert(node* &R, T v = T(), node* f = NULL)
    {
        if (R == NULL)
        {
            R = new node(v, 1, NULL, NULL, f);
            splay(R, NULL);
            return;
        }
        else if (v <= R->v) insert(R->ch[0], v, R);
        else if (v >  R->v) insert(R->ch[1], v, R);
    }
    void clear(node*& root)
    {
        if(root == NULL) return;
        clear(root->ch[0]);
        clear(root->ch[1]);
        delete root;
        root = NULL;
    }
    node* join(node*& x, node*& y)
    {
        if(x == NULL) return y;
        if(y == NULL) return x;
        x->pre = y->pre = NULL;
        node* z = x;
        while(z->ch[1] != NULL) z = z->ch[1];
        splay(z, NULL);
        z->ch[1] = y;
        y->pre = z;
        pushDown(z);
        return z;
    }
    void splay(node* x, node* f)
    {
        pushDown(x);
        while(x->pre != f)
        {
            if (x->pre->pre == f)
            {
                if (x->pre->ch[0] == x) rotate(x, 1);
                else rotate(x, 0);
            }
            else
            {
                node *y = x->pre, *z = y->pre;
                if (z->ch[0] == y)
                {
                    if (y->ch[0] == x) //  l
                        rotate(y, 1), rotate(x, 1);
                    else // z
                        rotate(x, 0), rotate(x, 1);
                }
                else
                {
                    if (y->ch[1] == x) // l
                        rotate(y, 0), rotate(x, 0);
                    else // z
                        rotate(x, 1), rotate(x, 0);
                }
            }
        }
        pushUp(x);
    }
    node* MaxOrMin(node* x, int type)//0 minimum 1 maximum
    {
        if(x == NULL) return x;
        while(x->ch[type] != 0) x = x->ch[type];
        return x;
    }
public:
    Splay(): root(NULL) {}
    void insert(T v)
    {
        insert(root, v);
    }
    int size()
    {
        return SIZE(root);
    }
    void clear()
    {
        clear(root);
    }
    node* MaxOrMin(int type)
    {
        return MaxOrMin(root, type);
    }
    int rank(T v)
    {
        node* x = find(root, v);
        return SIZE(x) + 1;
    }
    node* selectK(int k)
    {
        node* x = root;
        while(x != NULL && SIZE(x->ch[0]) + 1 != k)
        {
            if(k <= SIZE(x->ch[0])) x = x->ch[0];
            else
            {
                k -= SIZE(x->ch[0]) + 1;
                x = x->ch[1];
            }
        }
        if(x != NULL) splay(x, NULL);
        return x;
    }
    node* find(T v)
    {
        node* x = root;
        while(x != NULL && x->v != v)
            x = x->ch[v > x->v];
        return x;
    }
    void erase(T v)
    {
        node* x = find(v);
        if(x == NULL) return;
        splay(x, NULL);
        root = join(x->ch[0], x->ch[1]);
        if(root != NULL)
            root->pre = NULL;
        delete x;
    }
    node* PreOrSuc(T v, int type)// 0 predecessor 1 successor
    {
        node* x = find(v);
        if(x == NULL) return NULL;
        else return MaxOrMin(x->ch[type], !type);
    }
    void update(int l, int r, int c)
    {
        node* ll = find(l - 1);
        splay(ll, NULL);
        node* rr = find(r + 1);
        splay(rr, root);
        if(root->ch[1] == NULL || CENTRE == NULL) return;
        CENTRE->add += c;
        CENTRE->sum += c;
    }
    int query(int l, int r)
    {
        node* ll = find(l - 1);
        splay(ll, NULL);
        node* rr = find(r + 1);
        splay(rr, root);
        if(root->ch[1] == NULL || CENTRE == NULL) 0;
        return CENTRE->sum;
    }
};
Splay<int> spl;
int main()
{
    int n, t;
    int a, b, cases = 1;
    char cmd[100];
    scanf("%d", &t);
    while(t-- && scanf("%d", &n) == 1)
    {
        spl.clear();
        for(int i = 0; i <= n + 1; i++) spl.insert(i);
        for(int i = 1; i <= n; i++)
        {
            scanf("%d", &a);
            spl.update(i, i, a);
        }
        printf("Case %d:\n", cases++);
        while(scanf("%s", cmd) == 1 && strcmp(cmd, "End"))
        {
            scanf("%d %d", &a, &b);
            if(!strcmp(cmd, "Query"))
            {
                printf("%d\n", spl.query(a, b));
            }
            else if(!strcmp(cmd, "Add"))
            {
                spl.update(a, a, b);
            }
            else
            {
                spl.update(a, a, -b);
            }
        }
    }
    return 0;
}
