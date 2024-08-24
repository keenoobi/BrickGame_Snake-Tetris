#include "snake_model.h"

namespace s21 {

SnakeModel::SnakeModel(int width, int height)
    : width(width),
      height(height),
      score(0),
      level(1),
      speed(300),
      record(0),
      snake_moved(false),
      currentDirection(Direction::LEFT),
      gameBoard(height, std::vector<int>(width, 0)),
      currentState(GameState::START) {
  lastUpdateTime = GetCurrentTimeInMilliseconds();
  LoadSnakeRecord();
}

void SnakeModel::ResetGame() {
  snake.clear();
  int startX = width / 2 - 1;
  int startY = height / 2;
  for (int i = 0; i < kBaseSnakeSize; ++i) {
    snake.push_back(Point(startX + i, startY));
  }
  generateFood();
  score = 0;
  level = 1;
  speed = 300;
  gameBoard = std::vector<std::vector<int>>(height, std::vector<int>(width, 0));
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
  game.high_score = record;
  game.level = level;
  game.speed = speed;
}

void SnakeModel::handleEvent(Signals signal) {
  action act =
      fsm_table[static_cast<int>(currentState)][static_cast<int>(signal)];

  if (act) (this->*act)();
}

void SnakeModel::generateFood() {
  do {
    food.x = rand() % width;
    food.y = rand() % height;
  } while (isCollisionWithBody(food) || gameBoard[food.y][food.x] != 0);
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
    gameBoard[snake.back().y][snake.back().x] = 0;
    snake.pop_back();
  } else {
    handleFoodConsumption();
  }
  // Добавляем новую голову
  snake.push_front(newHead);

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
  if (score == 200) {
    setCurrentState(GameState::GAMEOVER);
    return;
  } else if (score % kPointsPerLevel == 0 && level < kMaxLevel && speed) {
    level++;
    speed -= 21;  // Увеличиваем скорость с каждым уровнем
  }

  if (score > record) {
    record = score;
    WriteSnakeRecord();
  }
  generateFood();
}

void SnakeModel::updateGameBoard() {
  // Очищаем игровую доску
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      gameBoard[i][j] = 0;
    }
  }

  for (const auto& part : snake) {
    gameBoard[part.y][part.x] = 5;  // Змейка будет представлена 5
  }
  gameBoard[snake.front().y][snake.front().x] = 4;  // Голова змейки 4
  gameBoard[food.y][food.x] = 7;  // Еда будет представлена 7
}

long long SnakeModel::GetCurrentTimeInMilliseconds() {
  auto now = std::chrono::system_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      now.time_since_epoch());
  return duration.count();
}

const std::deque<Point>& SnakeModel::getSnake() const { return snake; }
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
    snake_moved = true;
  }
}

void SnakeModel::moveUp() {
  if (snake_moved && CurrentDirectionIsNot(Direction::DOWN)) {
    if (CurrentDirectionIsNot(Direction::UP)) {
      setCurrentDirection(Direction::UP);
      snake_moved = false;
    } else
      moveSnake();
  }
}

void SnakeModel::moveDown() {
  if (snake_moved && CurrentDirectionIsNot(Direction::UP)) {
    if (CurrentDirectionIsNot(Direction::DOWN)) {
      setCurrentDirection(Direction::DOWN);
      snake_moved = false;
    } else
      moveSnake();
  }
}

void SnakeModel::moveRight() {
  if (snake_moved && CurrentDirectionIsNot(Direction::LEFT)) {
    if (CurrentDirectionIsNot(Direction::RIGHT)) {
      setCurrentDirection(Direction::RIGHT);
      snake_moved = false;
    } else
      moveSnake();
  }
}

void SnakeModel::moveLeft() {
  if (snake_moved && CurrentDirectionIsNot(Direction::RIGHT)) {
    if (CurrentDirectionIsNot(Direction::LEFT)) {
      setCurrentDirection(Direction::LEFT);
      snake_moved = false;
    } else
      moveSnake();
  }
}

void SnakeModel::Start() {
  ResetGame();
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

void SnakeModel::LoadSnakeRecord() {
  std::ifstream file(kFileName, std::ios::binary);
  if (file.is_open()) {
    file.read(reinterpret_cast<char*>(&record), sizeof(record));
    file.close();
  }
}

void SnakeModel::WriteSnakeRecord() {
  std::ofstream file(kFileName, std::ios::binary);
  if (file.is_open()) {
    file.write(reinterpret_cast<const char*>(&record), sizeof(record));
    file.close();
  }
}

}  // namespace s21
