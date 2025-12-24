#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Test function with a known example
int main() {
    // Simple test case: [2, 1, 5, 6, 2, 3]
    // Expected max area should be 10 (rectangle of height 5 and width 2)
    vector<int> test = {2, 1, 5, 6, 2, 3};
    
    int n = test.size();
    if (n == 0) {
        cout << "Max area: 0" << endl;
        return 0;
    }

    vector<int> left(n), right(n);
    vector<int> stack; // Use standard vector for test
    
    // Calculate left boundaries
    for (int i = 0; i < n; i++) {
        while (!stack.empty() && test[stack[stack.size() - 1]] >= test[i]) {
            stack.pop_back();
        }
        left[i] = stack.empty() ? -1 : stack[stack.size() - 1];
        stack.push_back(i);
    }
    
    stack.clear();
    
    // Calculate right boundaries
    for (int i = n - 1; i >= 0; i--) {
        while (!stack.empty() && test[stack[stack.size() - 1]] >= test[i]) {
            stack.pop_back();
        }
        right[i] = stack.empty() ? n : stack[stack.size() - 1];
        stack.push_back(i);
    }

    // Calculate max area
    int res = 0;
    for (int i = 0; i < n; i++) {
        res = max(res, test[i] * (right[i] - left[i] - 1));
    }
    
    cout << "Test case [2, 1, 5, 6, 2, 3] max area: " << res << endl;
    return 0;
}