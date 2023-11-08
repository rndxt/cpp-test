#include <iostream>
#include <deque>

struct stack_with_min {
  std::deque<int> st_;
  std::deque<int> min_;

  void push(int x) {
    st_.push_back(x);
    if (!min_.empty()) {
      x = std::min(x, min_.back());
    }
    min_.push_back(x);
  }

  bool empty() {
    return st_.empty();
  }

  int back() {
    return st_.back();
  }

  void pop() {
    if (!empty()) {
      min_.pop_back();
      st_.pop_back();
    }
  }

  int minim() {
    return min_.back();
  }
}; 

struct queue_with_min {
  stack_with_min st_in;
  stack_with_min st_out;

  void push_back(int x) {
    st_in.push(x);
  }

  void pop() {
    if (st_out.empty()) {
      while (!st_in.empty()) {
        st_out.push(st_in.back());
        st_in.pop();
      }
    }

    st_out.pop();
  }

  int minim() {
    if (st_in.empty()) {
      return st_out.minim();
    }

    if (st_out.empty()) {
      return st_in.minim();
    }

    return std::min(st_in.minim(), st_out.minim());
  }
};


int main() {
  int N, K;
  std::cin >> N >> K;
  queue_with_min q;
  
  int x;
  for (int i = 0; i < K; ++i) {
    std::cin >> x;
    q.push_back(x);
  }
  std::cout << q.minim() << ' ';

  for (int i = 0; i < N-K; ++i) {
    q.pop();
    std::cin >> x;
    q.push_back(x);
    std::cout << q.minim() << ' ';
  }
}
