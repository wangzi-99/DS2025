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
    // �������Ƶ�������ӳ��
    map<string, string> Fun;
    // ֧�ֵĺ����б�
    vector<string> funs = {
        "sqrt", "ln", "log", "sin", "cos", "tan",
        "asin", "acos", "atan", "exp", "abs"
    };

    // ��������ȼ��� [ջ�������][��ǰ�����]
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

    // �������������ӳ��
    map<char, int> opI = {
        {'+', 0}, {'-', 1}, {'*', 2}, {'/', 3},
        {'^', 4}, {'!', 5}, {'(', 6}, {')', 7}, {'#', 8}
    };

    // ��ʼ����������
    void initFuns() {
        Fun["sqrt"] = "����ƽ����";
        Fun["ln"] = "��Ȼ����";
        Fun["log"] = "���ö���";
        Fun["sin"] = "���Һ���";
        Fun["cos"] = "���Һ���";
        Fun["tan"] = "���к���";
        Fun["asin"] = "�����Һ���";
        Fun["acos"] = "�����Һ���";
        Fun["atan"] = "�����к���";
        Fun["exp"] = "ָ������";
        Fun["abs"] = "����ֵ����";
    }

public:
    Calculator() {
        initFuns();
    }

    /**
     * �Ƚ���������������ȼ�
     * @param stackTop ջ�������
     * @param current ��ǰ�����
     * @return ���ȼ���ϵ��<, =, >��
     */
    char comparePrt(char stackTop, char current) {
        if (opI.find(stackTop) != opI.end() && opI.find(current) != opI.end()) {
            int row = opI[stackTop];
            int col = opI[current];
            return prt[row][col];
        }
        throw runtime_error("δ֪�����");
    }

    /**
     * ִ�ж�Ԫ����
     * @param a �������
     * @param op �����
     * @param b �Ҳ�����
     * @return ������
     */
    double calculateSimple(double a, char op, double b) {
        switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) {
                throw runtime_error("��������Ϊ��");
            }
            return a / b;
        case '^': return pow(a, b);
        default:
            throw runtime_error("δ֪��Ԫ�����");
        }
    }

    /**
     * ����׳ˣ���Ŀ���㣩
     * @param n ������
     * @return �׳˽��
     */
    double Factorial(double n) {
        if (n < 0) {
            throw runtime_error("�׳˺�����������Ϊ��");
        }
        if (n != floor(n)) {  // ����Ƿ�Ϊ����
            throw runtime_error("�׳˺�����������Ϊ����");
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
     * ִ����ѧ��������
     * @param func ������
     * @param value ��������
     * @return ������
     */
    double calculateFun(const string& func, double value) {
        if (func == "sqrt") {
            if (value < 0) {
                throw runtime_error("ƽ����������������Ϊ��");
            }
            return sqrt(value);
        }
        else if (func == "ln") {
            if (value <= 0) {
                throw runtime_error("��Ȼ����������������Ϊ��");
            }
            return log(value);
        }
        else if (func == "log") {
            if (value <= 0) {
                throw runtime_error("���ö���������������Ϊ��");
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
                throw runtime_error("�����Һ�������������[-1,1]��Χ��");
            }
            return asin(value);
        }
        else if (func == "acos") {
            if (value < -1 || value > 1) {
                throw runtime_error("�����Һ�������������[-1,1]��Χ��");
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
            throw runtime_error("δ֪����: " + func);
        }
    }

    /**
     * ����Ƿ�Ϊ֧�ֵĺ���
     * @param func ������
     * @return �Ƿ�֧��
     */
    bool isF(const string& func) {
        return find(funs.begin(), funs.end(), func) != funs.end();
    }

    /**
     * ����Ƿ�Ϊ�����
     * @param c �ַ�
     * @return �Ƿ�Ϊ�����
     */
    bool isO(char c) {
        return string("+-*/^!()#").find(c) != string::npos;
    }

    /**
     * �ӱ��ʽ����ȡ������
     * @param expr ���ʽ
     * @param pos ��ǰλ�ã����ô��ݣ��ᱻ���£�
     * @return ������
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
     * �ӱ��ʽ����ȡ����
     * @param expr ���ʽ
     * @param pos ��ǰλ�ã����ô��ݣ��ᱻ���£�
     * @return �����ַ���
     */
    string getNumber(const string& expr, size_t& pos) {
        string numStr;
        bool hasDecimal = false;
        bool hasExponent = false;

        // �����ţ���Ϊ����ǰ׺��
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
                // ����ָ�����ֵ�������
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
     * ���������������㺯��ֵ
     * @param expr ���ʽ
     * @param pos ��ǰλ�ã����ô��ݣ��ᱻ���£�
     * @return ����������
     */
    double DealFunc(const string& expr, size_t& pos) {
        if (pos >= expr.length() || expr[pos] != '(') {
            throw runtime_error("����������������");
        }
        pos++;  // ����������

        string paramExpr;
        int bcont = 1;  // ���ż�����������Ƕ������
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
            throw runtime_error("�����������Ų�ƥ��");
        }

        return Evaluate(paramExpr + "#");  // �ݹ����������ʽ
    }

    /**
     * ������������ʽ
     * @param expression ������ı��ʽ
     * @return ������
     */
    double Evaluate(const string& expression) {
        stack<char> opStack;       // �����ջ
        stack<double> numStack;    // ������ջ

        string expr = expression + "#";  // ��ӽ�����
        opStack.push('#');

        size_t i = 0;
        while (i < expr.length() && !(expr[i] == '#' && opStack.top() == '#')) {
            if (isspace(expr[i])) {  // �����ո�
                i++;
                continue;
            }

            // ������
            if (isalpha(expr[i])) {
                size_t start = i;
                string func = getFunc(expr, i);

                if (isF(func)) {
                    double param = DealFunc(expr, i);
                    double result = calculateFun(func, param);
                    numStack.push(result);
                }
                else {
                    i = start;  // ����λ�ã�׼���׳�����
                    throw runtime_error("δ֪����: " + func);
                }
                continue;
            }

            // ��������
            if (isdigit(expr[i]) || expr[i] == '.' ||
                (expr[i] == '-' && (i == 0 || isO(expr[i - 1])))) {
                string numStr = getNumber(expr, i);
                try {
                    double num = stod(numStr);
                    numStack.push(num);
                }
                catch (const exception& e) {
                    throw runtime_error("���ָ�ʽ����: " + numStr);
                }
                continue;
            }

            // ���������
            if (isO(expr[i])) {
                char currentOp = expr[i];
                char stackTop = opStack.top();
                char rel = comparePrt(stackTop, currentOp);

                switch (rel) {
                case '<':  // ջ�����ȼ��ͣ���ջ
                    opStack.push(currentOp);
                    i++;
                    break;
                case '=':  // ���ȼ���ȣ�ͨ��������ƥ�䣩
                    opStack.pop();
                    i++;
                    break;
                case '>': {  // ջ�����ȼ��ߣ��ȼ���
                    char op = opStack.top();
                    opStack.pop();

                    if (op == '!') {  // ����׳ˣ���Ŀ���㣩
                        if (numStack.empty()) {
                            throw runtime_error("�׳�����ȱ�ٲ�����");
                        }
                        double val = numStack.top();
                        numStack.pop();
                        numStack.push(Factorial(val));
                    }
                    else {  // �����Ԫ����
                        if (numStack.size() < 2) {
                            throw runtime_error("��Ԫ�������������");
                        }
                        double b = numStack.top(); numStack.pop();
                        double a = numStack.top(); numStack.pop();
                        numStack.push(calculateSimple(a, op, b));
                    }
                    break;
                }
                default:
                    throw runtime_error("���ʽ�﷨����: ��Ч���������� " +
                        string(1, stackTop) + " �� " + string(1, currentOp));
                }
            }
            else {
                throw runtime_error("δ֪�ַ�: " + string(1, expr[i]));
            }
        }

        // �������״̬�Ƿ�Ϸ�
        if (numStack.size() != 1 || opStack.top() != '#') {
            throw runtime_error("���ʽ���������ʽ����");
        }

        return numStack.top();
    }

    /**
     * ������������������
     */
    void ActivateEvaluator() {
        string input;
        while (true) {
            cout << "��������ʽ (���� 'quit' �˳�): ";
            getline(cin, input);

            if (input == "quit" || input == "exit") {
                break;
            }
            if (input.empty()) {
                continue;
            }

            try {
                double result = Evaluate(input);
                cout << ">>> ���: " << result << endl << endl;
            }
            catch (const exception& e) {
                cout << ">>> ����: " << e.what() << endl << endl;
            }
        }
    }
};

int main() {
    Calculator calculator;
    calculator.ActivateEvaluator();
    return 0;
}