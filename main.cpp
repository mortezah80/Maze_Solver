#include<iostream>
#include <stdexcept>
using namespace std;



template <typename T> class TwoThreeTree;

template <typename T>
class TwoThreeNode
{
    friend class TwoThreeTree<T>;
public:
    TwoThreeNode();
    bool isLeaf() const;
    bool isFull() const;
    void setKey(unsigned int, T);
    T getKey(unsigned int) const;
    void setKeyNum(unsigned int);
    void shrink();
    void setChild(unsigned int, TwoThreeNode<T>*);
    TwoThreeNode<T>* getChild(unsigned int) const;
    void insertKey(T, TwoThreeNode<T>* = nullptr);
    TwoThreeNode<T>* getParent() const;
    void setParent(TwoThreeNode<T>*);

private:
    T key[2];
    unsigned int keyNum;
    TwoThreeNode<T> *child[3];
    TwoThreeNode<T> *parent;
};

template <typename T>
TwoThreeNode<T>::TwoThreeNode()
{
	keyNum = 0;
	child[0] = child[1] = child[2] = parent = nullptr;
}

template <typename T>
void TwoThreeNode<T>::setKey(unsigned int idx, T k)
{
    if(idx > 1 || (idx == 1 && keyNum == 0))
        throw invalid_argument("Invalid index to set key");
    key[idx] = k;
    if(idx == keyNum)
        keyNum = idx + 1;
}

template <typename T>
void TwoThreeNode<T>::setKeyNum(unsigned int n)
{
    if(n > 2)
        throw invalid_argument("Invalid index to set nuumber of key");
    keyNum = n;
}

template <typename T>
void TwoThreeNode<T>::shrink()
{
    if(isFull())
    {
        setChild(2, nullptr);
        setKeyNum(1);
    }
}

template <typename T>
T TwoThreeNode<T>::getKey(unsigned int idx) const
{
    if(idx >= keyNum)
        throw invalid_argument("Invalid index to get key");
    return key[idx];
}

template <typename T>
void TwoThreeNode<T>::setChild(unsigned int idx, TwoThreeNode<T> *p)
{
    if(idx > 2)
        throw invalid_argument("Invalid index to set child");
    if(p != nullptr)
    {
        if(idx == 2 && keyNum < 2)
            throw invalid_argument("Invalid index to set child");
        if(keyNum == 0)
            throw invalid_argument("Invalid index to set child");
    }
    child[idx] = p;
    if(p)
        p->setParent(this);
}

template <typename T>
TwoThreeNode<T>* TwoThreeNode<T>::getChild(unsigned int idx) const
{
    if(idx > keyNum)
        throw invalid_argument("Invalid index to get child");
    return child[idx];
}

template <typename T>
void TwoThreeNode<T>::insertKey(T key, TwoThreeNode<T> *newChild)
{
    if(isFull())
        throw out_of_range("Insertion into a full node.");
    if(key < getKey(0))
    {
        setKey(1, getKey(0));
        setChild(2, getChild(1));

        setKey(0, key);
        setChild(1, newChild);
    }
    else
    {
        setKey(1, key);
        setChild(2, newChild);
    }
}

template <typename T>
TwoThreeNode<T>* TwoThreeNode<T>::getParent() const
{
    return parent;
}

template <typename T>
void TwoThreeNode<T>::setParent(TwoThreeNode<T> *p)
{
    parent = p;
}

template <typename T>
bool TwoThreeNode<T>::isLeaf() const
{
    return !child[0] && !child[1] && !child[2];
}

template <typename T>
bool TwoThreeNode<T>::isFull() const
{
    return keyNum == 2;
}



template <typename T>
class TwoThreeTree
{
public:
    TwoThreeTree() : root(nullptr) { }
    bool isEmpty() { return root == nullptr; }
    void insert(T);
    void sortedOrder();
    bool searchnode(int );

private:
    TwoThreeNode<T> *root;
    void split(TwoThreeNode<T>*, T, TwoThreeNode<T>*);
    void sortedOrder(TwoThreeNode<T> *);
    void preorder(TwoThreeNode<T> *);
};

template <typename T>
void TwoThreeTree<T>::insert(T key)
{
    if(isEmpty())
    {
        root = new TwoThreeNode<T>;
        root->setKey(0, key);
        return;
    }

    TwoThreeNode<T> *p = root;
    while(!p->isLeaf())
    {
        if(key < p->getKey(0))
            p = p->getChild(0);
        else if(!p->isFull() || key < p->getKey(1))
            p = p->getChild(1);
        else
            p = p->getChild(2);
    }

    if(p->isFull())
        split(p, key, nullptr);
    else
        p->insertKey(key);

}

template <typename T>
void TwoThreeTree<T>::split(TwoThreeNode<T> *p, T key, TwoThreeNode<T> *newChild)
{
    TwoThreeNode<T> *newNode = new TwoThreeNode<T>;
    TwoThreeNode<T> *parent = (p == root ? new TwoThreeNode<T> : p->getParent());

    T min = p->getKey(0), mid = p->getKey(1), max;
    if(key < min)
    {
        max = mid;
        mid = min;
        min = key;
    }
    else if( key < mid)
    {
        max = mid;
        mid = key;
    }
    else
        max = key;

    p->setKey(0, min);
    newNode->setKey(0, max);
    if(!p->isLeaf())
    {
        if(key == max)
        {
            newNode->setChild(0, p->getChild(2));
            newNode->setChild(1, newChild);
        }
        else if(key == mid)
        {
            newNode->setChild(1, p->getChild(2));
            newNode->setChild(0, newChild);
        }
        else
        {
            newNode->setChild(1, p->getChild(2));
            newNode->setChild(0, p->getChild(1));
            p->setChild(1, newChild);
        }
    }
    p->shrink();


    if(p == root)
    {
        parent->setKey(0, mid);
        parent->setChild(0, p);
        parent->setChild(1, newNode);
        root = parent;
    }
    else if(parent->isFull())
        split(parent, mid, newNode);
    else
        parent->insertKey(mid, newNode);
}

