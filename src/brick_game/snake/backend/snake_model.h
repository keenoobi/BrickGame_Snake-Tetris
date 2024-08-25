#ifndef MODEL_HPP
#define MODEL_HPP

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

  const std::deque<Point>& getSnake() const;
  const Point& getFood() const;
  Direction getCurrentDirection() const;
  bool CurrentDirectionIs(Direction) const;
  void setCurrentDirection(Direction);

  GameState getCurrentState() const override;
  void setCurrentState(GameState);
  void handleEvent(Signals) override;
  void setSnakeMoved(bool);
  void setFoodEaten(bool);
  void setFood(int, int);
  void LoadSnakeRecord();
  void WriteSnakeRecord();

 private:
  void moveForward();
  void moveUp();
  void moveDown();
  void moveRight();
  void moveLeft();
  void moveSnake();
  void Start();
  void Exit();
  void Pause();
  void GameOver();

  void generateFood();
  Point calculateNewHead(const Point& head);
  bool isOutOfBounds(const Point& newHead);
  bool isCollisionWithBody(const Point& newHead);
  void handleFoodConsumption();
  void updateGameBoard();
  static long long GetCurrentTimeInMilliseconds();

 private:
  long long lastUpdateTime;
  std::deque<Point> snake;
  Point food;
  int width, height;
  int score;
  int level;
  int speed;
  int record;
  bool snake_moved, foodEaten;
  Direction currentDirection;
  std::vector<std::vector<int>> gameBoard;

  GameState currentState;

  action fsm_table[4][8] = {
      {nullptr, nullptr, nullptr, nullptr, nullptr, &SnakeModel::Exit,
       &SnakeModel::Start, nullptr},
      {&SnakeModel::moveForward, &SnakeModel::moveUp, &SnakeModel::moveDown,
       &SnakeModel::moveRight, &SnakeModel::moveLeft, &SnakeModel::Exit,
       nullptr, &SnakeModel::Pause},
      {&SnakeModel::GameOver, &SnakeModel::GameOver, &SnakeModel::GameOver,
       &SnakeModel::GameOver, &SnakeModel::GameOver, &SnakeModel::Exit,
       &SnakeModel::Start, &SnakeModel::GameOver},
      {nullptr, nullptr, nullptr, nullptr, nullptr, &SnakeModel::Exit, nullptr,
       &SnakeModel::Pause}};
};

}  // namespace s21

#endif  // MODEL_HPP
