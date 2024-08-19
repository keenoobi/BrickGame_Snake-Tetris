#ifndef MODEL_HPP
#define MODEL_HPP

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "BaseModel.hpp"

namespace s21 {

struct Point {
  int x, y;
  Point(int x = 0, int y = 0) : x(x), y(y) {}
  bool operator==(const Point& other) const {
    return x == other.x && y == other.y;
  }
};

class SnakeModel : public BrickGame {
 public:
  using action = void (SnakeModel::*)();
  SnakeModel(int width = 10, int height = 20);
  ~SnakeModel() = default;
  void ResetGame();
  void GetData(GameInfo_t& game) const override;

  const std::vector<Point>& getSnake() const;
  const Point& getFood() const;
  Direction getCurrentDirection() const;
  bool CurrentDirectionIsNot(Direction) const;
  void setCurrentDirection(Direction);

  void handleEvent(Signals) override;
  GameState getCurrentState() const override;
  void setCurrentState(GameState);

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

 private:
  void generateFood();
  Point calculateNewHead(const Point& head);
  bool isOutOfBounds(const Point& newHead);
  bool isCollisionWithBody(const Point& newHead);
  void handleFoodConsumption();
  void updateGameBoard();
  static long long GetCurrentTimeInMilliseconds();

  long long lastUpdateTime;

  std::vector<Point> snake;
  Point food;
  int width, height;
  bool gameOver;
  int score;
  int level;
  int speed;
  bool snake_moved;
  int accelerationSpeed;
  bool acceleration;
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