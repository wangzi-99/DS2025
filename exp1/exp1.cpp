#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <string>

// 复数类定义
class Complex {
private:
    double real;  // 实部
    double imag;  // 虚部

public:
    // 构造函数，默认初始化为0
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // 获取实部
    double getReal() const { return real; }

    // 获取虚部
    double getImag() const { return imag; }

    // 计算复数的模
    double getModulus() const { return std::sqrt(real * real + imag * imag); }

    // 重载相等运算符（实部和虚部均相同才视为相等）
    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }

    // 重载小于运算符（按模排序，模相同则按实部排序）
    bool operator<(const Complex& other) const {
        double mod1 = getModulus();
        double mod2 = other.getModulus();
        // 考虑浮点数精度问题，使用极小值判断模是否相等
        if (std::abs(mod1 - mod2) < 1e-10) {
            return real < other.real;
        }
        return mod1 < mod2;
    }

    // 重载输出运算符，方便打印复数
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << "(" << c.real;
        if (c.imag >= 0)
            os << "+" << c.imag << "i)";
        else
            os << c.imag << "i)";
        return os;
    }
};

// 向量置乱函数（Fisher-Yates洗牌算法）
void shuffleVector(std::vector<Complex>& vec) {
    for (int i = vec.size(); i > 0; i--) {
        std::swap(vec[i - 1], vec[rand() % i]);
    }
}

// 查找函数（查找实部和虚部均相同的复数）
int findComplex(const std::vector<Complex>& vec, const Complex& target) {
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == target) {
            return static_cast<int>(i);
        }
    }
    return -1;  // 未找到返回-1
}

// 向量唯一化函数（去除重复元素）
void uniqueVector(std::vector<Complex>& vec) {
    auto it = std::unique(vec.begin(), vec.end());
    vec.erase(it, vec.end());
}

// 起泡排序（按复数模排序）
void bubbleSort(std::vector<Complex>& vec) {
    for (size_t i = 0; i < vec.size() - 1; ++i) {
        for (size_t j = 0; j < vec.size() - i - 1; ++j) {
            if (!(vec[j] < vec[j + 1])) {  // 使用重载的<运算符比较
                std::swap(vec[j], vec[j + 1]);
            }
        }
    }
}

// 归并排序的合并函数
void merge(std::vector<Complex>& vec, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // 临时向量存储左右两部分
    std::vector<Complex> L(n1), R(n2);

    // 复制数据到临时向量
    for (int i = 0; i < n1; ++i)
        L[i] = vec[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = vec[mid + 1 + j];

    // 合并临时向量
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] < R[j]) {
            vec[k] = L[i];
            ++i;
        }
        else {
            vec[k] = R[j];
            ++j;
        }
        ++k;
    }

    // 处理剩余元素
    while (i < n1) {
        vec[k] = L[i];
        ++i;
        ++k;
    }
    while (j < n2) {
        vec[k] = R[j];
        ++j;
        ++k;
    }
}

// 归并排序递归实现
void mergeSort(std::vector<Complex>& vec, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;  // 避免溢出
    mergeSort(vec, left, mid);           // 排序左半部分
    mergeSort(vec, mid + 1, right);      // 排序右半部分
    merge(vec, left, mid, right);        // 合并两部分
}

// 归并排序接口函数
void mergeSort(std::vector<Complex>& vec) {
    if (vec.empty()) return;
    mergeSort(vec, 0, vec.size() - 1);
}

// 区间查找函数（查找模介于[m1, m2)的复数）
std::vector<Complex> rangeSearch(const std::vector<Complex>& vec, double m1, double m2) {
    std::vector<Complex> result;
    for (const auto& c : vec) {
        double mod = c.getModulus();
        if (mod >= m1 && mod < m2) {
            result.push_back(c);
        }
    }
    return result;
}

// 生成随机复数向量（包含重复项）
std::vector<Complex> generateRandomComplexVector(int size) {
    std::vector<Complex> vec;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-10.0, 10.0);  // 实部和虚部范围：[-10, 10)

    for (int i = 0; i < size; ++i) {
        vec.emplace_back(dis(gen), dis(gen));
    }
    return vec;
}

