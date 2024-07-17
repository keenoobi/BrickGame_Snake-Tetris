#include "../include/Model.hpp"

SnakeModel::SnakeModel(int width, int height)
    : width(width), height(height), score(0), level(1), speed(300) {
  resetGame();
}

void SnakeModel::resetGame() {
  snake.clear();
  int startX = width / 2 - 1;
  int startY = height / 2;
  for (int i = 0; i < 4; ++i) {
    snake.push_back(Point(startX + i, startY));
  }
  generateFood();
  score = 0;
  level = 1;
  speed = 300;
  gameOver = false;
  currentDirection = Direction::LEFT;
}

void SnakeModel::generateFood() {
  srand(time(0));
  bool foodOnSnake;
  do {
    food.x = rand() % width;
    food.y = rand() % height;
    foodOnSnake = false;
    for (const auto& part : snake) {
      if (part == food) {
        foodOnSnake = true;
        break;
      }
    }
  } while (foodOnSnake);
}

void SnakeModel::moveSnake() {
  Point head = snake.front();
  Point newHead(head.x, head.y);

  switch (currentDirection) {
    case Direction::UP:
      newHead.y--;
      break;
    case Direction::DOWN:
      newHead.y++;
      break;
    case Direction::LEFT:
      newHead.x--;
      break;
    case Direction::RIGHT:
      newHead.x++;
      break;
  }

  if (newHead.x < 0 || newHead.x >= width || newHead.y < 0 ||
      newHead.y >= height) {
    gameOver = true;
    return;
  }

  for (auto it = snake.begin(); it != snake.end(); it++) {
    if (*it == newHead) {
      gameOver = true;
      return;
    }
  }

  snake.insert(snake.begin(), newHead);

  if (newHead == food) {
    score++;
    if (score % 5 == 0 && level < 10) {
      level++;
      speed -= 40;  // Increase speed with each level
    }
    generateFood();
  } else {
    snake.pop_back();
  }
}

bool SnakeModel::isGameOver() const { return gameOver; }

int SnakeModel::getScore() const { return score; }

int SnakeModel::getLevel() const { return level; }

int SnakeModel::getSpeed() const { return speed; }

const std::vector<Point>& SnakeModel::getSnake() const { return snake; }

const Point& SnakeModel::getFood() const { return food; }

Direction SnakeModel::getCurrentDirection() const { return currentDirection; }

void SnakeModel::setCurrentDirection(Direction dir) { currentDirection = dir; }