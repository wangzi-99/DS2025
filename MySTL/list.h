#ifndef MYLIBRARY_LIST_H
#define MYLIBRARY_LIST_H

#include <algorithm> // ����std::swap

// ˫��// ˫������ڵ�ģ����
// ÿ���ڵ�������ݡ�ǰ��ָ��ͺ��ָ��
//
template <typename T> struct ListNode {
    T data;               // �ڵ�洢������
    ListNode<T>* pred;    // ǰ���ڵ�ָ��
    ListNode<T>* succ;    // ��̽ڵ�ָ��

    // ���캯����Ĭ�ϳ�ʼ��ָ��Ϊnullptr
    ListNode() : pred(nullptr), succ(nullptr) {}
    // ���캯������ʼ�����ݺ�ǰ��ָ��
    ListNode(T e, ListNode<T>* p = nullptr, ListNode<T>* s = nullptr)
        : data(e), pred(p), succ(s) {
    }

    // �ڵ�ǰ�ڵ�ǰ�����½ڵ㣬�����½ڵ�ָ��
    ListNode<T>* insertAsPred(T const& e) {
        ListNode<T>* x = new ListNode(e, pred, this);
        pred->succ = x;
        pred = x;
        return x;
    }

    // �ڵ�ǰ�ڵ������½ڵ㣬�����½ڵ�ָ��
    ListNode<T>* insertAsSucc(T const& e) {
        ListNode<T>* x = new ListNode(e, this, succ);
        succ->pred = x;
        succ = x;
        return x;
    }
};

