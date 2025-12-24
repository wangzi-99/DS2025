#include <iostream>
#include "./MySTL/Stack.h"
#include <string>
#include <cmath>
#include <map>
#include <cctype>
#include "./MySTL/Vector.h"
#include <algorithm>

using namespace std;
using namespace CustomContainer;

class Calculator {
private:
    // 函数名到描述的映射
    map<string, string> Fun;
    // 支持的函数列表
    vector<string> funs = {
        "sqrt", "ln", "log", "sin", "cos", "tan",
        "asin", "acos", "atan", "exp", "abs"
    };

    // 运算符优先级表 [栈顶运算符][当前运算符]
    const char prt[9][9] = {
        /*              +    -    *    /    ^    !    (    )    #  */
        /*  + */    '>', '>', '<', '<', '<', '<', '<', '>', '>',
        /*  - */    '>', '>', '<', '<', '<', '<', '<', '>', '>',
        /*  * */    '>', '>', '>', '>', '<', '<', '<', '>', '>',
        /*  / */    '>', '>', '>', '>', '<', '<', '<', '>', '>',
        /*  ^ */    '>', '>', '>', '>', '>', '<', '<', '>', '>',
        /*  ! */    '>', '>', '>', '>', '>', '>', ' ', '>', '>',
        /*  ( */    '<', '<', '<', '<', '<', '<', '<', '=', ' ',
        /*  ) */    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        /*  # */    '<', '<', '<', '<', '<', '<', '<', ' ', '='
    };

    // 运算符到索引的映射
    map<char, int> opI = {
        {'+', 0}, {'-', 1}, {'*', 2}, {'/', 3},
        {'^', 4}, {'!', 5}, {'(', 6}, {')', 7}, {'#', 8}
    };

    // 初始化函数映射
    void initFuns() {
        Fun["sqrt"] = "平方根函数";
        Fun["ln"] = "自然对数";
        Fun["log"] = "常用对数";
        Fun["sin"] = "正弦函数";
        Fun["cos"] = "余弦函数";
        Fun["tan"] = "正切函数";
        Fun["asin"] = "反正弦函数";
        Fun["acos"] = "反余弦函数";
        Fun["atan"] = "反正切函数";
        Fun["exp"] = "指数函数";
        Fun["abs"] = "绝对值函数";
    }

public:
    Calculator() {
        initFuns();
    }


    char comparePrt(char stackTop, char current) {
        if (opI.find(stackTop) != opI.end() && opI.find(current) != opI.end()) {
            int row = opI[stackTop];
            int col = opI[current];
            return prt[row][col];
        }
        throw runtime_error("未知运算符");
    }


