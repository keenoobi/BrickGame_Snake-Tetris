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
  const Point& GetFood() const;
  Direction GetCurrentDirection() const;
  bool CurrentDirectionIs(Direction) const;
  void SetCurrentDirection(Direction);

  GameState GetCurrentState() const override;
  void setCurrentState(GameState);

 private:
  void HandleEvent(Signals) override;
  void MoveForward();
  void moveUp();
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
  std::vector<Point> snake;
  Point food_;
  int width_, height_;
  bool gameOver;
  int score_;
  int level_;
  int speed_;
  bool snake_moved;
  int accelerationSpeed;
  bool acceleration;
  Direction current_direction_;
  std::vector<std::vector<int>> game_board_;

  GameState currentState;

  action fsm_table_[4][8] = {
      {nullptr, nullptr, nullptr, nullptr, nullptr, &SnakeModel::Exit,
       &SnakeModel::Start, nullptr},
      {&SnakeModel::MoveForward, &SnakeModel::moveUp, &SnakeModel::MoveDown,
       &SnakeModel::MoveRight, &SnakeModel::MoveLeft, &SnakeModel::Exit,
       nullptr, &SnakeModel::Pause},
      {&SnakeModel::GameOver, &SnakeModel::GameOver, &SnakeModel::GameOver,
       &SnakeModel::GameOver, &SnakeModel::GameOver, &SnakeModel::Exit,
       &SnakeModel::Start, &SnakeModel::GameOver},
      {nullptr, nullptr, nullptr, nullptr, nullptr, &SnakeModel::Exit, nullptr,
       &SnakeModel::Pause}};
};

}  // namespace s21

#endif  // MODEL_HPP