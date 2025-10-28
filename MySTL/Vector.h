#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include <cassert>

template <typename T>
class vector {
public:
    // 类型别名（高调用率：基础类型定义，所有操作都依赖）
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_pointer = const T*;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    // 补充迭代器类型定义
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    pointer _elem;       // 数据指针
    size_type _size;     // 当前元素数量
    size_type _capacity; // 容量
    static const size_type DEFAULT_CAPACITY = 3;

    // 低调用率：内部扩容逻辑
    void expand();
    // 低调用率：内部缩容逻辑
    void shrink();
    // 新增：内存分配与初始化
    void allocate_and_copy(size_type new_cap, const_pointer first, const_pointer last);

public:
    // 高调用率：元素访问接口
    reference operator[](size_type r);
    const_reference operator[](size_type r) const;
    reference at(size_type r);
    const_reference at(size_type r) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    pointer data() noexcept;
    const_pointer data() const noexcept;

    // 高调用率：容量相关接口
    size_type size() const noexcept;
    size_type capacity() const noexcept;
    bool empty() const noexcept;
    void resize(size_type new_size, const T& val = T());

    // 中调用率：修改操作
    void push_back(const T& e);
    void pop_back();
    iterator insert(iterator pos, const T& e);
    size_type insert(size_type r, const T& e);
    iterator erase(iterator pos);
    T erase(size_type r);
    void clear() noexcept;
    void swap(vector<T>& other) noexcept;

    // 中调用率：构造与析构
    vector();
    explicit vector(size_type c, size_type s = 0, const T& v = T());
    vector(const T* A, size_type lo, size_type hi);
    vector(const vector<T>& other);
    vector(vector<T>&& other) noexcept;
    // 新增：初始化列表构造
    vector(std::initializer_list<T> init);
    ~vector();

    // 中调用率：赋值操作
    vector<T>& operator=(const vector<T>& other);
    vector<T>& operator=(vector<T>&& other) noexcept;
    vector<T>& operator=(std::initializer_list<T> init);

    // 高调用率：迭代器接口
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // 低调用率：容量调整
    void reserve(size_type c);
    void shrink_to_fit();

    // 低调用率：算法相关
    size_type uniquify();
    size_type find(const T& e, size_type lo, size_type hi) const;
    template <typename VST>
    void traverse(VST& visitor);
    // 新增：排序算法
    void sort(size_type lo, size_type hi);
    // 新增：比较运算符
    bool operator==(const vector<T>& other) const;
    bool operator!=(const vector<T>& other) const;
    bool operator<(const vector<T>& other) const;
    bool operator>(const vector<T>& other) const;
    bool operator<=(const vector<T>& other) const;
    bool operator>=(const vector<T>& other) const;
};

// 高调用率：元素访问实现
template <typename T>
typename vector<T>::reference vector<T>::operator[](size_type r) {
    assert(r < _size);
    return _elem[r];
}

template <typename T>
typename vector<T>::const_reference vector<T>::operator[](size_type r) const {
    assert(r < _size);
    return _elem[r];
}

template <typename T>
typename vector<T>::reference vector<T>::at(size_type r) {
    if (r >= _size) {
        throw std::out_of_range("vector::at - index out of range");
    }
    return _elem[r];
}

template <typename T>
typename vector<T>::const_reference vector<T>::at(size_type r) const {
    if (r >= _size) {
        throw std::out_of_range("vector::at - index out of range");
    }
    return _elem[r];
}

template <typename T>
typename vector<T>::reference vector<T>::front() {
    assert(!empty());
    return _elem[0];
}

template <typename T>
typename vector<T>::const_reference vector<T>::front() const {
    assert(!empty());
    return _elem[0];
}

template <typename T>
typename vector<T>::reference vector<T>::back() {
    assert(!empty());
    return _elem[_size - 1];
}

template <typename T>
typename vector<T>::const_reference vector<T>::back() const {
    assert(!empty());
    return _elem[_size - 1];
}

template <typename T>
typename vector<T>::pointer vector<T>::data() noexcept {
    return _elem;
}

template <typename T>
typename vector<T>::const_pointer vector<T>::data() const noexcept {
    return _elem;
}

// 高调用率：容量查询实现
template <typename T>
typename vector<T>::size_type vector<T>::size() const noexcept {
    return _size;
}

template <typename T>
typename vector<T>::size_type vector<T>::capacity() const noexcept {
    return _capacity;
}

template <typename T>
bool vector<T>::empty() const noexcept {
    return _size == 0;
}

template <typename T>
void vector<T>::resize(size_type new_size, const T& val) {
    if (new_size < _size) {
        _size = new_size;
        shrink();
    }
    else if (new_size > _size) {
        reserve(new_size);
        for (size_type i = _size; i < new_size; ++i) {
            _elem[i] = val;
        }
        _size = new_size;
    }
}

// 中调用率：修改操作实现
template <typename T>
void vector<T>::push_back(const T& e) {
    insert(_size, e);
}

