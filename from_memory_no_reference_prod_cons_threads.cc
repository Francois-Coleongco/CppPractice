#include <atomic>
#include <condition_variable>
#include <mutex>
#include <fcntl.h>
#include <queue>


// did this basically from memory, should be good with the mutex/conditional variable implementation for the mutex based producer-consumer now

struct Packet {};

class DataQueue {
  std::mutex _mu;
  std::queue<Packet> _qu;
  std::condition_variable _cv;

public:
  void push(const Packet &p) {
    std::unique_lock<std::mutex> lock(_mu);
    _qu.push(p);
    _cv.notify_one();
  }

  Packet pop() {
    std::unique_lock<std::mutex> lock(_mu);
    _cv.wait(lock, [this] { return !_qu.empty(); });
    Packet ret = _qu.front();
    _qu.pop();
    return ret;
  }
};
