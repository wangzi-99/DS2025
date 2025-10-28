#ifndef MYLIBRARY_LIST_H
#define MYLIBRARY_LIST_H

#include <algorithm> // 用于std::swap

// 双向// 双向链表节点模板类
// 每个节点包含数据、前驱指针和后继指针
//
template <typename T> struct ListNode {
    T data;               // 节点存储的数据
    ListNode<T>* pred;    // 前驱节点指针
    ListNode<T>* succ;    // 后继节点指针

    // 构造函数：默认初始化指针为nullptr
    ListNode() : pred(nullptr), succ(nullptr) {}
    // 构造函数：初始化数据和前后指针
    ListNode(T e, ListNode<T>* p = nullptr, ListNode<T>* s = nullptr)
        : data(e), pred(p), succ(s) {
    }

    // 在当前节点前插入新节点，返回新节点指针
    ListNode<T>* insertAsPred(T const& e) {
        ListNode<T>* x = new ListNode(e, pred, this);
        pred->succ = x;
        pred = x;
        return x;
    }

    // 在当前节点后插入新节点，返回新节点指针
    ListNode<T>* insertAsSucc(T const& e) {
        ListNode<T>* x = new ListNode(e, this, succ);
        succ->pred = x;
        succ = x;
        return x;
    }
};

