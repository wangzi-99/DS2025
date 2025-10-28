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
            Node* next;  // ָ��ջ��������һ�ڵ�
            Node* prev;  // ָ��ջ�׷�����һ�ڵ�

            Node(const T& val, Node* n = nullptr, Node* p = nullptr)
                : data(val), next(n), prev(p) {
            }
            Node(T&& val, Node* n = nullptr, Node* p = nullptr)
                : data(std::move(val)), next(n), prev(p) {
            }
        };

        Node* top_;       // ջ��ָ�루��ߵ����ʷ��ʵ㣩
        Node* bottom_;    // ջ��ָ��
        size_t size_;     // Ԫ�ؼ�������Ƶ��ѯ��

        // ��Ƶ�ڲ����ߣ��ͷ����нڵ㣨����/��ձص���
        void clear_nodes() noexcept {
            while (top_) {
                Node* temp = top_;
                top_ = top_->next;
                delete temp;
            }
            bottom_ = nullptr;
            size_ = 0;
        }

        // ��Ƶ�ڲ����ߣ����ƽڵ㣨��������/��ֵ�ص���
        void copy_nodes(const Stack& other) {
            Node* cur = other.bottom_;
            while (cur) {
                push(cur->data);
                cur = cur->prev;
            }
        }

    public:
        // ���ߵ����ʣ����Ĳ�������ջ��
        void push(const T& value) {
            Node* new_node = new Node(value, top_);
            if (top_) top_->prev = new_node;
            else bottom_ = new_node;  // ��ջʱͬ������ջ��
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

        // ���ߵ����ʣ����Ĳ�������ȡջ����
        T& top() {
            if (empty()) throw std::underflow_error("Stack empty (top)");
            return top_->data;
        }

        const T& top() const {
            if (empty()) throw std::underflow_error("Stack empty (top)");
            return top_->data;
        }

        // ���ߵ����ʣ����Ĳ�������ջ��
        void pop() {
            if (empty()) throw std::underflow_error("Stack empty (pop)");
            Node* temp = top_;
            top_ = top_->next;
            if (top_) top_->prev = nullptr;
            else bottom_ = nullptr;  // ջ��ʱͬ�����ջ��
            delete temp;
            size_--;
        }

        // �ߵ����ʣ�״̬��ѯ
        bool empty() const noexcept { return size_ == 0; }
        size_t size() const noexcept { return size_; }

        // �е����ʣ�����������
        Stack() noexcept : top_(nullptr), bottom_(nullptr), size_(0) {}
        ~Stack() noexcept { clear_nodes(); }

        // �е����ʣ���������
        void clear() noexcept { clear_nodes(); }

        // �е����ʣ���������
        void swap(Stack& other) noexcept {
            using std::swap;
            swap(top_, other.top_);
            swap(bottom_, other.bottom_);
            swap(size_, other.size_);
        }

        // �е͵����ʣ�����/�ƶ�����
        Stack(const Stack& other) : Stack() { copy_nodes(other); }
        Stack(Stack&& other) noexcept
            : top_(other.top_), bottom_(other.bottom_), size_(other.size_) {
            other.top_ = other.bottom_ = nullptr;
            other.size_ = 0;
        }

        // �е͵����ʣ���ʼ���б���
        Stack(std::initializer_list<T> init) : Stack() {
            for (const auto& val : init) push(val);
        }

        // �е͵����ʣ���ֵ����
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

        // �͵����ʣ���չ���ܣ�ջ�׷��ʣ�
        T& bottom() {
            if (empty()) throw std::underflow_error("Stack empty (bottom)");
            return bottom_->data;
        }

        const T& bottom() const {
            if (empty()) throw std::underflow_error("Stack empty (bottom)");
            return bottom_->data;
        }

        // �͵����ʣ���չ���ܣ�ջ��ת��
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

    // �е����ʣ��ǳ�Ա��������
    template <typename T>
    void swap(Stack<T>& lhs, Stack<T>& rhs) noexcept {
        lhs.swap(rhs);
    }

} // namespace CustomContainer

#endif // CUSTOM_STACK_H