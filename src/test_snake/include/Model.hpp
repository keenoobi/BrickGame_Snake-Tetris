#ifndef MODEL_H
#define MODEL_H

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
  void generateFood();
  void moveSnake();
  bool isGameOver() const;
  int getScore() const;
  int getLevel() const;
  int getSpeed() const;
  const std::vector<Point>& getSnake() const;
  const Point& getFood() const;
  Direction getCurrentDirection() const;
  void setCurrentDirection(Direction dir);

 private:
  int width, height;
  bool gameOver;
  int score;
  int level;
  int speed;
  std::vector<Point> snake;
  Point food;
  Direction currentDirection;
};

#endif  // MODEL_H