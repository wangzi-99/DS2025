#include <iostream>
#include <vector>  // Using standard library vector
#include <cstdlib>
#include <ctime>
using namespace std;

// 计算最大矩形面积 - 使用数组实现栈
int maxArea(const int* heights, int n) {
    if (n == 0) return 0;

    int* left = new int[n];  // 左边界数组
    int* right = new int[n]; // 右边界数组
    int* stack = new int[n]; // 用数组模拟栈
    int top = -1;            // 栈顶指针

    // 计算左边界：找到每个位置左边第一个更小的元素
    for (int i = 0; i < n; i++) {
        while (top >= 0 && heights[stack[top]] >= heights[i]) {
            top--;
        }
        left[i] = (top == -1) ? -1 : stack[top];
        stack[++top] = i;
    }

    // 重置栈
    top = -1;

    // 计算右边界：找到每个位置右边第一个更小的元素
    for (int i = n - 1; i >= 0; i--) {
        while (top >= 0 && heights[stack[top]] >= heights[i]) {
            top--;
        }
        right[i] = (top == -1) ? n : stack[top];
        stack[++top] = i;
    }

    // 计算最大面积
    int res = 0;
    for (int i = 0; i < n; i++) {
        int area = heights[i] * (right[i] - left[i] - 1);
        if (area > res) res = area;
    }

    // 释放动态分配的内存
    delete[] left;
    delete[] right;
    delete[] stack;

    return res;
}

// 包装函数，用于兼容vector接口
int maxArea(const vector<int>& heights) {
    int n = heights.size();
    if (n == 0) return 0;
    
    int* arr = new int[n];
    for (int i = 0; i < n; i++) {
        arr[i] = heights[i];
    }
    
    int result = maxArea(arr, n);
    
    delete[] arr;
    return result;
}

// 生成随机高度数组
vector<int> generateRandomHeights() {
    int length = rand() % 100 + 1; // Using smaller size
    vector<int> heights(length);
    for (int i = 0; i < length; i++) {
        heights[i] = rand() % 10001;
    }
    return heights;
}

int main() {
    srand(time(0));
    for (int i = 1; i <= 10; i++) {
        cout << "测试用例 " << i << ": " << maxArea(generateRandomHeights()) << endl;
    }
    return 0;
}