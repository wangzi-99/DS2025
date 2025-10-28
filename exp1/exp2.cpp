#include <iostream>
#include "./MySTL/Stack.h"
#include <string>
#include <cmath>
#include <map>
#include <cctype>
#include "./MySTL/Vector.h"
#include <algorithm>

using namespace std;

class Calculator {
private:
    // 函数名称到描述的映射
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

    // 初始化函数描述
    void initFuns() {
        Fun["sqrt"] = "计算平方根";
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

    /**
     * 比较两个运算符的优先级
     * @param stackTop 栈顶运算符
     * @param current 当前运算符
     * @return 优先级关系（<, =, >）
     */
    char comparePrt(char stackTop, char current) {
        if (opI.find(stackTop) != opI.end() && opI.find(current) != opI.end()) {
            int row = opI[stackTop];
            int col = opI[current];
            return prt[row][col];
        }
        throw runtime_error("未知运算符");
    }

    /**
     * 执行二元运算
     * @param a 左操作数
     * @param op 运算符
     * @param b 右操作数
     * @return 运算结果
     */
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

    /**
     * 计算阶乘（单目运算）
     * @param n 操作数
     * @return 阶乘结果
     */
    double Factorial(double n) {
        if (n < 0) {
            throw runtime_error("阶乘函数参数不能为负");
        }
        if (n != floor(n)) {  // 检查是否为整数
            throw runtime_error("阶乘函数参数必须为整数");
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

    /**
     * 执行数学函数计算
     * @param func 函数名
     * @param value 函数参数
     * @return 计算结果
     */
    double calculateFun(const string& func, double value) {
        if (func == "sqrt") {
            if (value < 0) {
                throw runtime_error("平方根函数参数不能为负");
            }
            return sqrt(value);
        }
        else if (func == "ln") {
            if (value <= 0) {
                throw runtime_error("自然对数函数参数必须为正");
            }
            return log(value);
        }
        else if (func == "log") {
            if (value <= 0) {
                throw runtime_error("常用对数函数参数必须为正");
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
                throw runtime_error("反正弦函数参数必须在[-1,1]范围内");
            }
            return asin(value);
        }
        else if (func == "acos") {
            if (value < -1 || value > 1) {
                throw runtime_error("反余弦函数参数必须在[-1,1]范围内");
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

    /**
     * 检查是否为支持的函数
     * @param func 函数名
     * @return 是否支持
     */
    bool isF(const string& func) {
        return find(funs.begin(), funs.end(), func) != funs.end();
    }

    /**
     * 检查是否为运算符
     * @param c 字符
     * @return 是否为运算符
     */
    bool isO(char c) {
        return string("+-*/^!()#").find(c) != string::npos;
    }

    /**
     * 从表达式中提取函数名
     * @param expr 表达式
     * @param pos 当前位置（引用传递，会被更新）
     * @return 函数名
     */
    string getFunc(const string& expr, size_t& pos) {
        string func;
        while (pos < expr.length() && isalpha(expr[pos])) {
            func += expr[pos];
            pos++;
        }
        return func;
    }

    /**
     * 从表达式中提取数字
     * @param expr 表达式
     * @param pos 当前位置（引用传递，会被更新）
     * @return 数字字符串
     */
    string getNumber(const string& expr, size_t& pos) {
        string numStr;
        bool hasDecimal = false;
        bool hasExponent = false;

        // 处理负号（作为负数前缀）
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

    /**
     * 处理函数参数并计算函数值
     * @param expr 表达式
     * @param pos 当前位置（引用传递，会被更新）
     * @return 函数计算结果
     */
    double DealFunc(const string& expr, size_t& pos) {
        if (pos >= expr.length() || expr[pos] != '(') {
            throw runtime_error("函数后必须跟左括号");
        }
        pos++;  // 跳过左括号

        string paramExpr;
        int bcont = 1;  // 括号计数器，处理嵌套括号
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
            throw runtime_error("函数参数括号不匹配");
        }

        return Evaluate(paramExpr + "#");  // 递归计算参数表达式
    }

    /**
     * 解析并计算表达式
     * @param expression 待计算的表达式
     * @return 计算结果
     */
    double Evaluate(const string& expression) {
        stack<char> opStack;       // 运算符栈
        stack<double> numStack;    // 操作数栈

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
                case '=':  // 优先级相等（通常是括号匹配）
                    opStack.pop();
                    i++;
                    break;
                case '>': {  // 栈顶优先级高，先计算
                    char op = opStack.top();
                    opStack.pop();

                    if (op == '!') {  // 处理阶乘（单目运算）
                        if (numStack.empty()) {
                            throw runtime_error("阶乘运算缺少操作数");
                        }
                        double val = numStack.top();
                        numStack.pop();
                        numStack.push(Factorial(val));
                    }
                    else {  // 处理二元运算
                        if (numStack.size() < 2) {
                            throw runtime_error("二元运算操作数不足");
                        }
                        double b = numStack.top(); numStack.pop();
                        double a = numStack.top(); numStack.pop();
                        numStack.push(calculateSimple(a, op, b));
                    }
                    break;
                }
                default:
                    throw runtime_error("表达式语法错误: 无效的运算符组合 " +
                        string(1, stackTop) + " 和 " + string(1, currentOp));
                }
            }
            else {
                throw runtime_error("未知字符: " + string(1, expr[i]));
            }
        }

        // 检查最终状态是否合法
        if (numStack.size() != 1 || opStack.top() != '#') {
            throw runtime_error("表达式不完整或格式错误");
        }

        return numStack.top();
    }

    /**
     * 启动计算器交互界面
     */
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