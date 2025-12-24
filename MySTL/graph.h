#ifndef GRAPH_H
#define GRAPH_H

#include "stack.h"
#include "queue.h"
#include <climits>

#define hca(x) (fTime(x))

// 修复枚举类型定义，使用具名枚举而非匿名枚举
enum VStatus
{
    UNDISCOVERED,
    DISCOVERED,
    VISITED
};

enum EType
{
    UNDETERMINED,
    TREE,
    CROSS,
    FORWARD,
    BACKWARD
};

template <typename Tv, typename Te>
class Graph
{
private:
    void reset()
    {
        for (Rank v = 0; v < n; v++)
        {
            status(v) = UNDISCOVERED;
            dTime(v) = fTime(v) = -1;
            parent(v) = -1;
            priority(v) = INT_MAX;
            for (Rank u = 0; u < n; u++)
                if (exists(v, u))
                    type(v, u) = UNDETERMINED;
        }
    }
    void BFS(Rank, int&);
    void DFS(Rank, int&);
    void BCC(Rank, int&, Stack<Rank>&);
    bool TSort(Rank, int&, Stack<Tv>*);
    template <typename PU>
    void PFS(Rank, PU);

public:
    int n;
    virtual Rank insert(Tv const&) = 0;
    virtual Tv remove(Rank) = 0;
    virtual Tv& vertex(Rank) = 0;
    virtual int inDegree(Rank) = 0;
    virtual int outDegree(Rank) = 0;
    virtual Rank firstNbr(Rank) = 0;
    virtual Rank nextNbr(Rank, Rank) = 0;
    virtual VStatus& status(Rank) = 0;
    virtual int& dTime(Rank) = 0;
    virtual int& fTime(Rank) = 0;
    virtual Rank& parent(Rank) = 0;
    virtual int& priority(Rank) = 0;
    int e;
    virtual bool exists(Rank, Rank) = 0;
    virtual void insert(Te const&, int, Rank, Rank) = 0;
    virtual Te remove(Rank, Rank) = 0;
    virtual EType& type(Rank, Rank) = 0;
    virtual Te& edge(Rank, Rank) = 0;
    virtual int& weight(Rank, Rank) = 0;

    void bfs(Rank);
    void dfs(Rank);
    void bcc(Rank);
    Stack<Tv>* tSort(Rank);
    void prim(Rank);
    void dijkstra(Rank);
    template <typename PU>
    void pfs(Rank, PU);
};

#endif