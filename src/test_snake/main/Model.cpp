#include "../include/Model.hpp"

namespace s21 {

SnakeModel::SnakeModel(int width, int height)
    : width(width),
      height(height),
      score(0),
      level(1),
      speed(300),
      accelerationSpeed(100),
      acceleration(false),
      gameBoard(height, std::vector<int>(width, 0)),
      currentState(GameState::START) {
  resetGame();
  lastUpdateTime = GetCurrentTimeInMilliseconds();
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

  currentDirection = Direction::LEFT;
  currentState = GameState::START;
  updateGameBoard();
}

void SnakeModel::handleEvent(Signals signal) {
  // if (signal == Signals::NONE) return;
  action act =
      fsm_table[static_cast<int>(currentState)][static_cast<int>(signal)];

  if (act) (this->*act)();
}

void SnakeModel::generateFood() {
  do {
    food.x = rand() % width;
    food.y = rand() % height;
  } while (isCollisionWithBody(food));
}

void SnakeModel::moveSnake() {
  Point newHead = calculateNewHead(snake.front());

  if (isOutOfBounds(newHead) || isCollisionWithBody(newHead)) {
    setCurrentState(GameState::GAMEOVER);
    return;
  }

  bool foodEaten = (newHead == food);

  // Удаляем хвост, если змейка не съела еду
  if (!foodEaten) {
    snake.pop_back();
  } else if (foodEaten) {
    handleFoodConsumption();
  }

  // Добавляем новую голову
  snake.insert(snake.begin(), newHead);

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
    speed -= 40;  // Увеличиваем скорость с каждым уровнем
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

long long SnakeModel::GetCurrentTimeInMilliseconds() {
  auto now = std::chrono::system_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      now.time_since_epoch());
  return duration.count();
}

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
GameState SnakeModel::getCurrentState() const { return currentState; }
void SnakeModel::setCurrentState(GameState state) { currentState = state; }

bool SnakeModel::CurrentDirectionIsNot(Direction dir) const {
  return currentDirection != dir;
}

void SnakeModel::moveForward() {
  long long currentTime = GetCurrentTimeInMilliseconds();
  if (currentTime - lastUpdateTime >= speed) {
    moveSnake();
    lastUpdateTime = currentTime;
  }
}

void SnakeModel::moveUp() {
  if (CurrentDirectionIsNot(Direction::DOWN)) {
    if (CurrentDirectionIsNot(Direction::UP))
      setCurrentDirection(Direction::UP);
    moveSnake();
  }
}

void SnakeModel::moveDown() {
  if (CurrentDirectionIsNot(Direction::UP)) {
    if (CurrentDirectionIsNot(Direction::DOWN))
      setCurrentDirection(Direction::DOWN);
    moveSnake();
  }
}

void SnakeModel::moveRight() {
  if (CurrentDirectionIsNot(Direction::LEFT)) {
    if (CurrentDirectionIsNot(Direction::RIGHT))
      setCurrentDirection(Direction::RIGHT);
    moveSnake();
  }
}

void SnakeModel::moveLeft() {
  if (CurrentDirectionIsNot(Direction::RIGHT)) {
    if (CurrentDirectionIsNot(Direction::LEFT))
      setCurrentDirection(Direction::LEFT);
    moveSnake();
  }
}

void SnakeModel::Start() {
  resetGame();
  setCurrentState(GameState::PLAYING);
}

void SnakeModel::Exit() { setCurrentState(GameState::EXIT); }

void SnakeModel::Pause() {
  if (currentState != GameState::PAUSE) {
    setCurrentState(GameState::PAUSE);
  } else {
    setCurrentState(GameState::PLAYING);
  }
}

void SnakeModel::GameOver() { setCurrentState(GameState::GAMEOVER); }

}  // namespace s21