//
// 双向链表模板类（带哨兵节点）
// 包含头哨兵(header)和尾哨兵(trailer)，简化边界处理
//
template <typename T> class List {
private:
    int _size;                 // 链表实际节点数量
    ListNode<T>* header;       // 头哨兵节点（不存储数据）
    ListNode<T>* trailer;      // 尾哨兵节点（不存储数据）

    // 交换两个有效节点的数据
    void swapData(ListNode<T>* a, ListNode<T>* b) {
        if (valid(a) && valid(b)) {
            T temp = a->data;
            a->data = b->data;
            b->data = temp;
        }
    }

protected:
    // 初始化链表：创建哨兵节点并建立连接
    void init() {
        header = new ListNode<T>;
        trailer = new ListNode<T>;
        header->succ = trailer;
        header->pred = nullptr;
        trailer->pred = header;
        trailer->succ = nullptr;
        _size = 0;
    }

    // 清空所有有效节点，返回原节点数量
    int clear() {
        int oldSize = _size;
        while (0 < _size) remove(header->succ);
        return oldSize;
    }

    // 从节点p开始复制n个节点到当前链表
    void copyNodes(ListNode<T>* p, int n) {
        init();
        while (n--) {
            insertAsLast(p->data);
            p = p->succ;
        }
    }

    // 归并排序辅助函数：合并两个有序子链表
    void merge(ListNode<T>*& p, int n, List<T>& L, ListNode<T>* q, int m) {
        ListNode<T>* pp = p->pred;
        while (0 < m) {
            if ((0 < n) && (p->data <= q->data)) {
                if (q == (p = p->succ)) break;
                n--;
            }
            else {
                insertBefore(p, L.remove((q = q->succ)->pred));
                m--;
            }
        }
        p = pp->succ;
    }

    // 归并排序：递归划分并合并
    void mergeSort(ListNode<T>*& p, int n) {
        if (n < 2) return;
        int m = n >> 1; // 二分
        ListNode<T>* q = p;
        for (int i = 0; i < m; i++) q = q->succ;
        mergeSort(p, m);
        mergeSort(q, n - m);
        merge(p, m, *this, q, n - m);
    }

    // 选择排序：每次选最大元素放到尾部
    void selectionSort(ListNode<T>* p, int n) {
        ListNode<T>* head = p->pred;
        ListNode<T>* tail = p;
        for (int i = 0; i < n; i++) tail = tail->succ;
        while (1 < n) {
            ListNode<T>* max = selectMax(head->succ, n);
            insertBefore(tail, remove(max));
            tail = tail->pred;
            n--;
        }
    }

    // 插入排序：将元素插入到前面合适位置
    void insertionSort(ListNode<T>* p, int n) {
        for (int r = 0; r < n; r++) {
            insertAfter(search(p->data, r, p), p->data);
            p = p->succ;
            remove(p->pred);
        }
    }

public:
    // 构造函数
    List() { init(); }                                  // 默认构造
    List(List<T> const& L) { copyNodes(L.first(), L._size); } // 复制构造
    List(List<T> const& L, int r, int n);               // 范围构造（声明）
    List(ListNode<T>* p, int n);                        // 节点范围构造（声明）

    // 析构函数：清空节点并释放哨兵
    ~List() {
        clear();
        delete header;
        delete trailer;
    }

    // 赋值运算符重载：深拷贝
    List<T>& operator=(List<T> const& L) {
        if (this != &L) {
            clear();
            copyNodes(L.first(), L._size);
        }
        return *this;
    }

    // 基础访问接口
    int size() const { return _size; }                  // 获取当前大小
    bool empty() const { return _size <= 0; }           // 判断是否为空
    T& operator[](int r) const;                         // 重载[]访问（声明）
    ListNode<T>* first() const { return header->succ; } // 获取首节点
    ListNode<T>* last() const { return trailer->pred; } // 获取尾节点
    bool valid(ListNode<T>* p) {                        // 判断节点是否有效
        return p && p != header && p != trailer;
    }

    // 查找相关接口
    int disordered() const;                             // 判断无序程度（声明）
    ListNode<T>* find(T const& e) const;                // 查找元素（声明）
    ListNode<T>* find(T const& e, int n, ListNode<T>* p) const; // 范围查找（声明）
    ListNode<T>* search(T const& e) const;              // 有序查找（声明）
    ListNode<T>* search(T const& e, int n, ListNode<T>* p) const; // 范围有序查找（声明）

    // 排序辅助接口
    ListNode<T>* selectMax(ListNode<T>* p, int n);      // 范围选最大（声明）
    ListNode<T>* selectMax() {                          // 整体选最大
        return selectMax(header->succ, _size);
    }

    // 插入操作
    ListNode<T>* insertAsFirst(T const& e) {            // 头部插入
        _size++;
        return header->insertAsSucc(e);
    }
    ListNode<T>* insertAsLast(T const& e) {             // 尾部插入
        _size++;
        return trailer->insertAsPred(e);
    }
    ListNode<T>* insertBefore(ListNode<T>* p, T const& e) { // 在p前插入
        if (!valid(p)) throw "Invalid position";
        _size++;
        return p->insertAsPred(e);
    }
    ListNode<T>* insertAfter(ListNode<T>* p, T const& e) {  // 在p后插入
        if (!valid(p)) throw "Invalid position";
        _size++;
        return p->insertAsSucc(e);
    }

    // 删除操作：移除节点p，返回其数据
    T remove(ListNode<T>* p) {
        if (!valid(p)) throw "Invalid node";
        T e = p->data;
        p->pred->succ = p->succ;
        p->succ->pred = p->pred;
        delete p;
        _size--;
        return e;
    }

    // 排序操作
    void merge(List<T>& L) {                            // 合并另一个链表
        if (this != &L) merge(first(), _size, L, L.first(), L._size);
    }
    void sort(ListNode<T>* p, int n);                   // 范围排序（声明）
    void sort() {                                       // 整体排序
        if (_size > 1) sort(first(), _size);
    }

    // 去重操作
    int deduplicate();                                  // 无序去重（声明）
    int uniquify();                                     // 有序去重（声明）

    // 反转操作：反转整个链表
    void reverse() {
        if (_size < 2) return;
        ListNode<T>* p = header;
        do {
            std::swap(p->pred, p->succ);  // 交换前后指针
        } while ((p = p->pred) != trailer);
        std::swap(header, trailer);       // 交换头尾哨兵
    }

    // 遍历操作
    void traverse(void (*visit)(T&)) {                   // 函数指针遍历
        for (ListNode<T>* p = first(); p != trailer; p = p->succ) {
            visit(p->data);
        }
    }
    template <typename VST> void traverse(VST& visit) {  // 函数对象遍历
        for (ListNode<T>* p = first(); p != trailer; p = p->succ) {
            visit(p->data);
        }
    }

    // 获取节点的索引（秩）
    int rank(ListNode<T>* p) const {
        if (!valid(p)) return -1;
        int r = 0;
        for (ListNode<T>* cur = first(); cur != p; cur = cur->succ) r++;
        return r;
    }

    // 移动节点p到节点q之前
    void moveBefore(ListNode<T>* p, ListNode<T>* q) {
        if (!valid(p) || !valid(q) || p == q) return;
        // 先从原位置移除p
        p->pred->succ = p->succ;
        p->succ->pred = p->pred;
        // 再插入到q之前
        p->pred = q->pred;
        p->succ = q;
        q->pred->succ = p;
        q->pred = p;
    }
};