//
// ˫������ģ���ࣨ���ڱ��ڵ㣩
// ����ͷ�ڱ�(header)��β�ڱ�(trailer)���򻯱߽紦��
//
template <typename T> class List {
private:
    int _size;                 // ����ʵ�ʽڵ�����
    ListNode<T>* header;       // ͷ�ڱ��ڵ㣨���洢���ݣ�
    ListNode<T>* trailer;      // β�ڱ��ڵ㣨���洢���ݣ�

    // ����������Ч�ڵ������
    void swapData(ListNode<T>* a, ListNode<T>* b) {
        if (valid(a) && valid(b)) {
            T temp = a->data;
            a->data = b->data;
            b->data = temp;
        }
    }

protected:
    // ��ʼ�����������ڱ��ڵ㲢��������
    void init() {
        header = new ListNode<T>;
        trailer = new ListNode<T>;
        header->succ = trailer;
        header->pred = nullptr;
        trailer->pred = header;
        trailer->succ = nullptr;
        _size = 0;
    }

    // ���������Ч�ڵ㣬����ԭ�ڵ�����
    int clear() {
        int oldSize = _size;
        while (0 < _size) remove(header->succ);
        return oldSize;
    }

    // �ӽڵ�p��ʼ����n���ڵ㵽��ǰ����
    void copyNodes(ListNode<T>* p, int n) {
        init();
        while (n--) {
            insertAsLast(p->data);
            p = p->succ;
        }
    }

    // �鲢�������������ϲ���������������
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

    // �鲢���򣺵ݹ黮�ֲ��ϲ�
    void mergeSort(ListNode<T>*& p, int n) {
        if (n < 2) return;
        int m = n >> 1; // ����
        ListNode<T>* q = p;
        for (int i = 0; i < m; i++) q = q->succ;
        mergeSort(p, m);
        mergeSort(q, n - m);
        merge(p, m, *this, q, n - m);
    }

    // ѡ������ÿ��ѡ���Ԫ�طŵ�β��
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

    // �������򣺽�Ԫ�ز��뵽ǰ�����λ��
    void insertionSort(ListNode<T>* p, int n) {
        for (int r = 0; r < n; r++) {
            insertAfter(search(p->data, r, p), p->data);
            p = p->succ;
            remove(p->pred);
        }
    }

public:
    // ���캯��
    List() { init(); }                                  // Ĭ�Ϲ���
    List(List<T> const& L) { copyNodes(L.first(), L._size); } // ���ƹ���
    List(List<T> const& L, int r, int n);               // ��Χ���죨������
    List(ListNode<T>* p, int n);                        // �ڵ㷶Χ���죨������

    // ������������սڵ㲢�ͷ��ڱ�
    ~List() {
        clear();
        delete header;
        delete trailer;
    }

    // ��ֵ��������أ����
    List<T>& operator=(List<T> const& L) {
        if (this != &L) {
            clear();
            copyNodes(L.first(), L._size);
        }
        return *this;
    }

    // �������ʽӿ�
    int size() const { return _size; }                  // ��ȡ��ǰ��С
    bool empty() const { return _size <= 0; }           // �ж��Ƿ�Ϊ��
    T& operator[](int r) const;                         // ����[]���ʣ�������
    ListNode<T>* first() const { return header->succ; } // ��ȡ�׽ڵ�
    ListNode<T>* last() const { return trailer->pred; } // ��ȡβ�ڵ�
    bool valid(ListNode<T>* p) {                        // �жϽڵ��Ƿ���Ч
        return p && p != header && p != trailer;
    }

    // ������ؽӿ�
    int disordered() const;                             // �ж�����̶ȣ�������
    ListNode<T>* find(T const& e) const;                // ����Ԫ�أ�������
    ListNode<T>* find(T const& e, int n, ListNode<T>* p) const; // ��Χ���ң�������
    ListNode<T>* search(T const& e) const;              // ������ң�������
    ListNode<T>* search(T const& e, int n, ListNode<T>* p) const; // ��Χ������ң�������

    // �������ӿ�
    ListNode<T>* selectMax(ListNode<T>* p, int n);      // ��Χѡ���������
    ListNode<T>* selectMax() {                          // ����ѡ���
        return selectMax(header->succ, _size);
    }

    // �������
    ListNode<T>* insertAsFirst(T const& e) {            // ͷ������
        _size++;
        return header->insertAsSucc(e);
    }
    ListNode<T>* insertAsLast(T const& e) {             // β������
        _size++;
        return trailer->insertAsPred(e);
    }
    ListNode<T>* insertBefore(ListNode<T>* p, T const& e) { // ��pǰ����
        if (!valid(p)) throw "Invalid position";
        _size++;
        return p->insertAsPred(e);
    }
    ListNode<T>* insertAfter(ListNode<T>* p, T const& e) {  // ��p�����
        if (!valid(p)) throw "Invalid position";
        _size++;
        return p->insertAsSucc(e);
    }

    // ɾ���������Ƴ��ڵ�p������������
    T remove(ListNode<T>* p) {
        if (!valid(p)) throw "Invalid node";
        T e = p->data;
        p->pred->succ = p->succ;
        p->succ->pred = p->pred;
        delete p;
        _size--;
        return e;
    }

    // �������
    void merge(List<T>& L) {                            // �ϲ���һ������
        if (this != &L) merge(first(), _size, L, L.first(), L._size);
    }
    void sort(ListNode<T>* p, int n);                   // ��Χ����������
    void sort() {                                       // ��������
        if (_size > 1) sort(first(), _size);
    }

    // ȥ�ز���
    int deduplicate();                                  // ����ȥ�أ�������
    int uniquify();                                     // ����ȥ�أ�������

    // ��ת��������ת��������
    void reverse() {
        if (_size < 2) return;
        ListNode<T>* p = header;
        do {
            std::swap(p->pred, p->succ);  // ����ǰ��ָ��
        } while ((p = p->pred) != trailer);
        std::swap(header, trailer);       // ����ͷβ�ڱ�
    }

    // ��������
    void traverse(void (*visit)(T&)) {                   // ����ָ�����
        for (ListNode<T>* p = first(); p != trailer; p = p->succ) {
            visit(p->data);
        }
    }
    template <typename VST> void traverse(VST& visit) {  // �����������
        for (ListNode<T>* p = first(); p != trailer; p = p->succ) {
            visit(p->data);
        }
    }

    // ��ȡ�ڵ���������ȣ�
    int rank(ListNode<T>* p) const {
        if (!valid(p)) return -1;
        int r = 0;
        for (ListNode<T>* cur = first(); cur != p; cur = cur->succ) r++;
        return r;
    }

    // �ƶ��ڵ�p���ڵ�q֮ǰ
    void moveBefore(ListNode<T>* p, ListNode<T>* q) {
        if (!valid(p) || !valid(q) || p == q) return;
        // �ȴ�ԭλ���Ƴ�p
        p->pred->succ = p->succ;
        p->succ->pred = p->pred;
        // �ٲ��뵽q֮ǰ
        p->pred = q->pred;
        p->succ = q;
        q->pred->succ = p;
        q->pred = p;
    }
};

