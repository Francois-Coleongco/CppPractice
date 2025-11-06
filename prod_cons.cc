#include <array>
#include <memory>
#include <queue>
#include <stdexcept>
#include <utility>

class Sample {};

class BufferPool {
  int _num_buffers;
  std::queue<std::unique_ptr<std::array<Sample, 2048>>> _pool;

public:
  BufferPool(int num_buffers) {
    _num_buffers = num_buffers;
    for (int i = 0; i < _num_buffers; ++i) {
      _pool.push(std::make_unique<std::array<Sample, 2048>>());
    }
  }

    std::unique_ptr<std::array<Sample, 2048>> grabBuffer() {
    if (_num_buffers == 0) {
      throw std::runtime_error("no buffers available!");
    }
    std::unique_ptr<std::array<Sample, 2048>> ret = std::move(_pool.front());
    _pool.pop();
    --_num_buffers;
    return ret;
  }

      void returnBuffer(std::unique_ptr<std::array<Sample, 2048>> buf) {
    _pool.push(buf);
    ++_num_buffers;
  }

  ~BufferPool() {
    while (!_pool.empty()) {
      _pool.pop();
    }
  }
};
