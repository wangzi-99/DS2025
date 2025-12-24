#include "Dictionary.h"
#include "Entry.h"
#include "Bitmap.h"

// 字符哈希函数
static size_t hashCode(char c) { return (size_t)c; }

// 符合Dictionary接口的Hashtable模板类
template <typename K, typename V>
class Hashtable : public Dictionary<K, V>
{ // key、value
    friend class UniPrint;

private:
    Entry<K, V>** ht; // 桶数组，存放词条指针
    int M, N, L;      // 桶的总数、词条的数目、懒惰删除标记的数目（N + L <= M）
    Bitmap* removed;  // 懒惰删除标记
protected:
    int probe4Hit(const K& k);  // 沿关键码k对应的试探链，找到词条匹配的桶
    int probe4Free(const K& k); // 沿关键码k对应的试探链，找到首个可用空桶
    void rehash();              // 重散列算法：扩充桶数组，保证装填因子在警戒线以下
public:
    Hashtable(int c = 5);          // 创建一个容量不小于c的散列表（为测试暂时选用较小的默认值）
    ~Hashtable();                  // 释放桶数组及其中各（非空）元素所指向的词条
    int size() const { return N; } // 当前的词条数目
    bool put(K, V);                // 插入（禁止雷同词条，故可能失败）
    V* get(K k);                   // 读取
    bool remove(K k);              // 删除
};

// Hashtable 构造函数实现
template <typename K, typename V>
Hashtable<K, V>::Hashtable(int c) {
    M = c; N = 0; L = 0;
    ht = new Entry<K, V>* [M];
    memset(ht, 0, sizeof(Entry<K, V>*) * M);
    removed = new Bitmap(M);
}

// Hashtable 析构函数实现
template <typename K, typename V>
Hashtable<K, V>::~Hashtable() {
    for (int i = 0; i < M; i++) {
        if (ht[i]) {
            delete ht[i];
        }
    }
    delete[] ht;
    delete removed;
}

// Hashtable put方法实现
template <typename K, typename V>
bool Hashtable<K, V>::put(K k, V v) {
    if (get(k) != nullptr) return false; // 已存在相同关键码的词条

    int i = probe4Free(k);
    ht[i] = new Entry<K, V>(k, v);
    N++;

    if (N * 2 > M) rehash();
    return true;
}

// Hashtable get方法实现
template <typename K, typename V>
V* Hashtable<K, V>::get(K k) {
    int i = probe4Hit(k);
    return (i < 0) ? nullptr : &(ht[i]->value);
}

// Hashtable remove方法实现
template <typename K, typename V>
bool Hashtable<K, V>::remove(K k) {
    int i = probe4Hit(k);
    if (i < 0) return false;

    delete ht[i];
    ht[i] = nullptr;
    N--;
    L++;

    if (L > N / 2) rehash();
    return true;
}

// Hashtable probe4Hit方法实现
template <typename K, typename V>
int Hashtable<K, V>::probe4Hit(const K& k) {
    int r = hashCode(k) % M;
    int i = r;

    do {
        if (!ht[i] || (ht[i] && ht[i]->key == k)) {
            break;
        }
        i = (i + 1) % M;
    } while (i != r);

    return (ht[i] && (ht[i]->key == k)) ? i : -1;
}

// Hashtable probe4Free方法实现
template <typename K, typename V>
int Hashtable<K, V>::probe4Free(const K& k) {
    int r = hashCode(k) % M;
    int i = r;

    while (ht[i] != nullptr) {
        i = (i + 1) % M;
    }

    return i;
}

// Hashtable rehash方法实现
template <typename K, typename V>
void Hashtable<K, V>::rehash() {
    // 保存旧的桶数组
    Entry<K, V>** old_ht = ht;
    int old_M = M;

    // 创建新的更大的桶数组
    M = 2 * M + 1;
    N = 0; L = 0;
    ht = new Entry<K, V>* [M];
    memset(ht, 0, sizeof(Entry<K, V>*) * M);

    // 将旧的词条重新插入到新表中
    for (int i = 0; i < old_M; i++) {
        if (old_ht[i]) {
            put(old_ht[i]->key, old_ht[i]->value);
        }
    }

    // 释放旧的桶数组
    delete[] old_ht;
}

// 释放节点值
template <typename T>
void releases(T& e)
{
    e = NULL;
}