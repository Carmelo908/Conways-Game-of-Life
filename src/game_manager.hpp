#include <atomic>
#include <cstddef>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include "position.hpp"

/// @brief represents a work manager for Position that process them
/// asynchronously and puts them in a queue for the user to consume them.
///
/// The queue has a maximun size of 256 and has to have at least one element to
/// process from.
class GameManager
{
public:
  /// @brief constructs the GameManager, creates the thread and starts
  /// processing from position.
  /// @param position the starting position to process. if null, the constructor
  /// doesn't start processing.
  GameManager(std::unique_ptr<Position> &&position);

  /// @brief defaults constructs the GameManager and creates the worker thread.
  /// Doesn't start processing until restart() is called with a non null
  /// argument.
  GameManager();

  /// @brief signals the working thread to stop.
  ~GameManager();

  GameManager(const GameManager &) = delete;
  GameManager(GameManager &&) = delete;
  GameManager &operator=(const GameManager &) = delete;
  GameManager &operator=(GameManager &&) = delete;

  /// @return the next position in the queue, may lock the current thread if the
  /// queue size equals 1.
  /// @throw std::runtime_error if called while empty equals true.
  std::unique_ptr<Position> yieldPosition();

  /// @brief Makes the GameManager clear its queue and start processing another
  /// position.
  /// @param position the new position to manage and own.
  void reset(std::unique_ptr<Position> &&position);

  /// @return wether the queue is empty, which would mean the Manager isn't
  /// processing any Position.
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