// ��Χ���캯��ʵ��
template <typename T>
List<T>::List(List<T> const& L, int r, int n) {
    if (r < 0 || n < 0 || r + n > L._size) throw "Invalid range";
    copyNodes(L[r], n);
}

// �ڵ㷶Χ���캯��ʵ��
template <typename T>
List<T>::List(ListNode<T>* p, int n) {
    if (!p || n < 0) throw "Invalid node or count";
    copyNodes(p, n);
}

// []���������ʵ�֣����������ʣ�
template <typename T>
T& List<T>::operator[](int r) const {
    if (r < 0 || r >= _size) throw "Index out of range";
    ListNode<T>* p = first();
    while (r--) p = p->succ;  // �ƶ�r�ε�Ŀ��λ��
    return p->data;
}

// �ж���������̶ȣ����������������
template <typename T>
int List<T>::disordered() const {
    int cnt = 0;
    for (ListNode<T>* p = first(); p->succ != trailer; p = p->succ) {
        if (p->data > p->succ->data) cnt++;
    }
    return cnt;
}

// ����Ԫ��e����β����ǰ����������
template <typename T>
ListNode<T>* List<T>::find(T const& e) const {
    return find(e, _size, trailer);
}

// ��Χ���ң���p��ǰ��n���ڵ����Ƿ���e
template <typename T>
ListNode<T>* List<T>::find(T const& e, int n, ListNode<T>* p) const {
    if (n < 0 || !valid(p)) throw "Invalid parameters";
    while (n--) {
        p = p->pred;
        if (p->data == e) return p;
    }
    return nullptr;
}

// �������e��Ĭ�ϲ���������
template <typename T>
ListNode<T>* List<T>::search(T const& e) const {
    return search(e, _size, trailer);
}

// ��Χ������ң���p��ǰ��n���ڵ㣬����<=e�����һ���ڵ�
template <typename T>
ListNode<T>* List<T>::search(T const& e, int n, ListNode<T>* p) const {
    if (n < 0 || !valid(p)) throw "Invalid parameters";
    while (n-- >= 0) {
        p = p->pred;
        if (p->data <= e) break;
    }
    return p;
}

// ��Χѡ���ڵ㣨��p��ʼ��n���ڵ㣩
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

// ��Χ�������ѡ�������㷨
template <typename T>
void List<T>::sort(ListNode<T>* p, int n) {
    if (!valid(p) || n <= 0 || n > _size) throw "Invalid range";
    switch (rand() % 3) {
    case 1: insertionSort(p, n); break;
    case 2: selectionSort(p, n); break;
    default: mergeSort(p, n); break;
    }
}

// ����ȥ�أ�ɾ���ظ�Ԫ�أ�������һ����
template <typename T>
int List<T>::deduplicate() {
    if (_size < 2) return 0;
    int oldSize = _size;
    ListNode<T>* p = first();
    int r = 0;
    while (p != trailer) {
        ListNode<T>* q = find(p->data, r, p);  // ����ǰr���ڵ����Ƿ����ظ�
        if (q) remove(q);
        else r++;
        p = p->succ;
    }
    return oldSize - _size;  // ����ɾ���Ľڵ���
}

// ����ȥ�أ�ɾ�������ظ�Ԫ�أ�������һ����
template <typename T>
int List<T>::uniquify() {
    if (_size < 2) return 0;
    int oldSize = _size;
    ListNode<T>* p = first();
    ListNode<T>* q = p->succ;
    while (q != trailer) {
        if (p->data == q->data) {
            remove(q);
            q = p->succ;  // ɾ����qָ��p���º��
        }
        else {
            p = q;
            q = q->succ;
        }
    }
    return oldSize - _size;  // ����ɾ���Ľڵ���
}

#endif // MYLIBRARY_LIST_H