#pragma once

template<typename T, unsigned int N>
struct RingBuffer {
  T data[N] = {0};
  unsigned int head = 0;
  unsigned int tail = 0;

  void push(T value) {
    data[head] = value;
    head = head + 1;
    if ( head >= N) {
      head = 0;
    }
  }

  [[nodiscard]] T pop() {
    T value = data[tail];
    if (tail != head){
      tail = tail + 1;
      if (tail >= N)
        tail = 0;
    }
    return value;
  }

  [[nodiscard]] bool empty() const {
    return head == tail;
  }

  [[nodiscard]] unsigned int capacity() const {
    return N;
  }

  [[nodiscard]] unsigned int size() const {
    if (head >= tail)
      return head - tail;
    else
      return N - tail + head;
  }
};