// 范围构造函数实现
template <typename T>
List<T>::List(List<T> const& L, int r, int n) {
    if (r < 0 || n < 0 || r + n > L._size) throw "Invalid range";
    copyNodes(L[r], n);
}

// 节点范围构造函数实现
template <typename T>
List<T>::List(ListNode<T>* p, int n) {
    if (!p || n < 0) throw "Invalid node or count";
    copyNodes(p, n);
}

// []运算符重载实现（按索引访问）
template <typename T>
T& List<T>::operator[](int r) const {
    if (r < 0 || r >= _size) throw "Index out of range";
    ListNode<T>* p = first();
    while (r--) p = p->succ;  // 移动r次到目标位置
    return p->data;
}

// 判断链表无序程度（相邻逆序对数量）
template <typename T>
int List<T>::disordered() const {
    int cnt = 0;
    for (ListNode<T>* p = first(); p->succ != trailer; p = p->succ) {
        if (p->data > p->succ->data) cnt++;
    }
    return cnt;
}

// 查找元素e（从尾部向前找整个链表）
template <typename T>
ListNode<T>* List<T>::find(T const& e) const {
    return find(e, _size, trailer);
}

// 范围查找：从p向前找n个节点中是否有e
template <typename T>
ListNode<T>* List<T>::find(T const& e, int n, ListNode<T>* p) const {
    if (n < 0 || !valid(p)) throw "Invalid parameters";
    while (n--) {
        p = p->pred;
        if (p->data == e) return p;
    }
    return nullptr;
}

// 有序查找e（默认查整个链表）
template <typename T>
ListNode<T>* List<T>::search(T const& e) const {
    return search(e, _size, trailer);
}

// 范围有序查找：从p向前找n个节点，返回<=e的最后一个节点
template <typename T>
ListNode<T>* List<T>::search(T const& e, int n, ListNode<T>* p) const {
    if (n < 0 || !valid(p)) throw "Invalid parameters";
    while (n-- >= 0) {
        p = p->pred;
        if (p->data <= e) break;
    }
    return p;
}

// 范围选最大节点（从p开始的n个节点）
template <typename T>
ListNode<T>* List<T>::selectMax(ListNode<T>* p, int n) {
    if (!valid(p) || n <= 0) throw "Invalid parameters";
    ListNode<T>* max = p;
    for (int i = 1; i < n; i++) {
        p = p->succ;
        if (p->data >= max->data) max = p;
    }
    return max;
}

// 范围排序：随机选择排序算法
template <typename T>
void List<T>::sort(ListNode<T>* p, int n) {
    if (!valid(p) || n <= 0 || n > _size) throw "Invalid range";
    switch (rand() % 3) {
    case 1: insertionSort(p, n); break;
    case 2: selectionSort(p, n); break;
    default: mergeSort(p, n); break;
    }
}

// 无序去重：删除重复元素（保留第一个）
template <typename T>
int List<T>::deduplicate() {
    if (_size < 2) return 0;
    int oldSize = _size;
    ListNode<T>* p = first();
    int r = 0;
    while (p != trailer) {
        ListNode<T>* q = find(p->data, r, p);  // 查找前r个节点中是否有重复
        if (q) remove(q);
        else r++;
        p = p->succ;
    }
    return oldSize - _size;  // 返回删除的节点数
}

// 有序去重：删除连续重复元素（保留第一个）
template <typename T>
int List<T>::uniquify() {
    if (_size < 2) return 0;
    int oldSize = _size;
    ListNode<T>* p = first();
    ListNode<T>* q = p->succ;
    while (q != trailer) {
        if (p->data == q->data) {
            remove(q);
            q = p->succ;  // 删除后q指向p的新后继
        }
        else {
            p = q;
            q = q->succ;
        }
    }
    return oldSize - _size;  // 返回删除的节点数
}

#endif // MYLIBRARY_LIST_H