// 打印向量
void printVector(const std::vector<Complex>& vec, const std::string& title = "") {
    if (!title.empty()) {
        std::cout << title << ":" << std::endl;
    }
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) std::cout << ", ";
        if ((i + 1) % 5 == 0) std::cout << std::endl;  // 每5个元素换行
    }
    std::cout << std::endl << std::endl;
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));  // 初始化随机数种子

    // 1. 生成随机复数向量（含重复项）
    std::vector<Complex> vec = generateRandomComplexVector(20);
    printVector(vec, "初始随机复数向量");
    std::cout << "随机复数向量生成完毕" << std::endl;

    // (1) 测试无序向量的各种操作
    std::cout << "=== 测试各种操作 ===" << std::endl;

    // 置乱操作
    shuffleVector(vec);
    printVector(vec, "置乱后的向量");

    // 查找操作
    Complex target = vec[3];  // 以第4个元素为查找目标
    int index = findComplex(vec, target);
    std::cout << "查找 " << target << " 的位置: " << index << std::endl << std::endl;

    // 插入操作
    Complex newComplex(7.5, -3.2);
    vec.insert(vec.begin() + 2, newComplex);
    printVector(vec, "在位置2插入(7.5-3.2i)后的向量");

    // 删除操作
    vec.erase(vec.begin() + 5);
    printVector(vec, "删除位置5元素后的向量");

    // 唯一化操作（先添加重复元素）
    vec.push_back(vec[0]);
    vec.push_back(vec[1]);
    printVector(vec, "添加重复元素后的向量");
    uniqueVector(vec);
    printVector(vec, "唯一化后的向量");

    // (2) 排序效率比较
    std::cout << "=== 排序效率比较 ===" << std::endl;
    std::vector<Complex> largeVec = generateRandomComplexVector(5000);  // 生成较大向量用于测试效率

    // 准备三种排序状态的向量
    std::vector<Complex> sortedVec = largeVec;
    std::sort(sortedVec.begin(), sortedVec.end());  // 顺序向量

    std::vector<Complex> reverseVec = sortedVec;
    std::reverse(reverseVec.begin(), reverseVec.end());  // 逆序向量

    std::vector<Complex> randomVec = largeVec;
    shuffleVector(randomVec);  // 乱序向量

    // 测试起泡排序效率
    auto testBubbleSort = [](std::vector<Complex> vec, const std::string& type) {
        clock_t start = clock();
        bubbleSort(vec);
        clock_t end = clock();
        double duration = double(end - start) / CLOCKS_PER_SEC * 1000.0;  // 转换为毫秒
        std::cout << "起泡排序(" << type << "): " << std::fixed << std::setprecision(6)
            << duration << " ms" << std::endl;
        };

    // 测试归并排序效率
    auto testMergeSort = [](std::vector<Complex> vec, const std::string& type) {
        clock_t start = clock();
        mergeSort(vec);
        clock_t end = clock();
        double duration = double(end - start) / CLOCKS_PER_SEC * 1000.0;  // 转换为毫秒
        std::cout << "归并排序(" << type << "): " << std::fixed << std::setprecision(6)
            << duration << " ms" << std::endl;
        };

    // 测试并输出排序时间
    testBubbleSort(sortedVec, "顺序");
    testBubbleSort(randomVec, "乱序");
    testBubbleSort(reverseVec, "逆序");
    std::cout << std::endl;

    testMergeSort(sortedVec, "顺序");
    testMergeSort(randomVec, "乱序");
    testMergeSort(reverseVec, "逆序");
    std::cout << std::endl;

    // (3) 区间查找
    std::cout << "=== 区间查找 ===" << std::endl;

    // 先对向量排序
    mergeSort(vec);
    printVector(vec, "排序后的向量（用于区间查找）");

    // 查找模介于[5.0, 10.0)的复数
    double m1 = 5.0, m2 = 10.0;
    std::vector<Complex> rangeResult = rangeSearch(vec, m1, m2);
    printVector(rangeResult, "模在[" + std::to_string(m1) + ", " + std::to_string(m2) + ")范围内的复数");

    // 显示找到的复数的模值
    std::cout << "对应的模值: ";
    for (const auto& c : rangeResult) {
        std::cout << std::fixed << std::setprecision(2) << c.getModulus() << " ";
    }
    std::cout << std::endl;

    return 0;
}