#include "../include/Model.hpp"

namespace s21 {
constexpr int base_snake_size = 4;

SnakeModel::SnakeModel(int width, int height)
    : width(width),
      height(height),
      score(0),
      level(1),
      speed(300),
      move_forward(false),
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
  for (int i = 0; i < base_snake_size; ++i) {
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

void SnakeModel::GetData(GameInfo_t& game) const {
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      game.field[i][j] = gameBoard[i][j];
    }
  }
  game.score = score;
  game.level = level;
  game.speed = speed;
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
  if (score % 5 == 0 && level < 10 && speed) {
    level++;
    speed -= 40;  // Увеличиваем скорость с каждым уровнем
  }
  generateFood();
}

void SnakeModel::updateGameBoard() {
  gameBoard = std::vector<std::vector<int>>(height, std::vector<int>(width, 0));
  for (const auto& part : snake) {
    gameBoard[part.y][part.x] = 5;  // Змейка будет представлена 1
  }
  gameBoard[food.y][food.x] = 7;  // Еда будет представлена 2
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
// int SnakeModel::getAccelerationSpeed() const { return accelerationSpeed; }
// bool SnakeModel::isAccelerationOn() const { return acceleration; }
// void SnakeModel::setAcceleration(bool status) { acceleration = status; }
const std::vector<Point>& SnakeModel::getSnake() const { return snake; }
const std::vector<std::vector<int>>& SnakeModel::getGameBoard() const {
  return gameBoard;
}
const Point& SnakeModel::getFood() const { return food; }
Direction SnakeModel::getCurrentDirection() const { return currentDirection; }
void SnakeModel::setCurrentDirection(Direction dir) { currentDirection = dir; }
GameState SnakeModel::getCurrentState() const { return currentState; }
void SnakeModel::setCurrentState(GameState state) { currentState = state; }

inline bool SnakeModel::CurrentDirectionIsNot(Direction dir) const {
  return currentDirection != dir;
}

void SnakeModel::moveForward() {
  long long currentTime = GetCurrentTimeInMilliseconds();
  if (currentTime - lastUpdateTime >= speed) {
    moveSnake();
    lastUpdateTime = currentTime;
    move_forward = true;
  }
}

void SnakeModel::moveUp() {
  if (move_forward && CurrentDirectionIsNot(Direction::DOWN)) {
    if (CurrentDirectionIsNot(Direction::UP)) {
      setCurrentDirection(Direction::UP);
      move_forward = false;
    } else
      moveSnake();
  }
}

void SnakeModel::moveDown() {
  if (move_forward && CurrentDirectionIsNot(Direction::UP)) {
    if (CurrentDirectionIsNot(Direction::DOWN)) {
      setCurrentDirection(Direction::DOWN);
      move_forward = false;
    } else
      moveSnake();
  }
}

void SnakeModel::moveRight() {
  if (move_forward && CurrentDirectionIsNot(Direction::LEFT)) {
    if (CurrentDirectionIsNot(Direction::RIGHT)) {
      setCurrentDirection(Direction::RIGHT);
      move_forward = false;
    } else
      moveSnake();
  }
}

void SnakeModel::moveLeft() {
  if (move_forward && CurrentDirectionIsNot(Direction::RIGHT)) {
    if (CurrentDirectionIsNot(Direction::LEFT)) {
      setCurrentDirection(Direction::LEFT);
      move_forward = false;
    } else
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