template <typename T>
void vector<T>::pop_back() {
    assert(!empty());
    erase(_size - 1);
}

template <typename T>
typename vector<T>::size_type vector<T>::insert(size_type r, const T& e) {
    assert(r <= _size);
    expand();
    for (size_type i = _size; i > r; --i) {
        _elem[i] = _elem[i - 1];
    }
    _elem[r] = e;
    ++_size;
    return r;
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(iterator pos, const T& e) {
    size_type r = pos - begin();
    insert(r, e);
    return begin() + r;
}

template <typename T>
T vector<T>::erase(size_type r) {
    assert(r < _size);
    T old_val = _elem[r];
    for (size_type i = r; i < _size - 1; ++i) {
        _elem[i] = _elem[i + 1];
    }
    --_size;
    shrink();
    return old_val;
}

template <typename T>
typename vector<T>::iterator vector<T>::erase(iterator pos) {
    size_type r = pos - begin();
    erase(r);
    return begin() + r;
}

template <typename T>
void vector<T>::clear() noexcept {
    _size = 0;
    shrink_to_fit(); // 清理时缩容更合理
}

template <typename T>
void vector<T>::swap(vector<T>& other) noexcept {
    std::swap(_elem, other._elem);
    std::swap(_size, other._size);
    std::swap(_capacity, other._capacity);
}

// 中调用率：构造与析构实现
template <typename T>
vector<T>::vector() : _elem(nullptr), _size(0), _capacity(0) {}

template <typename T>
vector<T>::vector(size_type c, size_type s, const T& v) : _capacity(c), _size(s) {
    assert(s <= c);
    _elem = new T[_capacity];
    for (size_type i = 0; i < _size; ++i) {
        _elem[i] = v;
    }
}

template <typename T>
vector<T>::vector(const T* A, size_type lo, size_type hi) {
    assert(lo < hi);
    _size = hi - lo;
    _capacity = _size;
    _elem = new T[_capacity];
    for (size_type i = 0; i < _size; ++i) {
        _elem[i] = A[lo + i];
    }
}

template <typename T>
vector<T>::vector(const vector<T>& other) {
    allocate_and_copy(other._capacity, other._elem, other._elem + other._size);
}

template <typename T>
vector<T>::vector(vector<T>&& other) noexcept
    : _elem(other._elem), _size(other._size), _capacity(other._capacity) {
    other._elem = nullptr;
    other._size = 0;
    other._capacity = 0;
}

template <typename T>
vector<T>::vector(std::initializer_list<T> init) {
    _size = init.size();
    _capacity = _size;
    _elem = new T[_capacity];
    std::copy(init.begin(), init.end(), _elem);
}

template <typename T>
vector<T>::~vector() {
    delete[] _elem;
}

// 中调用率：赋值操作实现
template <typename T>
vector<T>& vector<T>::operator=(const vector<T>& other) {
    if (this != &other) {
        allocate_and_copy(other._capacity, other._elem, other._elem + other._size);
    }
    return *this;
}

template <typename T>
vector<T>& vector<T>::operator=(vector<T>&& other) noexcept {
    if (this != &other) {
        delete[] _elem;
        _elem = other._elem;
        _size = other._size;
        _capacity = other._capacity;
        other._elem = nullptr;
        other._size = 0;
        other._capacity = 0;
    }
    return *this;
}

template <typename T>
vector<T>& vector<T>::operator=(std::initializer_list<T> init) {
    if (init.size() > _capacity) {
        delete[] _elem;
        _capacity = init.size();
        _elem = new T[_capacity];
    }
    _size = init.size();
    std::copy(init.begin(), init.end(), _elem);
    return *this;
}

// 高调用率：迭代器实现
template <typename T>
typename vector<T>::iterator vector<T>::begin() noexcept {
    return _elem;
}

template <typename T>
typename vector<T>::const_iterator vector<T>::begin() const noexcept {
    return _elem;
}

template <typename T>
typename vector<T>::const_iterator vector<T>::cbegin() const noexcept {
    return _elem;
}

template <typename T>
typename vector<T>::iterator vector<T>::end() noexcept {
    return _elem + _size;
}

template <typename T>
typename vector<T>::const_iterator vector<T>::end() const noexcept {
    return _elem + _size;
}

template <typename T>
typename vector<T>::const_iterator vector<T>::cend() const noexcept {
    return _elem + _size;
}

template <typename T>
typename vector<T>::reverse_iterator vector<T>::rbegin() noexcept {
    return reverse_iterator(end());
}

template <typename T>
typename vector<T>::const_reverse_iterator vector<T>::rbegin() const noexcept {
    return const_reverse_iterator(end());
}

template <typename T>
typename vector<T>::const_reverse_iterator vector<T>::crbegin() const noexcept {
    return const_reverse_iterator(end());
}

template <typename T>
typename vector<T>::reverse_iterator vector<T>::rend() noexcept {
    return reverse_iterator(begin());
}

template <typename T>
typename vector<T>::const_reverse_iterator vector<T>::rend() const noexcept {
    return const_reverse_iterator(begin());
}

template <typename T>
typename vector<T>::const_reverse_iterator vector<T>::crend() const noexcept {
    return const_reverse_iterator(begin());
}

// 低调用率：内部工具函数
template <typename T>
void vector<T>::allocate_and_copy(size_type new_cap, const_pointer first, const_pointer last) {
    _capacity = new_cap;
    _size = last - first;
    _elem = new T[_capacity];
    std::copy(first, last, _elem);
}

// 低调用率：内部扩容实现
template <typename T>
void vector<T>::expand() {
    if (_size < _capacity) return;
    _capacity = (_capacity == 0) ? DEFAULT_CAPACITY : _capacity * 2;
    pointer new_elem = new T[_capacity];
    for (size_type i = 0; i < _size; ++i) {
        new_elem[i] = _elem[i];
    }
    delete[] _elem;
    _elem = new_elem;
}

// 低调用率：内部缩容实现
template <typename T>
void vector<T>::shrink() {
    if (_capacity <= DEFAULT_CAPACITY || _size * 4 >= _capacity) return;
    size_type new_cap = std::max(_size * 2, DEFAULT_CAPACITY);
    pointer new_elem = new T[new_cap];
    for (size_type i = 0; i < _size; ++i) {
        new_elem[i] = _elem[i];
    }
    delete[] _elem;
    _elem = new_elem;
    _capacity = new_cap;
}

// 低调用率：容量调整实现
template <typename T>
void vector<T>::reserve(size_type c) {
    if (c <= _capacity) return;
    pointer new_elem = new T[c];
    for (size_type i = 0; i < _size; ++i) {
        new_elem[i] = _elem[i];
    }
    delete[] _elem;
    _elem = new_elem;
    _capacity = c;
}

template <typename T>
void vector<T>::shrink_to_fit() {
    if (_size == _capacity) return;
    pointer new_elem = new T[_size];
    for (size_type i = 0; i < _size; ++i) {
        new_elem[i] = _elem[i];
    }
    delete[] _elem;
    _elem = new_elem;
    _capacity = _size;
}

// 低调用率：算法相关实现
template <typename T>
typename vector<T>::size_type vector<T>::uniquify() {
    if (_size < 2) return 0;
    size_type old_size = _size;
    size_type i = 0;
    for (size_type j = 1; j < _size; ++j) {
        if (_elem[i] != _elem[j]) {
            _elem[++i] = _elem[j];
        }
    }
    _size = i + 1;
    shrink();
    return old_size - _size;
}

template <typename T>
typename vector<T>::size_type vector<T>::find(const T& e, size_type lo, size_type hi) const {
    assert(lo <= hi && hi <= _size);
    for (size_type i = hi - 1; i >= lo; --i) {
        if (_elem[i] == e) return i;
    }
    return static_cast<size_type>(-1);
}

template <typename T>
template <typename VST>
void vector<T>::traverse(VST& visitor) {
    for (size_type i = 0; i < _size; ++i) {
        visitor(_elem[i]);
    }
}

// 快速排序实现
template <typename T>
void vector<T>::sort(size_type lo, size_type hi) {
    if (hi - lo < 2) return;
    size_type pivot = lo + rand() % (hi - lo);
    std::swap(_elem[lo], _elem[pivot]);
    T pivot_val = _elem[lo];
    size_type i = lo, j = hi;
    while (true) {
        while (++i < hi && _elem[i] < pivot_val);
        while (_elem[--j] > pivot_val);
        if (i >= j) break;
        std::swap(_elem[i], _elem[j]);
    }
    std::swap(_elem[lo], _elem[j]);
    sort(lo, j);
    sort(j + 1, hi);
}

// 比较运算符实现
template <typename T>
bool vector<T>::operator==(const vector<T>& other) const {
    if (_size != other._size) return false;
    for (size_type i = 0; i < _size; ++i) {
        if (_elem[i] != other._elem[i]) return false;
    }
    return true;
}

template <typename T>
bool vector<T>::operator!=(const vector<T>& other) const {
    return !(*this == other);
}

template <typename T>
bool vector<T>::operator<(const vector<T>& other) const {
    size_type min_size = std::min(_size, other._size);
    for (size_type i = 0; i < min_size; ++i) {
        if (_elem[i] < other._elem[i]) return true;
        if (_elem[i] > other._elem[i]) return false;
    }
    return _size < other._size;
}

template <typename T>
bool vector<T>::operator>(const vector<T>& other) const {
    return other < *this;
}

template <typename T>
bool vector<T>::operator<=(const vector<T>& other) const {
    return !(*this > other);
}

template <typename T>
bool vector<T>::operator>=(const vector<T>& other) const {
    return !(*this < other);
}