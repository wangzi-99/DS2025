#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <string>

// �����ඨ��
class Complex {
private:
    double real;  // ʵ��
    double imag;  // �鲿

public:
    // ���캯����Ĭ�ϳ�ʼ��Ϊ0
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // ��ȡʵ��
    double getReal() const { return real; }

    // ��ȡ�鲿
    double getImag() const { return imag; }

    // ���㸴����ģ
    double getModulus() const { return std::sqrt(real * real + imag * imag); }

    // ��������������ʵ�����鲿����ͬ����Ϊ��ȣ�
    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }

    // ����С�����������ģ����ģ��ͬ��ʵ������
    bool operator<(const Complex& other) const {
        double mod1 = getModulus();
        double mod2 = other.getModulus();
        // ���Ǹ������������⣬ʹ�ü�Сֵ�ж�ģ�Ƿ����
        if (std::abs(mod1 - mod2) < 1e-10) {
            return real < other.real;
        }
        return mod1 < mod2;
    }

    // �������������������ӡ����
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << "(" << c.real;
        if (c.imag >= 0)
            os << "+" << c.imag << "i)";
        else
            os << c.imag << "i)";
        return os;
    }
};

// �������Һ�����Fisher-Yatesϴ���㷨��
void shuffleVector(std::vector<Complex>& vec) {
    for (int i = vec.size(); i > 0; i--) {
        std::swap(vec[i - 1], vec[rand() % i]);
    }
}

// ���Һ���������ʵ�����鲿����ͬ�ĸ�����
int findComplex(const std::vector<Complex>& vec, const Complex& target) {
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == target) {
            return static_cast<int>(i);
        }
    }
    return -1;  // δ�ҵ�����-1
}

// ����Ψһ��������ȥ���ظ�Ԫ�أ�
void uniqueVector(std::vector<Complex>& vec) {
    auto it = std::unique(vec.begin(), vec.end());
    vec.erase(it, vec.end());
}

// �������򣨰�����ģ����
void bubbleSort(std::vector<Complex>& vec) {
    for (size_t i = 0; i < vec.size() - 1; ++i) {
        for (size_t j = 0; j < vec.size() - i - 1; ++j) {
            if (!(vec[j] < vec[j + 1])) {  // ʹ�����ص�<������Ƚ�
                std::swap(vec[j], vec[j + 1]);
            }
        }
    }
}

