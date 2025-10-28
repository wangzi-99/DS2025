#ifndef CUSTOM_STACK_H
#define CUSTOM_STACK_H

#include <cstddef>
#include <utility>
#include <stdexcept>

namespace CustomContainer {

    template <typename T>
    class Stack {
    private:
        struct Node {
            T data;
            Node* next;  // 指向栈顶方向下一节点
            Node* prev;  // 指向栈底方向上一节点

            Node(const T& val, Node* n = nullptr, Node* p = nullptr)
                : data(val), next(n), prev(p) {
            }
            Node(T&& val, Node* n = nullptr, Node* p = nullptr)
                : data(std::move(val)), next(n), prev(p) {
            }
        };

        Node* top_;       // 栈顶指针（最高调用率访问点）
        Node* bottom_;    // 栈底指针
        size_t size_;     // 元素计数（高频查询）

        // 高频内部工具：释放所有节点（析构/清空必调）
        void clear_nodes() noexcept {
            while (top_) {
                Node* temp = top_;
                top_ = top_->next;
                delete temp;
            }
            bottom_ = nullptr;
            size_ = 0;
        }

        // 中频内部工具：复制节点（拷贝构造/赋值必调）
        void copy_nodes(const Stack& other) {
            Node* cur = other.bottom_;
            while (cur) {
                push(cur->data);
                cur = cur->prev;
            }
        }

    public:
        // 极高调用率：核心操作（入栈）
        void push(const T& value) {
            Node* new_node = new Node(value, top_);
            if (top_) top_->prev = new_node;
            else bottom_ = new_node;  // 空栈时同步更新栈底
            top_ = new_node;
            size_++;
        }

        void push(T&& value) {
            Node* new_node = new Node(std::move(value), top_);
            if (top_) top_->prev = new_node;
            else bottom_ = new_node;
            top_ = new_node;
            size_++;
        }

        // 极高调用率：核心操作（获取栈顶）
        T& top() {
            if (empty()) throw std::underflow_error("Stack empty (top)");
            return top_->data;
        }

        const T& top() const {
            if (empty()) throw std::underflow_error("Stack empty (top)");
            return top_->data;
        }

        // 极高调用率：核心操作（出栈）
        void pop() {
            if (empty()) throw std::underflow_error("Stack empty (pop)");
            Node* temp = top_;
            top_ = top_->next;
            if (top_) top_->prev = nullptr;
            else bottom_ = nullptr;  // 栈空时同步清空栈底
            delete temp;
            size_--;
        }

        // 高调用率：状态查询
        bool empty() const noexcept { return size_ == 0; }
        size_t size() const noexcept { return size_; }

        // 中调用率：构造与析构
        Stack() noexcept : top_(nullptr), bottom_(nullptr), size_(0) {}
        ~Stack() noexcept { clear_nodes(); }

        // 中调用率：批量操作
        void clear() noexcept { clear_nodes(); }

        // 中调用率：交换操作
        void swap(Stack& other) noexcept {
            using std::swap;
            swap(top_, other.top_);
            swap(bottom_, other.bottom_);
            swap(size_, other.size_);
        }

        // 中低调用率：拷贝/移动构造
        Stack(const Stack& other) : Stack() { copy_nodes(other); }
        Stack(Stack&& other) noexcept
            : top_(other.top_), bottom_(other.bottom_), size_(other.size_) {
            other.top_ = other.bottom_ = nullptr;
            other.size_ = 0;
        }

        // 中低调用率：初始化列表构造
        Stack(std::initializer_list<T> init) : Stack() {
            for (const auto& val : init) push(val);
        }

        // 中低调用率：赋值操作
        Stack& operator=(const Stack& other) {
            if (this != &other) {
                clear();
                copy_nodes(other);
            }
            return *this;
        }

        Stack& operator=(Stack&& other) noexcept {
            if (this != &other) {
                clear_nodes();
                top_ = other.top_;
                bottom_ = other.bottom_;
                size_ = other.size_;
                other.top_ = other.bottom_ = nullptr;
                other.size_ = 0;
            }
            return *this;
        }

        // 低调用率：扩展功能（栈底访问）
        T& bottom() {
            if (empty()) throw std::underflow_error("Stack empty (bottom)");
            return bottom_->data;
        }

        const T& bottom() const {
            if (empty()) throw std::underflow_error("Stack empty (bottom)");
            return bottom_->data;
        }

        // 低调用率：扩展功能（栈反转）
        void reverse() noexcept {
            if (size_ <= 1) return;
            Node* current = top_;
            Node* temp = nullptr;
            while (current) {
                temp = current->next;
                current->next = current->prev;
                current->prev = temp;
                current = temp;
            }
            swap(top_, bottom_);
        }
    };

    // 中调用率：非成员交换函数
    template <typename T>
    void swap(Stack<T>& lhs, Stack<T>& rhs) noexcept {
        lhs.swap(rhs);
    }

} // namespace CustomContainer

#endif // CUSTOM_STACK_H