    double calculateSimple(double a, char op, double b) {
        switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) {
                throw runtime_error("除数不能为零");
            }
            return a / b;
        case '^': return pow(a, b);
        default:
            throw runtime_error("未知二元运算符");
        }
    }


    double Factorial(double n) {
        if (n < 0) {
            throw runtime_error("阶乘运算数不能为负");
        }
        if (n != floor(n)) {  // 检查是否为整数
            throw runtime_error("阶乘运算数必须为整数");
        }
        if (n == 0 || n == 1) {
            return 1;
        }
        double result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        return result;
    }


    double calculateFun(const string& func, double value) {
        if (func == "sqrt") {
            if (value < 0) {
                throw runtime_error("平方根参数不能为负");
            }
            return sqrt(value);
        }
        else if (func == "ln") {
            if (value <= 0) {
                throw runtime_error("自然对数参数必须为正");
            }
            return log(value);
        }
        else if (func == "log") {
            if (value <= 0) {
                throw runtime_error("常用对数参数必须为正");
            }
            return log10(value);
        }
        else if (func == "sin") {
            return sin(value);
        }
        else if (func == "cos") {
            return cos(value);
        }
        else if (func == "tan") {
            return tan(value);
        }
        else if (func == "asin") {
            if (value < -1 || value > 1) {
                throw runtime_error("反正弦参数超出[-1,1]范围");
            }
            return asin(value);
        }
        else if (func == "acos") {
            if (value < -1 || value > 1) {
                throw runtime_error("反余弦参数超出[-1,1]范围");
            }
            return acos(value);
        }
        else if (func == "atan") {
            return atan(value);
        }
        else if (func == "exp") {
            return exp(value);
        }
        else if (func == "abs") {
            return fabs(value);
        }
        else {
            throw runtime_error("未知函数: " + func);
        }
    }


    bool isF(const string& func) {
        return find(funs.begin(), funs.end(), func) != funs.end();
    }


    bool isO(char c) {
        return string("+-*/^!()#").find(c) != string::npos;
    }


    string getFunc(const string& expr, size_t& pos) {
        string func;
        while (pos < expr.length() && isalpha(expr[pos])) {
            func += expr[pos];
            pos++;
        }
        return func;
    }


    string getNumber(const string& expr, size_t& pos) {
        string numStr;
        bool hasDecimal = false;
        bool hasExponent = false;

        // 处理负号，作为数字前缀
        if (expr[pos] == '-' && (pos == 0 || isO(expr[pos - 1]) || expr[pos - 1] == 'e' || expr[pos - 1] == 'E')) {
            numStr += expr[pos++];
        }

        while (pos < expr.length()) {
            char c = expr[pos];

            if (isdigit(c)) {
                numStr += c;
            }
            else if (c == '.' && !hasDecimal) {
                numStr += c;
                hasDecimal = true;
            }
            else if ((c == 'e' || c == 'E') && !hasExponent) {
                numStr += c;
                hasExponent = true;
                // 处理指数部分的正负号
                if (pos + 1 < expr.length() && (expr[pos + 1] == '+' || expr[pos + 1] == '-')) {
                    numStr += expr[++pos];
                }
            }
            else {
                break;
            }
            pos++;
        }

        return numStr;
    }


    double DealFunc(const string& expr, size_t& pos) {
        if (pos >= expr.length() || expr[pos] != '(') {
            throw runtime_error("函数调用缺少左括号");
        }
        pos++;  // 跳过左括号

        string paramExpr;
        int bcont = 1;  // 括号计数器(判断是否匹配)
        while (pos < expr.length() && bcont > 0) {
            if (expr[pos] == '(') {
                bcont++;
            }
            else if (expr[pos] == ')') {
                bcont--;
            }

            if (bcont > 0) {
                paramExpr += expr[pos];
            }
            pos++;
        }

        if (bcont != 0) {
            throw runtime_error("括号不匹配");
        }

        return Evaluate(paramExpr + "#");  // 递归计算函数参数表达式
    }


    double Evaluate(const string& expression) {
        // 关键修改：使用自定义Stack替代std::stack
        Stack<char> opStack;       // 运算符栈
        Stack<double> numStack;    // 数字栈

        string expr = expression + "#";  // 添加结束符
        opStack.push('#');

        size_t i = 0;
        while (i < expr.length() && !(expr[i] == '#' && opStack.top() == '#')) {
            if (isspace(expr[i])) {  // 跳过空格
                i++;
                continue;
            }

            // 处理函数
            if (isalpha(expr[i])) {
                size_t start = i;
                string func = getFunc(expr, i);

                if (isF(func)) {
                    double param = DealFunc(expr, i);
                    double result = calculateFun(func, param);
                    numStack.push(result);
                }
                else {
                    i = start;  // 回退位置，准备抛出错误
                    throw runtime_error("未知函数: " + func);
                }
                continue;
            }

            // 处理数字
            if (isdigit(expr[i]) || expr[i] == '.' ||
                (expr[i] == '-' && (i == 0 || isO(expr[i - 1])))) {
                string numStr = getNumber(expr, i);
                try {
                    double num = stod(numStr);
                    numStack.push(num);
                }
                catch (const exception& e) {
                    throw runtime_error("数字格式错误: " + numStr);
                }
                continue;
            }

            // 处理运算符
            if (isO(expr[i])) {
                char currentOp = expr[i];
                char stackTop = opStack.top();
                char rel = comparePrt(stackTop, currentOp);

                switch (rel) {
                case '<':  // 栈顶优先级低，入栈
                    opStack.push(currentOp);
                    i++;
                    break;
                case '=':  // 优先级相等(处理括号匹配)
                    opStack.pop();
                    i++;
                    break;
                case '>': {  // 栈顶优先级高，先计算
                    char op = opStack.top();
                    opStack.pop();

                    if (op == '!') {  // 处理阶乘(单目运算符)
                        if (numStack.empty()) {
                            throw runtime_error("阶乘缺少操作数");
                        }
                        double val = numStack.top();
                        numStack.pop();
                        numStack.push(Factorial(val));
                    }
                    else {  // 处理二元运算符
                        if (numStack.size() < 2) {
                            throw runtime_error("二元运算缺少操作数");
                        }
                        double b = numStack.top(); numStack.pop();
                        double a = numStack.top(); numStack.pop();
                        numStack.push(calculateSimple(a, op, b));
                    }
                    break;
                }
                default:
                    throw runtime_error("表达式语法错误: 无效运算符组合 " +
                        string(1, stackTop) + " 和 " + string(1, currentOp));
                }
            }
            else {
                throw runtime_error("未知字符: " + string(1, expr[i]));
            }
        }

        // 检查最终状态是否合法
        if (numStack.size() != 1 || opStack.top() != '#') {
            throw runtime_error("表达式不完整或语法错误");
        }

        return numStack.top();
    }


    void ActivateEvaluator() {
        string input;
        while (true) {
            cout << "请输入表达式 (输入 'quit' 退出): ";
            getline(cin, input);

            if (input == "quit" || input == "exit") {
                break;
            }
            if (input.empty()) {
                continue;
            }

            try {
                double result = Evaluate(input);
                cout << ">>> 结果: " << result << endl << endl;
            }
            catch (const exception& e) {
                cout << ">>> 错误: " << e.what() << endl << endl;
            }
        }
    }
};

int main() {
    Calculator calculator;
    calculator.ActivateEvaluator();
    return 0;
}