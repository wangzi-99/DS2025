#ifndef ADJACENCYMATRIX_H
#define ADJACENCYMATRIX_H

#include "vector.h"
#include "graph.h"

template <typename Tv>
struct Vertex
{
    Tv data;
    int inDegree, outDegree;
    VStatus status;
    int dTime, fTime;
    Rank parent;
    int priority;
    Vertex(Tv const& d = (Tv)0) : data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
        dTime(-1), fTime(-1), parent(-1), priority(INT_MAX)
    {
    }
};

template <typename Te>
struct Edge
{
    Te data;
    int weight;
    EType type;
    Edge(Te const& d, int w) : data(d), weight(w), type(UNDETERMINED) {}
};

template <typename Tv, typename Te>
class GraphMatrix : public Graph<Tv, Te>
{
private:
    Vector<Vertex<Tv>> V;
    Vector<Vector<Edge<Te>*>> E;

public:
    // 构造和析构函数
    GraphMatrix()
    {
        this->n = this->e = 0;
    }

    GraphMatrix(int n)
    {
        this->n = n;
        this->e = 0;

        for (int i = 0; i < n; i++)
        {
            V.insert(Vertex<Tv>());
        }

        for (int i = 0; i < n; i++)
        {
            E.insert(Vector<Edge<Te> *>(n, n, (Edge<Te> *)NULL));
        }
    }

    GraphMatrix(int n, int e)
    {
        this->n = n;
        this->e = e;

        for (int i = 0; i < n; i++)
        {
            V.insert(Vertex<Tv>());
        }

        for (int i = 0; i < n; i++)
        {
            E.insert(Vector<Edge<Te> *>(n, n, (Edge<Te> *)NULL));
        }
    }

    ~GraphMatrix()
    {
        for (Rank v = 0; v < this->n; v++)
            for (Rank u = 0; u < this->n; u++)
                delete E[v][u];
    }

    virtual Tv& vertex(Rank v) override
    {
        return V[v].data;
    }

    virtual int inDegree(Rank v) override
    {
        return V[v].inDegree;
    }

    virtual int outDegree(Rank v) override
    {
        return V[v].outDegree;
    }

    virtual Rank firstNbr(Rank v) override
    {
        return nextNbr(v, this->n);
    }

    virtual Rank nextNbr(Rank v, Rank u) override
    {
        while ((-1 < u) && (!exists(v, --u)))
            ;
        return u;
    }

    virtual VStatus& status(Rank v) override
    {
        return V[v].status;
    }

    virtual int& dTime(Rank v) override
    {
        return V[v].dTime;
    }

    virtual int& fTime(Rank v) override
    {
        return V[v].fTime;
    }

    virtual Rank& parent(Rank v) override
    {
        return V[v].parent;
    }

    virtual int& priority(Rank v) override
    {
        return V[v].priority;
    }

    virtual bool exists(Rank v, Rank u) override
    {
        return (v < this->n) && (u < this->n) && E[v][u] != NULL;
    }

    virtual EType& type(Rank v, Rank u) override
    {
        return E[v][u]->type;
    }

    virtual Te& edge(Rank v, Rank u) override
    {
        return E[v][u]->data;
    }

    virtual int& weight(Rank v, Rank u) override
    {
        return E[v][u]->weight;
    }

    virtual Rank insert(Tv const& vertex)
    {
        for (Rank u = 0; u < this->n; u++)
            E[u].insert(NULL);
        this->n++;
        E.insert(Vector<Edge<Te> *>(this->n, this->n, (Edge<Te> *)NULL));
        return V.insert(Vertex<Tv>(vertex));
    }

    virtual Tv remove(Rank v)
    {
        for (Rank u = 0; u < this->n; u++)
            if (exists(v, u))
            {
                delete E[v][u];
                V[u].inDegree--;
                this->e--;
            }
        E.remove(v);
        this->n--;
        Tv vBak = vertex(v);
        V.remove(v);
        for (Rank u = 0; u < this->n; u++)
            if (Edge<Te>* x = E[u].remove(v))
            {
                delete x;
                V[u].outDegree--;
                this->e--;
            }
        return vBak;
    }

    virtual void insert(Te const& edge, int w, Rank v, Rank u)
    {
        if (v < 0 || u > this->n - 1)
        {
            cout << "插入有误！" << endl;
            return;
        }
        E[v][u] = new Edge<Te>(edge, w);
        this->e++;
        V[v].outDegree++;
        V[u].inDegree++;
    }

    virtual Te remove(Rank v, Rank u)
    {
        Te eBak = edge(v, u);
        delete E[v][u];
        E[v][u] = NULL;
        this->e--;
        V[v].outDegree--;
        V[u].inDegree--;
        return eBak;
    }
};

