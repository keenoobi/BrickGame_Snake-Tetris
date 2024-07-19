#ifndef MODEL_HPP
#define MODEL_HPP

#include <cstdlib>
#include <ctime>
#include <vector>

enum class Direction { UP, DOWN, LEFT, RIGHT };

struct Point {
  int x, y;
  Point(int x = 0, int y = 0) : x(x), y(y) {}
  bool operator==(const Point& other) const {
    return x == other.x && y == other.y;
  }
};

class SnakeModel {
 public:
  SnakeModel(int width, int height);
  void resetGame();
  void moveSnake();
  bool isGameOver() const;
  int getScore() const;
  int getLevel() const;
  int getSpeed() const;
  int getAccelerationSpeed() const;

  bool isAccelerationOn() const;
  void setAcceleration(bool);

  const std::vector<Point>& getSnake() const;
  const std::vector<std::vector<int>>& getGameBoard() const;

  const Point& getFood() const;
  Direction getCurrentDirection() const;
  void setCurrentDirection(Direction);

 private:
  void generateFood();
  Point calculateNewHead(const Point& head);
  bool isOutOfBounds(const Point& newHead);
  bool isCollisionWithBody(const Point& newHead);
  void handleFoodConsumption();
  void updateGameBoard();

  std::vector<Point> snake;
  Point food;
  int width, height;
  bool gameOver;
  int score;
  int level;
  int speed;
  int accelerationSpeed;
  bool acceleration;
  Direction currentDirection;
  std::vector<std::vector<int>> gameBoard;
};

#endif  // MODEL_HPP