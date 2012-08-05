/* minimum heap */
using namespace std;
template<typename T>
class heap
{
public:
#define RR(i) (((i) * 2) ^ 1)
#define LL(i) (((i) * 2))
#define PARENT(i) ((i) >> 1)
    static const int MAXSIZE = 100001;
    T A[MAXSIZE];
    int s;
    heap(){s = 0;}
    T top(){return A[1];}
    int size(){return s;}
    void insert(T e)
    {
        A[++s] = e;
        maintain(s);
    }
    void pop()
    {
        this->swap(A[s], A[1]);
        s--;heapify(1);
    }
private:
    void swap(T& a, T& b)
    {
        T tmp = a;a = b;b = tmp;
    }
    void heapify(int pos)
    {
        int l = LL(pos),r = RR(pos),smallest = pos;
        if(l <= s && A[l] < A[pos])
            smallest = l;
        if(r <= s && A[r] < A[smallest])
            smallest = r;
        if(smallest != pos)
        {
            swap(A[smallest], A[pos]);
            heapify(smallest);
        }
    }
    void maintain(int pos)
    {
        while(pos > 1 && A[pos] < A[PARENT(pos)])
        {
            swap(A[PARENT(pos)], A[pos]);
            pos = PARENT(pos);
        }
    }
};
 

