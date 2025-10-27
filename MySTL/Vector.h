#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include <memory>

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

private:
    pointer _elem;       // 数据指针
    size_type _size;     // 当前元素数量
    size_type _capacity; // 容量
    static const size_type DEFAULT_CAPACITY = 3;

    // 低调用率：内部扩容逻辑
    void expand();
    // 低调用率：内部缩容逻辑
    void shrink();

public:
    // 高调用率：元素访问接口
    reference operator[](size_type r);
    const_reference operator[](size_type r) const;
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

    // 中调用率：修改操作
    void push_back(const T& e);
    void pop_back();
    size_type insert(size_type r, const T& e);
    T erase(size_type r);
    void clear() noexcept;

    // 中调用率：构造与析构
    vector();
    explicit vector(size_type c, size_type s = 0, const T& v = T());
    vector(const T* A, size_type lo, size_type hi);
    vector(const vector<T>& other);
    vector(vector<T>&& other) noexcept;
    ~vector();

    // 中调用率：赋值操作
    vector<T>& operator=(const vector<T>& other);
    vector<T>& operator=(vector<T>&& other) noexcept;

    // 低调用率：容量调整
    void reserve(size_type c);
    void shrink_to_fit();

    // 低调用率：算法相关
    size_type uniquify();
    size_type find(const T& e, size_type lo, size_type hi) const;
    template <typename VST>
    void traverse(VST& visitor);
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
void vector<T>::clear() noexcept {
    _size = 0;
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
    _capacity = other._capacity;
    _size = other._size;
    _elem = new T[_capacity];
    for (size_type i = 0; i < _size; ++i) {
        _elem[i] = other._elem[i];
    }
}

template <typename T>
vector<T>::vector(vector<T>&& other) noexcept
    : _elem(other._elem), _size(other._size), _capacity(other._capacity) {
    other._elem = nullptr;
    other._size = 0;
    other._capacity = 0;
}

template <typename T>
vector<T>::~vector() {
    delete[] _elem;
}

// 中调用率：赋值操作实现
template <typename T>
vector<T>& vector<T>::operator=(const vector<T>& other) {
    if (this == &other) return *this;
    delete[] _elem;
    _capacity = other._capacity;
    _size = other._size;
    _elem = new T[_capacity];
    for (size_type i = 0; i < _size; ++i) {
        _elem[i] = other._elem[i];
    }
    return *this;
}

template <typename T>
vector<T>& vector<T>::operator=(vector<T>&& other) noexcept {
    if (this == &other) return *this;
    delete[] _elem;
    _elem = other._elem;
    _size = other._size;
    _capacity = other._capacity;
    other._elem = nullptr;
    other._size = 0;
    other._capacity = 0;
    return *this;
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