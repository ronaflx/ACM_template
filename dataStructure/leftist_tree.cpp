#define CMP(a, b) ((a) > (b))
#define DIST(v) ((v == NULL) ? -1 : (v->dist))
//must be careful when clear after merge
//because of the pointer could not be NULL
//especially when use new just makeNULL when memory is enough
template<typename T>
class leftist_tree {
private:
    class node {
    public:
        T v;
        int dist;
        node *rr, *ll;
        node(){rr = ll = NULL; dist = 0;}
        node(T v){this->v = v; rr = ll = NULL;dist = 0;}
    };
    node* root;
    int s;
    node* merge(node* &left, node* &right) {
        if(left == NULL) return right;
        if(right == NULL) return left;
        if(CMP(right->v, left->v)) swap(left, right);
        left->rr = merge(left->rr, right);
        if(DIST(left->rr) > DIST(left->ll)) swap(left->ll, left->rr);
        left->dist = DIST(left->rr) + 1;
        return left;
    }
    void clear(node* root) {
        if(root == NULL) return;
        clear(root->ll);
        clear(root->rr);
        delete root;
        root = NULL;
    }
public:
    leftist_tree(){root = NULL;s = 0;}
    ~leftist_tree(){clear(root);}
    void push(T v) {
        node * newNode = new node(v);
        root = merge(newNode, root);
        s++;
    }
    void clear(){clear(root);}
    int size(){return this->s;}
    T top(){return root->v;}
    void pop() {
        node *tmp = root;
        root = merge(root->ll, root->rr);
        delete tmp;
        s--;
    }
    void merge(leftist_tree<T>& tree) {
        this->root = merge(root, tree.root);
        s += tree.s;
        tree.root = NULL;
    }
    void makeNULL(){root = NULL;}
};
