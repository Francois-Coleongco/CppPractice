#include <array>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

class Sample {};

class BufferPool {
  // multiple consumers
  // multiple producers

  std::mutex _mu;
  unsigned int _total_bufs;
  std::queue<std::unique_ptr<std::array<Sample, 2048>>> _pool;
  std::condition_variable cv_;

  BufferPool(int num_bufs) : _total_bufs(num_bufs) {
    for (int i = 0; i < num_bufs; ++i) {
      _pool.push(std::make_unique<std::array<Sample, 2048>>());
    }
  }

  std::unique_ptr<std::array<Sample, 2048>> grabBuf() {
    std::unique_lock<std::mutex> l(_mu);
    cv_.wait(l, [this] { return !_pool.empty(); });
    std::unique_ptr<std::array<Sample, 2048>> ret = std::move(_pool.front());
    _pool.pop();
    return ret;
  }

  void returnBuf(std::unique_ptr<std::array<Sample, 2048>> buf) {
    std::lock_guard<std::mutex> l(_mu);
    _pool.push(std::move(buf));
    cv_.notify_one();
  }

  ~BufferPool() {
    while (!_pool.empty()) {
      _pool.pop();
    }
  }
};