template <typename T>
void TwoThreeTree<T>::sortedOrder(TwoThreeNode<T> *r)
{
    if(!r->isLeaf())
        sortedOrder(r->getChild(0));
	cout << r->getKey(0) << "  ";
	if(!r->isLeaf())
        sortedOrder(r->getChild(1));
	if(r->isFull())
    {
        cout << r->getKey(1) << "  ";
        if(!r->isLeaf())
            sortedOrder(r->getChild(2));
    }
}

template <typename T>
void TwoThreeTree<T>::sortedOrder()
{
    sortedOrder(root);
}



template <typename T>
bool TwoThreeTree<T>::searchnode(int k)
{
    TwoThreeNode<T>* temp=root;
    while(temp)
    {
        if(k==temp->key[0] || k==temp->key[1])
        {
            return true;
        }
        else if(k<temp->key[0])
        {
            temp=temp->child[0];
        }
        else if(k>temp->key[0] && (k<temp->key[1] || temp->keyNum==1))
        {
            temp=temp->child[1];
        }
        else if(k>temp->key[1])
        {
            temp=temp->child[2];
        }
    }
    return false;
}


TwoThreeTree<int> mytree;



const int MAXN = 1000 + 10;
const int INF = 1000*1000;
int start;
int endpos;
int tedad=0;
class vertex
{
public:
    vertex(){up=down=right=left=INF;}
    int up;
    int down;
    int right;
    int left;
};

int n, m;
vertex adj[1000*1000+10];
int firstmatrix[MAXN][MAXN];
int dist[MAXN];
int vertexto[MAXN];

void readInput(){
	cin >> n >> m;
	int z;
	for(int i=0; i<n; ++i)
    {
        for(int j=0;j<m;j++)
        {
            cin>>z;
            firstmatrix[i][j] = z;
        }


    }
    for(int j=0;j<m;j++)
    {
        if (firstmatrix[0][j]!=0)
        {
            start=j;
            break;
        }
    }
    for(int j=0;j<m;j++)
    {
        if(firstmatrix[n-1][j]!=0)
        {
            endpos=((n-1)*m)+j;
        }
    }

}

void dijkstra(int s){
	dist[s] = 0;
	for(int i=0; i<n*m; i++)
		if(i!=s)
			dist[i] = INF;
    for(int i=0;i<n*m;i++)
    {
        vertexto[i]=-1;
    }
	for(int rep=0; rep<tedad; ++rep)
	{
		int u = -1;
		int du = INF;
		for(int i=0; i<n*m; ++i)
			if(!mytree.searchnode(i) && dist[i] <= du){
				u = i;
				du = dist[i];
			}
		mytree.insert(u);

        if(adj[u].right!=INF)
        {
            if(dist[u]+adj[u].right < dist[u+1])
            {
                dist[u+1]=dist[u]+adj[u].right;
                vertexto[u+1]=u;
            }
        }
        if(adj[u].left!=INF)
        {
            if(dist[u]+adj[u].left < dist[u-1])
            {
                dist[u-1]=dist[u]+adj[u].left;
                vertexto[u-1]=u;
            }
        }
        if(adj[u].down!=INF)
        {
            if(dist[u]+adj[u].down < dist[u+m])
            {
                dist[u+m]=dist[u]+adj[u].down;
                vertexto[u+m]=u;
            }
        }
        if(adj[u].up!=INF)
        {
            if(dist[u]+adj[u].up < dist[u-m])
            {
                dist[u-m]=dist[u]+adj[u].up;
                vertexto[u-m]=u;
            }
        }
	}
}


void writeOutput(){
	for(int i=0; i<n; ++i)
    {
        for(int j=0;j<m;j++)
        {
                cout<<firstmatrix[i][j]<<" ";
        }
        cout<<endl;
    }
}


void makeadj()
{
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            if(firstmatrix[i][j]!=0)
            {
                if(j!=m-1)
                {
                    if(firstmatrix[i][j+1]!=0)
                    {
                        adj[(i*m)+j].right=firstmatrix[i][j+1];
                    }
                }
                if(j!=0)
                {
                    if(firstmatrix[i][j-1]!=0)
                    {
                        adj[(i*m)+j].left=firstmatrix[i][j-1];
                    }
                }
                if(i!=n-1)
                {
                    if(firstmatrix[i+1][j]!=0)
                    {
                        adj[(i*m)+j].down=firstmatrix[i+1][j];
                    }
                }
                if(i!=0)
                {
                    if(firstmatrix[i-1][j]!=0)
                    {
                        adj[(i*m)+j].up=firstmatrix[i-1][j];
                    }
                }

            }
        }
    }
}


void ctedad()
{
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            if(firstmatrix[i][j]!=0)
            {
                tedad++;
            }
        }
    }
}



void makematrixnew(int n)
{
    if(n!=-1)
    {
        firstmatrix[n/m][n%m]=-1;
        makematrixnew(vertexto[n]);
    }
}

int main(){

	readInput();
	makeadj();
	ctedad();
	dijkstra(start);
    makematrixnew(endpos);
    cout<<endl<<endl;
	writeOutput();
	return 0;
}
