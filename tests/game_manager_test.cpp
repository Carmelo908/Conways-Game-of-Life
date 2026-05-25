#include <chrono>
#include <future>
#include <memory>

#define CATCH_CONFIG_MAIN

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include <game_manager.hpp>

constexpr auto positionFilename = "testing_position.json";

const auto positionFilepath =
    std::filesystem::path(__FILE__).remove_filename().append(positionFilename);

std::unique_ptr<Position> openTestPosition()
{
  return std::make_unique<Position>(Position::parseJsonFile(positionFilepath));
}

TEST_CASE("Game Manager") // NOLINT
{
  SECTION("Yielding positions after constructing with a Position")
  {
    GameManager manager1{openTestPosition()};
    REQUIRE_FALSE(manager1.queueEmpty());
    for (int i = 0; i < 10; i++)
    {
      auto pos1 = std::async(std::launch::async, &GameManager::yieldPosition,
                             &manager1);
      auto status1 = pos1.wait_for(std::chrono::seconds(1));
      REQUIRE(status1 == std::future_status::ready);
      REQUIRE(pos1.get()->getCellsQuantity() > 0);
    }
  }

  SECTION("Yielding Positions after default constructing and then passing a "
          "Position")
  {
    GameManager manager2{};
    REQUIRE(manager2.queueEmpty());
    manager2.reset(openTestPosition());
    REQUIRE_FALSE(manager2.queueEmpty());
    for (int i = 0; i < 10; i++)
    {
      auto pos2 = std::async(std::launch::async, &GameManager::yieldPosition,
                             &manager2);
      auto status2 = pos2.wait_for(std::chrono::seconds(1));
      REQUIRE(status2 == std::future_status::ready);
      REQUIRE(pos2.get()->getCellsQuantity() > 0);
    }
  }

  SECTION("Yielding on empty manager expecting a exception to be thrown")
  {
    GameManager manager{};
    REQUIRE_THROWS(manager.yieldPosition());
  }
}