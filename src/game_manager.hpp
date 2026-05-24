#include <atomic>
#include <cstddef>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include "position.hpp"

/// @brief represents a manager that advances a position in a separate thread,
/// puts every copy in a queue and can pop the queue and return the next
/// Position on request.
class GameManager
{
public:
  GameManager(std::unique_ptr<Position> &&position);
  GameManager();
  ~GameManager();

  GameManager(const GameManager &) = delete;
  GameManager(GameManager &&) = delete;
  GameManager &operator=(const GameManager &) = delete;
  GameManager &operator=(GameManager &&) = delete;

  std::unique_ptr<Position> yieldPosition();

  void reset(std::unique_ptr<Position> &&position);

  bool empty() const;

private:
  void processPositions();

  class SharedQueue
  {
  public:
    SharedQueue(std::unique_ptr<Position> &&position);
    SharedQueue() = default;

    std::unique_ptr<Position> pop();
    std::unique_ptr<Position> copyBack() const;
    void pushBack(std::unique_ptr<Position> &&position);
    size_t size() const;
    bool empty() const;
    void clear();

  private:
    mutable std::mutex mutex;
    std::queue<std::unique_ptr<Position>> queue;
  };

  SharedQueue positionsQueue;
  std::atomic_bool resetFlag;
  std::atomic_bool stopFlag;
  std::jthread workerThread;

  static constexpr int queueLimit = 256;
};