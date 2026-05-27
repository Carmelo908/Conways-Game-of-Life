#include "game_manager.hpp"

#include <cassert>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <utility>

GameManager::GameManager(std::unique_ptr<Position> &&position)
  : workerThread{std::jthread(&GameManager::processPositions, this)}
{
  if (position != nullptr)
  {
    positionsQueue.pushBack(std::move(position));
  }
}

GameManager::GameManager()
  : workerThread{std::jthread(&GameManager::processPositions, this)}
{}

GameManager::~GameManager() { stopFlag = true; }

std::unique_ptr<Position> GameManager::yieldPosition()
{
  if (positionsQueue.empty())
  {
    throw std::runtime_error("Cannon pop empty GameManager queue");
  }
  return positionsQueue.pop();
}

void GameManager::reset(std::unique_ptr<Position> &&position)
{
  if (position == nullptr)
  {
    return;
  }
  positionsQueue.clear();
  positionsQueue.pushBack(std::move(position));
  resetFlag = true;
}

size_t GameManager::queueSize() const { return positionsQueue.size(); }

bool GameManager::queueEmpty() const { return positionsQueue.empty(); }

void GameManager::processPositions()
{
  while (true)
  {
    if (positionsQueue.empty())
    {
      if (stopFlag)
      {
        break;
      }
      continue;
    }
    auto next{positionsQueue.copyBack()};
    next->advanceGen();
    waitFullQueue();
    if (stopFlag)
    {
      break;
    }
    if (resetFlag)
    {
      resetFlag = false;
      continue;
    }
    positionsQueue.pushBack(std::move(next));
  }
}

void GameManager::waitFullQueue() const
{
  while (positionsQueue.size() >= queueLimit)
  {
    std::this_thread::yield();
  }
}

GameManager::SharedQueue::SharedQueue(std::unique_ptr<Position> &&position)
{
  std::scoped_lock lock{mutex};
  queue.push(std::move(position));
}

std::unique_ptr<Position> GameManager::SharedQueue::pop()
{
  while (true)
  {
    const std::scoped_lock lock{mutex};
    if (queue.size() > 1)
    {
      auto result = std::move(queue.front());
      queue.pop();
      return result;
    }
  }
}

std::unique_ptr<Position> GameManager::SharedQueue::copyBack() const
{
  std::scoped_lock lock{mutex};
  return queue.empty() ? nullptr : std::make_unique<Position>(*queue.back());
}

void GameManager::SharedQueue::pushBack(std::unique_ptr<Position> &&position)
{
  std::scoped_lock lock{mutex};
  queue.push(std::move(position));
}

size_t GameManager::SharedQueue::size() const
{
  std::scoped_lock lock{mutex};
  return queue.size();
}

bool GameManager::SharedQueue::empty() const
{
  std::scoped_lock lock{mutex};
  return queue.empty();
}

void GameManager::SharedQueue::clear()
{
  std::scoped_lock lock{mutex};
  std::queue<std::unique_ptr<Position>> tmp{};
  queue.swap(tmp);
}