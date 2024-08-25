#ifndef CPP3_BRICKGAME_2_0_1_SRC_BRICK_GAME_SNAKE_BACKEND_SNAKE_MODEL_H_
#define CPP3_BRICKGAME_2_0_1_SRC_BRICK_GAME_SNAKE_BACKEND_SNAKE_MODEL_H_

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <deque>

#include "../../base/base_model.h"

namespace s21 {

struct Point {
  int x, y;
  Point(int x = 0, int y = 0) : x(x), y(y) {}
  bool operator==(const Point& other) const {
    return x == other.x && y == other.y;
  }
};

class SnakeModel : public BrickGame {
  const int kBaseSnakeSize = 4;
  const int kPointsPerLevel = 5;
  const int kMaxLevel = 10;
  const char* kFileName = "snake.record";

 public:
  using action = void (SnakeModel::*)();
  SnakeModel();
  ~SnakeModel() = default;
  void ResetGame();
  void GetData(GameInfo_t& game) const override;

  const std::deque<Point>& GetSnake() const;
  const Point& GetFood() const;
  Direction GetCurrentDirection() const;
  bool CurrentDirectionIs(Direction) const;
  void SetCurrentDirection(Direction);

  GameState GetCurrentState() const override;
  void setCurrentState(GameState);
  void HandleEvent(Signals) override;
  void setSnakeMoved(bool);
  void SetFoodEaten(bool);
  void setFood(int, int);
  void LoadSnakeRecord();
  void WriteSnakeRecord();

 private:
  void MoveForward();
  void MoveUp();
  void MoveDown();
  void MoveRight();
  void MoveLeft();
  void MoveSnake();
  void Start();
  void Exit();
  void Pause();
  void GameOver();

  void GenerateFood();
  Point CalculateNewHead(const Point& head);
  bool OutOfBounds(const Point& newHead);
  bool CollisionWithBody(const Point& newHead);
  void HandleFoodConsumption();
  void UpdateGameBoard();
  static long long GetCurrentTimeInMilliseconds();

 private:
  long long last_update_time_;
  std::deque<Point> snake_;
  Point food_;
  int width_, height_;
  int score_;
  int level_;
  int speed_;
  int record_;
  bool snake_moved_, food_eaten_;
  Direction current_direction_;
  std::vector<std::vector<int>> game_board_;

  GameState currentState;

  action fsm_table_[4][8] = {
      {nullptr, nullptr, nullptr, nullptr, nullptr, &SnakeModel::Exit,
       &SnakeModel::Start, nullptr},
      {&SnakeModel::MoveForward, &SnakeModel::MoveUp, &SnakeModel::MoveDown,
       &SnakeModel::MoveRight, &SnakeModel::MoveLeft, &SnakeModel::Exit,
       nullptr, &SnakeModel::Pause},
      {&SnakeModel::GameOver, &SnakeModel::GameOver, &SnakeModel::GameOver,
       &SnakeModel::GameOver, &SnakeModel::GameOver, &SnakeModel::Exit,
       &SnakeModel::Start, &SnakeModel::GameOver},
      {nullptr, nullptr, nullptr, nullptr, nullptr, &SnakeModel::Exit, nullptr,
       &SnakeModel::Pause}};
};

}  // namespace s21

#endif  // CPP3_BRICKGAME_2_0_1_SRC_BRICK_GAME_SNAKE_BACKEND_SNAKE_MODEL_H_
