#include "../MySTL/adjacencymatrix.h"
#include "../MySTL/graph.h"
#include <iostream>
#include <iomanip>
using namespace std;
int main()
{
    GraphMatrix<char, int> graph(8, 0);
    graph.vertex(0) = 'A';
    graph.vertex(1) = 'B';
    graph.vertex(2) = 'C';
    graph.vertex(3) = 'D';
    graph.vertex(4) = 'E';
    graph.vertex(5) = 'F';
    graph.vertex(6) = 'G';
    graph.vertex(7) = 'H';

    graph.insert(0, 4, 0, 1);
    graph.insert(0, 6, 0, 3);
    graph.insert(0, 7, 0, 6);
    graph.insert(0, 12, 1, 2);
    graph.insert(0, 9, 2, 3);
    graph.insert(0, 1, 2, 4);
    graph.insert(0, 2, 2, 5);
    graph.insert(0, 10, 2, 7);
    graph.insert(0, 13, 3, 4);
    graph.insert(0, 2, 3, 6);
    graph.insert(0, 5, 4, 5);
    graph.insert(0, 8, 4, 6);
    graph.insert(0, 11, 4, 7);
    graph.insert(0, 3, 5, 7);
    graph.insert(0, 14, 6, 7);

    graph.insert(0, 4, 1, 0);
    graph.insert(0, 6, 3, 0);
    graph.insert(0, 7, 6, 0);
    graph.insert(0, 12, 2, 1);
    graph.insert(0, 9, 3, 2);
    graph.insert(0, 1, 4, 2);
    graph.insert(0, 2, 5, 2);
    graph.insert(0, 10, 7, 2);
    graph.insert(0, 13, 4, 3);
    graph.insert(0, 2, 6, 3);
    graph.insert(0, 5, 5, 4);
    graph.insert(0, 8, 6, 4);
    graph.insert(0, 11, 7, 4);
    graph.insert(0, 3, 7, 5);
    graph.insert(0, 14, 7, 6);

    cout << "邻接矩阵:" << endl;

    cout << setw(4) << " ";
    for (int i = 0; i < 8; i++)
    {
        cout << setw(4) << graph.vertex(i);
    }
    cout << endl;

    for (int i = 0; i < 8; i++)
    {
        cout << setw(4) << graph.vertex(i);
        for (int j = 0; j < 8; j++)
        {
            if (graph.exists(i, j))
            {
                cout << setw(4) << graph.weight(i, j);
            }
            else
            {
                cout << setw(4) << "0";
            }
        }
        cout << endl;
    }
    graph.bfs(0);
    graph.dijkstra(0);
    cout << "\n最短路径结果:" << endl;
    for (int i = 0; i < 8; i++)
    {
        cout << "从A到" << graph.vertex(i)
            << "的最短距离: " << graph.priority(i);
        if (graph.parent(i) != -1)
        {
            cout << ", 父节点: " << graph.vertex(graph.parent(i));
        }
        cout << endl;
    }
    graph.prim(0);
    cout << "\n最小支撑树的边:" << endl;
    for (int i = 0; i < 8; i++)
    {
        if (graph.parent(i) != -1)
        {
            cout << graph.vertex(graph.parent(i)) << " - " << graph.vertex(i)
                << " (权重: " << graph.weight(graph.parent(i), i) << ")" << endl;
        }
    }

    cout << "\n最小支撑树的总权重: ";
    int totalWeight = 0;
    for (int i = 0; i < 8; i++)
    {
        if (graph.parent(i) != -1)
        {
            totalWeight += graph.weight(graph.parent(i), i);
        }
    }
    cout << totalWeight << endl;

    GraphMatrix<char, int> graph2(12, 0);
    graph2.vertex(0) = 'A';
    graph2.vertex(1) = 'B';
    graph2.vertex(2) = 'C';
    graph2.vertex(3) = 'D';
    graph2.vertex(4) = 'E';
    graph2.vertex(5) = 'F';
    graph2.vertex(6) = 'G';
    graph2.vertex(7) = 'H';
    graph2.vertex(8) = 'I';
    graph2.vertex(9) = 'J';
    graph2.vertex(10) = 'K';
    graph2.vertex(11) = 'L';

    graph2.insert(0, 1, 0, 1);
    graph2.insert(0, 1, 0, 4);
    graph2.insert(0, 1, 1, 5);
    graph2.insert(0, 1, 2, 3);
    graph2.insert(0, 1, 2, 5);
    graph2.insert(0, 1, 2, 7);
    graph2.insert(0, 1, 3, 7);
    graph2.insert(0, 1, 4, 5);
    graph2.insert(0, 1, 4, 8);
    graph2.insert(0, 1, 5, 6);
    graph2.insert(0, 1, 5, 8);
    graph2.insert(0, 1, 5, 9);
    graph2.insert(0, 1, 5, 10);
    graph2.insert(0, 1, 6, 10);
    graph2.insert(0, 1, 9, 10);
    graph2.insert(0, 1, 10, 11);

    graph2.insert(0, 1, 1, 0);
    graph2.insert(0, 1, 4, 0);
    graph2.insert(0, 1, 5, 1);
    graph2.insert(0, 1, 3, 2);
    graph2.insert(0, 1, 5, 2);
    graph2.insert(0, 1, 7, 2);
    graph2.insert(0, 1, 7, 3);
    graph2.insert(0, 1, 5, 4);
    graph2.insert(0, 1, 8, 4);
    graph2.insert(0, 1, 6, 5);
    graph2.insert(0, 1, 8, 5);
    graph2.insert(0, 1, 9, 5);
    graph2.insert(0, 1, 10, 5);
    graph2.insert(0, 1, 10, 6);
    graph2.insert(0, 1, 10, 9);
    graph2.insert(0, 1, 11, 10);
    cout << "邻接矩阵:" << endl;

    cout << setw(4) << " ";
    for (int i = 0; i < 12; i++)
    {
        cout << setw(4) << graph2.vertex(i);
    }
    cout << endl;
    for (int i = 0; i < 12; i++)
    {
        cout << setw(4) << graph2.vertex(i);
        for (int j = 0; j < 12; j++)
        {
            if (graph2.exists(i, j))
            {
                cout << setw(4) << graph2.weight(i, j);
            }
            else
            {
                cout << setw(4) << "0";
            }
        }
        cout << endl;
    }

    graph2.bcc(6);
    graph2.bcc(4);
    cout << "结果不一致" << endl;
    return 0;
}