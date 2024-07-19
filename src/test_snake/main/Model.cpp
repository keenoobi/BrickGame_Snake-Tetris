#include "../include/Model.hpp"

SnakeModel::SnakeModel(int width, int height)
    : width(width),
      height(height),
      score(0),
      level(1),
      speed(300),
      accelerationSpeed(100),
      acceleration(false),
      gameBoard(height, std::vector<int>(width, 0)) {
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
  updateGameBoard();
}

void SnakeModel::generateFood() {
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
  Point newHead = calculateNewHead(snake.front());

  if (isOutOfBounds(newHead)) {
    gameOver = true;
    return;
  }

  // Удаляем хвост, чтобы он не мешал проверке столкновений
  Point tail = snake.back();
  snake.pop_back();

  if (isCollisionWithBody(newHead)) {
    gameOver = true;
    snake.push_back(tail);  // Восстанавливаем хвост перед выходом
    return;
  }

  // Добавляем новую голову
  snake.insert(snake.begin(), newHead);

  if (newHead == food) {
    handleFoodConsumption();
    snake.push_back(tail);  // Восстанавливаем хвост, так как змейка съела еду
  }
  updateGameBoard();
}

Point SnakeModel::calculateNewHead(const Point& head) {
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

  return newHead;
}

bool SnakeModel::isOutOfBounds(const Point& newHead) {
  return newHead.x < 0 || newHead.x >= width || newHead.y < 0 ||
         newHead.y >= height;
}

bool SnakeModel::isCollisionWithBody(const Point& newHead) {
  for (const auto& part : snake) {
    if (part == newHead) {
      return true;
    }
  }
  return false;
}

void SnakeModel::handleFoodConsumption() {
  score++;
  if (score % 5 == 0 && level < 10) {
    level++;
    speed -= 40;  // Increase speed with each level
  }
  generateFood();
}

void SnakeModel::updateGameBoard() {
  gameBoard = std::vector<std::vector<int>>(height, std::vector<int>(width, 0));
  for (const auto& part : snake) {
    gameBoard[part.y][part.x] = 1;  // Змейка будет представлена 1
  }
  gameBoard[food.y][food.x] = 2;  // Еда будет представлена 2
}

bool SnakeModel::isGameOver() const { return gameOver; }
int SnakeModel::getScore() const { return score; }
int SnakeModel::getLevel() const { return level; }
int SnakeModel::getSpeed() const { return speed; }
int SnakeModel::getAccelerationSpeed() const { return accelerationSpeed; }
bool SnakeModel::isAccelerationOn() const { return acceleration; }
void SnakeModel::setAcceleration(bool status) { acceleration = status; }
const std::vector<Point>& SnakeModel::getSnake() const { return snake; }
const std::vector<std::vector<int>>& SnakeModel::getGameBoard() const {
  return gameBoard;
}
const Point& SnakeModel::getFood() const { return food; }
Direction SnakeModel::getCurrentDirection() const { return currentDirection; }
void SnakeModel::setCurrentDirection(Direction dir) { currentDirection = dir; }