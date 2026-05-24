#include "game_manager.hpp"

#include <cassert>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <utility>

GameManager::GameManager(std::unique_ptr<Position> &&position)
{
  if (position == nullptr)
  {
    return;
  }
  positionsQueue.pushBack(std::move(position));
  workerThread = std::jthread(&GameManager::processPositions, this);
}

GameManager::GameManager()
{
  workerThread = std::jthread(&GameManager::processPositions, this);
}

GameManager::~GameManager() { stopFlag = true; }

std::unique_ptr<Position> GameManager::yieldPosition()
{
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
  if (workerThread.joinable())
  {
    resetFlag = true;
  } else
  {
    workerThread = std::jthread(&GameManager::processPositions, this);
  }
}

bool GameManager::empty() const { return positionsQueue.empty(); }

void GameManager::processPositions()
{
  while (true)
  {
    if (positionsQueue.empty())
    {
      std::this_thread::yield();
      continue;
    }
    auto next{positionsQueue.copyBack()};
    next->advanceGen();
    while (positionsQueue.size() >= queueLimit)
    {
      std::this_thread::yield();
    }
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

GameManager::SharedQueue::SharedQueue(std::unique_ptr<Position> &&position)
{
  const std::unique_lock lock{mutex};
  queue.push(std::move(position));
}

std::unique_ptr<Position> GameManager::SharedQueue::pop()
{
  while (true)
  {
    const std::unique_lock lock{mutex};
    if (queue.empty())
    {
      throw std::runtime_error("Cannon pop empty GameManager queue");
    }
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
  const std::unique_lock lock{mutex};
  return queue.empty() ? nullptr : std::make_unique<Position>(*queue.back());
}

void GameManager::SharedQueue::pushBack(std::unique_ptr<Position> &&position)
{
  const std::unique_lock lock{mutex};
  queue.push(std::move(position));
}

size_t GameManager::SharedQueue::size() const
{
  const std::unique_lock lock{mutex};
  return queue.size();
}

bool GameManager::SharedQueue::empty() const
{
  const std::unique_lock lock{mutex};
  return queue.empty();
}

void GameManager::SharedQueue::clear()
{
  const std::unique_lock lock{mutex};
  std::queue<std::unique_ptr<Position>> tmp{};
  queue.swap(tmp);
}