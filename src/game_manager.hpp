#include <atomic>
#include <cstddef>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include "position.hpp"

/// @brief represents a work manager for Position that process them
/// asynchronously and puts them in a queue for the consumer to get from.
///
/// The queue has a maximun size of 256 and has to have at least one element to
/// process from.
class GameManager
{
public:
  /// @brief constructs the GameManager, creates the thread and starts
  /// processing from position.
  /// @param position the starting position to process. if null, the worker
  /// thread doesn't start processing until reset() is called with a non null
  /// arguments.
  GameManager(std::unique_ptr<Position> &&position);

  /// @brief defaults constructs the GameManager and creates the worker thread.
  /// Doesn't start processing until reset() is called with a non null
  /// argument.
  GameManager();

  /// @brief signals the working thread to stop.
  ~GameManager();

  GameManager(const GameManager &) = delete;
  GameManager(GameManager &&) = delete;
  GameManager &operator=(const GameManager &) = delete;
  GameManager &operator=(GameManager &&) = delete;

  /// @return the next position in the queue.
  ///
  /// may lock the current thread until
  /// the Position is advanced if the queue size equals 1.
  ///
  /// @throw std::runtime_error if called while empty() == true.
  std::unique_ptr<Position> yieldPosition();

  /// @brief clears the GameManager queue and starts processing from another
  /// position.
  /// @param position the new position to manage and own.
  void reset(std::unique_ptr<Position> &&position);

  /// @return the elements amount of the manager's queue.
  size_t queueSize() const;

  /// @return wether the queue is empty, which would mean the Manager isn't
  /// managind or processing any Position.
  bool queueEmpty() const;

private:
  void processPositions();

  void waitEmptyQueue() const;

  void waitFullQueue() const;

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