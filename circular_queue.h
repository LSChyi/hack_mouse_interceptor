#ifndef CIRCULAR_QUEUE_H_
#define CIRCULAR_QUEUE_H_

#define QUEUE_SIZE 10
#define PtrMove(x) (x = (x + 1) % QUEUE_SIZE)

#include <stdint.h>

template <class T> class CircularQueue {
public:
  CircularQueue() : start_idx_(0), end_idx_(0) {};
  void Reset() { start_idx_ = end_idx_ = 0; };
  bool IsEmpty() { return start_idx_ == end_idx_; };
  void Enqueue(T t) {
    queue_[end_idx_] = t;
    PtrMove(end_idx_);
    if (end_idx_ == start_idx_) {
      PtrMove(start_idx_);
    }
  };
  T Dequeue() {
    // Caller must ensure queue is not empty.
    T ret = queue_[start_idx_];
    PtrMove(start_idx_);
    return ret;
  };

private:
  T queue_[QUEUE_SIZE];
  uint8_t start_idx_;
  uint8_t end_idx_;
};

#endif
