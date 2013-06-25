#define DIST(v) ((v == NULL) ? -1 : (v->dist))
template<typename T, class Compare = greater<T> >
class LeftistTree {
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
    Compare _compare;
    node* Merge(node* left, node* right) {
        if(left == NULL) return right;
        if(right == NULL) return left;
        if(_compare(right->v, left->v)) swap(left, right);
        left->rr = Merge(left->rr, right);
        if(DIST(left->rr)>DIST(left->ll))swap(left->ll, left->rr);
        left->dist = DIST(left->rr) + 1;
        return left;
    }
    void Clear(node*& root) {
        if(root == NULL) return;
        Clear(root->ll);
        Clear(root->rr);
        delete root;
        root = NULL;
    }
public:
    LeftistTree(){root = NULL;s = 0;}
    ~LeftistTree(){Clear(root);}
    void Push(T v) {
        node * newNode = new node(v);
        root = Merge(newNode, root);
        s++;
    }
    void Clear(){Clear(root);}
    int Size(){return this->s;}
    T Top(){return root->v;}
    void Pop() {
        node *tmp = root;
        root = Merge(root->ll, root->rr);
        delete tmp;
        s--;
    }
    void Merge(LeftistTree<T>& tree) {
        this->root = Merge(root, tree.root);
        s += tree.s;
        tree.root = NULL;
    }
};