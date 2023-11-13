#include <deque>
#include <iostream>

namespace {

template <typename T>
class StackWithMin {
    std::deque<T> st_;
    std::deque<T> min_;

   public:
    void push(T x);
    bool empty();
    T back();
    void pop();
    T get_min();
};

template <typename T>
void StackWithMin<T>::push(T x) {
    st_.push_back(x);
    if (!min_.empty()) {
        x = std::min(x, min_.back());
    }
    min_.push_back(x);
}

template <typename T>
bool StackWithMin<T>::empty() {
    return st_.empty();
}

template <typename T>
T StackWithMin<T>::back() {
    return st_.back();
}

template <typename T>
void StackWithMin<T>::pop() {
    if (!empty()) {
        min_.pop_back();
        st_.pop_back();
    }
}

template <typename T>
T StackWithMin<T>::get_min() {
    return min_.back();
}

template <typename T>
class QueueWithMin {
    StackWithMin<T> st_in;
    StackWithMin<T> st_out;

   public:
    void push_back(T x);
    void pop();
    T get_min();
};

template <typename T>
void QueueWithMin<T>::push_back(T x) {
    st_in.push(x);
}

template <typename T>
void QueueWithMin<T>::pop() {
    if (st_out.empty()) {
        while (!st_in.empty()) {
            st_out.push(st_in.back());
            st_in.pop();
        }
    }

    st_out.pop();
}

template <typename T>
T QueueWithMin<T>::get_min() {
    if (st_in.empty()) {
        return st_out.get_min();
    }

    if (st_out.empty()) {
        return st_in.get_min();
    }

    return std::min(st_in.get_min(), st_out.get_min());
}

}  // end namespace

int main() {
    int N, K;
    std::cin >> N >> K;
    QueueWithMin<int> q;

    int x;
    for (int i = 0; i < K; ++i) {
        std::cin >> x;
        q.push_back(x);
    }
    std::cout << q.get_min() << ' ';

    for (int i = 0; i < N - K; ++i) {
        q.pop();
        std::cin >> x;
        q.push_back(x);
        std::cout << q.get_min() << ' ';
    }
}