// �鲢����ĺϲ�����
void merge(std::vector<Complex>& vec, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // ��ʱ�����洢����������
    std::vector<Complex> L(n1), R(n2);

    // �������ݵ���ʱ����
    for (int i = 0; i < n1; ++i)
        L[i] = vec[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = vec[mid + 1 + j];

    // �ϲ���ʱ����
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

    // ����ʣ��Ԫ��
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

// �鲢����ݹ�ʵ��
void mergeSort(std::vector<Complex>& vec, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;  // �������
    mergeSort(vec, left, mid);           // ������벿��
    mergeSort(vec, mid + 1, right);      // �����Ұ벿��
    merge(vec, left, mid, right);        // �ϲ�������
}

// �鲢����ӿں���
void mergeSort(std::vector<Complex>& vec) {
    if (vec.empty()) return;
    mergeSort(vec, 0, vec.size() - 1);
}

// ������Һ���������ģ����[m1, m2)�ĸ�����
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

// ����������������������ظ��
std::vector<Complex> generateRandomComplexVector(int size) {
    std::vector<Complex> vec;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-10.0, 10.0);  // ʵ�����鲿��Χ��[-10, 10)

    for (int i = 0; i < size; ++i) {
        vec.emplace_back(dis(gen), dis(gen));
    }
    return vec;
}

// ��ӡ����
void printVector(const std::vector<Complex>& vec, const std::string& title = "") {
    if (!title.empty()) {
        std::cout << title << ":" << std::endl;
    }
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) std::cout << ", ";
        if ((i + 1) % 5 == 0) std::cout << std::endl;  // ÿ5��Ԫ�ػ���
    }
    std::cout << std::endl << std::endl;
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));  // ��ʼ�����������

    // 1. ��������������������ظ��
    std::vector<Complex> vec = generateRandomComplexVector(20);
    printVector(vec, "��ʼ�����������");
    std::cout << "������������������" << std::endl;

    // (1) �������������ĸ��ֲ���
    std::cout << "=== ���Ը��ֲ��� ===" << std::endl;

    // ���Ҳ���
    shuffleVector(vec);
    printVector(vec, "���Һ������");

    // ���Ҳ���
    Complex target = vec[3];  // �Ե�4��Ԫ��Ϊ����Ŀ��
    int index = findComplex(vec, target);
    std::cout << "���� " << target << " ��λ��: " << index << std::endl << std::endl;

    // �������
    Complex newComplex(7.5, -3.2);
    vec.insert(vec.begin() + 2, newComplex);
    printVector(vec, "��λ��2����(7.5-3.2i)�������");

    // ɾ������
    vec.erase(vec.begin() + 5);
    printVector(vec, "ɾ��λ��5Ԫ�غ������");

    // Ψһ��������������ظ�Ԫ�أ�
    vec.push_back(vec[0]);
    vec.push_back(vec[1]);
    printVector(vec, "����ظ�Ԫ�غ������");
    uniqueVector(vec);
    printVector(vec, "Ψһ���������");

    // (2) ����Ч�ʱȽ�
    std::cout << "=== ����Ч�ʱȽ� ===" << std::endl;
    std::vector<Complex> largeVec = generateRandomComplexVector(5000);  // ���ɽϴ��������ڲ���Ч��

    // ׼����������״̬������
    std::vector<Complex> sortedVec = largeVec;
    std::sort(sortedVec.begin(), sortedVec.end());  // ˳������

    std::vector<Complex> reverseVec = sortedVec;
    std::reverse(reverseVec.begin(), reverseVec.end());  // ��������

    std::vector<Complex> randomVec = largeVec;
    shuffleVector(randomVec);  // ��������

    // ������������Ч��
    auto testBubbleSort = [](std::vector<Complex> vec, const std::string& type) {
        clock_t start = clock();
        bubbleSort(vec);
        clock_t end = clock();
        double duration = double(end - start) / CLOCKS_PER_SEC * 1000.0;  // ת��Ϊ����
        std::cout << "��������(" << type << "): " << std::fixed << std::setprecision(6)
            << duration << " ms" << std::endl;
        };

    // ���Թ鲢����Ч��
    auto testMergeSort = [](std::vector<Complex> vec, const std::string& type) {
        clock_t start = clock();
        mergeSort(vec);
        clock_t end = clock();
        double duration = double(end - start) / CLOCKS_PER_SEC * 1000.0;  // ת��Ϊ����
        std::cout << "�鲢����(" << type << "): " << std::fixed << std::setprecision(6)
            << duration << " ms" << std::endl;
        };

    // ���Բ��������ʱ��
    testBubbleSort(sortedVec, "˳��");
    testBubbleSort(randomVec, "����");
    testBubbleSort(reverseVec, "����");
    std::cout << std::endl;

    testMergeSort(sortedVec, "˳��");
    testMergeSort(randomVec, "����");
    testMergeSort(reverseVec, "����");
    std::cout << std::endl;

    // (3) �������
    std::cout << "=== ������� ===" << std::endl;

    // �ȶ���������
    mergeSort(vec);
    printVector(vec, "����������������������ң�");

    // ����ģ����[5.0, 10.0)�ĸ���
    double m1 = 5.0, m2 = 10.0;
    std::vector<Complex> rangeResult = rangeSearch(vec, m1, m2);
    printVector(rangeResult, "ģ��[" + std::to_string(m1) + ", " + std::to_string(m2) + ")��Χ�ڵĸ���");

    // ��ʾ�ҵ��ĸ�����ģֵ
    std::cout << "��Ӧ��ģֵ: ";
    for (const auto& c : rangeResult) {
        std::cout << std::fixed << std::setprecision(2) << c.getModulus() << " ";
    }
    std::cout << std::endl;

    return 0;
}