#include <iostream>
#include "../MySTL/Vector.h"
#include <cstdlib>
#include <ctime>
using namespace std;

// �������������
int maxArea(const vector<int>& h) {
    int n = h.size();
    if (n == 0) return 0;

    vector<int> l(n), r(n); // ���ұ߽�����

    // ��߽磺����һ����������������
    l[0] = -1;
    for (int i = 1; i < n; i++) {
        int t = i - 1;
        while (t >= 0 && h[t] >= h[i]) t = l[t];
        l[i] = t;
    }

    // �ұ߽磺�Ҳ��һ�������ڵ�ǰ����������
    r[n - 1] = n;
    for (int i = n - 2; i >= 0; i--) {
        int t = i + 1;
        while (t < n && h[t] >= h[i]) t = r[t];
        r[i] = t;
    }

    // ����������
    int res = 0;
    for (int i = 0; i < n; i++) {
        res = max(res, h[i] * (r[i] - l[i] - 1));
    }
    return res;
}

// ��������߶�����
vector<int> randH() {
    int len = rand() % 100 + 1;  // Reduced size to avoid issues
    vector<int> h(len, len);     // Initialize with size=capacity=len
    for (int i = 0; i < len; i++) {
        h[i] = rand() % 10001;
    }
    return h;
}

int main() {
    srand(time(0));
    for (int i = 1; i <= 10; i++) {
        cout << "�������� " << i << ": " << maxArea(randH()) << endl;
    }
    return 0;
}