template <typename Tv, typename Te>
void Graph<Tv, Te>::bfs(Rank s)
{
    reset();
    int clock = 0;
    Rank v = s;
    do
        if (UNDISCOVERED == status(v))
            BFS(v, clock);
    while (s != (v = ((v + 1) % n)));
    cout << "广度优先搜索BFS算法执行成功!" << endl;
}
template <typename Tv, typename Te>
void Graph<Tv, Te>::BFS(Rank v, int& clock)
{
    Queue<Rank> Q;
    status(v) = DISCOVERED;
    Q.enqueue(v);
    dTime(v) = clock;
    clock = 0;
    while (!Q.empty())
    {
        Rank v = Q.dequeue();
        for (Rank u = firstNbr(v); -1 < u; u = nextNbr(v, u))
            if (UNDISCOVERED == status(u))
            {
                status(u) = DISCOVERED;
                Q.enqueue(u);
                dTime(u) = dTime(v) + 1;
                type(v, u) = TREE;
                parent(u) = v;
            }
            else
            {
                type(v, u) = CROSS;
            }
        status(v) = VISITED;
        fTime(v) = clock++;
        if (Q.empty())
            clock = dTime(v) + 1;
        else if (dTime(v) < dTime(Q.front()))
            clock = 0;
    }
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::dfs(Rank s)
{ // s < n
    reset();
    int clock = 0;
    Rank v = s;
    do
        if (UNDISCOVERED == status(v))
            DFS(v, clock);
    while (s != (v = ((v + 1) % n)));
    cout << "广度优先搜索DFS算法执行成功!" << endl;
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::DFS(Rank v, int& clock)
{ // v < n
    dTime(v) = ++clock;
    status(v) = DISCOVERED;
    for (Rank u = firstNbr(v); -1 < u; u = nextNbr(v, u))
        switch (status(u))
        {
        case UNDISCOVERED:
            type(v, u) = TREE;
            parent(u) = v;
            DFS(u, clock);
            break;
        case DISCOVERED:
            type(v, u) = BACKWARD;
            break;
        default:
            type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
            break;
        }
    status(v) = VISITED;
    fTime(v) = ++clock;
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::BCC(Rank v, int& clock, Stack<Rank>& S)
{ // assert: 0 <= v < n
    hca(v) = dTime(v) = ++clock;
    status(v) = DISCOVERED;
    S.push(v);
    for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u))
        switch (status(u))
        {
        case UNDISCOVERED:
            parent(u) = v;
            type(v, u) = TREE;
            BCC(u, clock, S);
            if (hca(u) < dTime(v))
                hca(v) = min(hca(v), hca(u));
            else
            {
                cout << "BCC的根节点是:" << vertex(v);
                Stack<int> temp;
                do
                {
                    temp.push(S.pop());
                    // cout << vertex(temp.top());
                } while (u != temp.top());
                // cout << vertex(parent(u));
                while (!temp.empty())
                    S.push(temp.pop());
                while (u != S.pop())
                    ;
                cout << endl;
            }
            break;
        case DISCOVERED:
            type(v, u) = BACKWARD;
            if (u != parent(v))
                hca(v) = min(hca(v), dTime(u));
            break;
        default:
            type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
            break;
        }
    status(v) = VISITED;
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::bcc(Rank s)
{
    reset();
    int clock = 0;
    int v = s;
    Stack<int> S;
    do
        if (UNDISCOVERED == status(v))
        {
            BCC(v, clock, S);
            S.pop();
        }
    while (s != (v = (++v % n)));
    cout << "成功调用了基于DFS的BCC分解算法!" << endl;
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::dijkstra(Rank s)
{
    reset();
    priority(s) = 0;
    for (int i = 0; i < n; i++)
    {
        status(s) = VISITED;
        if (-1 < parent(s))
            type(parent(s), s) = TREE;
        for (Rank j = firstNbr(s); -1 < j; j = nextNbr(s, j))
            if ((status(j) == UNDISCOVERED) && (priority(j) > priority(s) + weight(s, j)))
            {
                priority(j) = priority(s) + weight(s, j);
                parent(j) = s;
            }
        for (int shortest = INT_MAX, j = 0; j < n; j++)
            if ((status(j) == UNDISCOVERED) && (shortest > priority(j)))
            {
                shortest = priority(j);
                s = j;
            }
    }
    cout << "dijkstra算法运行成功！" << endl;
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::prim(Rank s)
{
    reset();
    priority(s) = 0;
    for (int i = 0; i < n; i++)
    {
        status(s) = VISITED;
        if (-1 < parent(s))
            type(parent(s), s) = TREE;
        for (Rank j = firstNbr(s); -1 < j; j = nextNbr(s, j))
            if ((status(j) == UNDISCOVERED) && (priority(j) > weight(s, j)))
            {
                priority(j) = weight(s, j);
                parent(j) = s;
            }
        for (int shortest = INT_MAX, j = 0; j < n; j++)
            if ((status(j) == UNDISCOVERED) && (shortest > priority(j)))
            {
                shortest = priority(j);
                s = j;
            }
    }
    cout << "Prim算法运行成功！" << endl;
}

template <typename Tv, typename Te>
Stack<Tv>* Graph<Tv, Te>::tSort(Rank s)
{ // assert: 0 <= s < n
    reset();
    int clock = 0;
    Rank v = s;
    Stack<Tv>* S = new Stack<Tv>;
    do
    {
        if (UNDISCOVERED == status(v))
            if (!TSort(v, clock, S))
            {
                while (!S->empty())
                    S->pop();
                break;
            }
    } while (s != (v = (++v % n)));
    cout << "基于DFS的拓扑排序算法运行成功！" << endl;
    return S;
}

template <typename Tv, typename Te>
bool Graph<Tv, Te>::TSort(Rank v, int& clock, Stack<Tv>* S)
{
    dTime(v) = ++clock;
    status(v) = DISCOVERED;
    for (Rank u = firstNbr(v); -1 < u; u = nextNbr(v, u))
        switch (status(u))
        {
        case UNDISCOVERED:
            parent(u) = v;
            type(v, u) = TREE;
            if (!TSort(u, clock, S))
                return false;
            break;
        case DISCOVERED:
            type(v, u) = BACKWARD;
            return false;
        default:
            type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
            break;
        }
    status(v) = VISITED;
    S->push(vertex(v));
    return true;
}

template <typename Tv, typename Te>
template <typename PU>
void Graph<Tv, Te>::pfs(Rank s, PU prioUpdater)
{
    reset();
    Rank v = s;
    do
        if (UNDISCOVERED == status(v))
            PFS(v, prioUpdater);
    while (s != (v = (++v % n)));
}

template <typename Tv, typename Te>
template <typename PU>
void Graph<Tv, Te>::PFS(int s, PU prioUpdater)
{
    priority(s) = 0;
    status(s) = VISITED;
    parent(s) = -1;
    while (1)
    {
        for (int w = firstNbr(s); -1 < w; w = nextNbr(s, w))
        {
            prioUpdater(this, s, w);
        }
        for (int shortest = INT_MAX, w = 0; w < n; w++)
        {
            if (UNDISCOVERED == status(w))
                if (shortest > priority(w))
                {
                    shortest = priority(w);
                    s = w;
                }
        }
        if (VISITED == status(s))
            break;
        status(s) = VISITED;
        type(parent(s), s) = TREE;
    }
}

template <typename Tv, typename Te>
struct BfsPu
{
    virtual void operator()(Graph<Tv, Te>* g, int uk, int v)
    {
        if (g->status(v) == UNDISCOVERED)
            if (g->priority(v) > g->priority(uk) + 1)
            {
                g->priority(v) = g->priority(uk) + 1;
                g->parent(v) = uk;
            }
    }
    BfsPu()
    {
        cout << "成功调用了pfs算法对应的BFS优先级更新器!" << endl;
    }
};

template <typename Tv, typename Te>
struct DfsPu
{
    virtual void operator()(Graph<Tv, Te>* g, int uk, int v)
    {
        if (g->status(v) == UNDISCOVERED)
            if (g->priority(v) > g->priority(uk) - 1)
            {
                g->priority(v) = g->priority(uk) - 1;
                g->parent(v) = uk;
            }
    }
    DfsPu()
    {
        cout << "成功调用了pfs算法对应的DFS优先级更新器!" << endl;
    }
};

template <typename Tv, typename Te>
struct DijkPU
{
    virtual void operator()(Graph<Tv, Te>* g, Rank v, Rank u)
    {
        if (UNDISCOVERED == g->status(u))
            if (g->priority(u) > g->priority(v) + g->weight(v, u))
            {
                g->priority(u) = g->priority(v) + g->weight(v, u);
                g->parent(u) = v;
            }
    }
    DijkPU()
    {
        cout << "成功调用了pfs算法对应的dijkstra优先级更新器!" << endl;
    }
};

template <typename Tv, typename Te>
struct PrimPU
{
    virtual void operator()(Graph<Tv, Te>* g, Rank v, Rank u)
    {
        if (UNDISCOVERED == g->status(u))
            if (g->priority(u) > g->weight(v, u))
            {
                g->priority(u) = g->weight(v, u);
                g->parent(u) = v;
            }
    }
    PrimPU()
    {
        cout << "成功调用了pfs算法对应的prim优先级更新器!" << endl;
    }
};

#endif