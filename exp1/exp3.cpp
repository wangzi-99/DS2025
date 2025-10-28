#include <iostream>
#include "./MySTL/Vector.h"
#include <cstdlib>
#include <ctime>
using namespace std;

// 计算最大矩形面积
int maxArea(const vector<int>& h) {
    int n = h.size();
    if (n == 0) return 0;

    vector<int> l(n), r(n); // 左右边界数组

    // 左边界：左侧第一个更矮的柱子索引
    l[0] = -1;
    for (int i = 1; i < n; i++) {
        int t = i - 1;
        while (t >= 0 && h[t] >= h[i]) t = l[t];
        l[i] = t;
    }

    // 右边界：右侧第一个不高于当前的柱子索引
    r[n - 1] = n;
    for (int i = n - 2; i >= 0; i--) {
        int t = i + 1;
        while (t < n && h[t] >= h[i]) t = r[t];
        r[i] = t;
    }

    // 计算最大面积
    int res = 0;
    for (int i = 0; i < n; i++) {
        res = max(res, h[i] * (r[i] - l[i] - 1));
    }
    return res;
}

// 生成随机高度数组
vector<int> randH() {
    int len = rand() % 100000 + 1; 
    vector<int> h(len);
    for (int i = 0; i < len; i++) {
        h[i] = rand() % 10001;
    }
    return h;
}

int main() {
    srand(time(0));
    for (int i = 1; i <= 10; i++) {
        cout << "测试用例 " << i << ": " << maxArea(randH()) << endl;
    }
    return 0;
}