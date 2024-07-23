#include <ncurses.h>

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <vector>

using namespace std;

enum class State { START, PLAYING, GAME_OVER };
enum class Direction { UP, DOWN, LEFT, RIGHT };

struct Point {
  int x, y;
  Point(int x = 0, int y = 0) : x(x), y(y) {}
  bool operator==(const Point& other) const {
    return x == other.x && y == other.y;
  }
};

class SnakeGame {
 private:
  State currentState;
  Direction currentDirection;
  vector<Point> snake;
  Point food;
  int width, height;
  bool gameOver;
  long long lastUpdateTime;
  int score;
  int level;
  int speed;
  int acceleratedSpeed;
  bool isAccelerated;
  vector<vector<int>> gameBoard;
  WINDOW* gameWin;
  WINDOW* sideBarWin;

  void initWindow() {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(speed);
    nodelay(stdscr, true);
    notimeout(stdscr, TRUE);
  }

  void generateFood() {
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

  void updateGameBoard() {
    gameBoard = vector<vector<int>>(height, vector<int>(width, 0));
    for (const auto& part : snake) {
      gameBoard[part.y][part.x] = 1;  // Змейка будет представлена 1
    }
    gameBoard[food.y][food.x] = 2;  // Еда будет представлена 2
  }

  void draw() {
    static vector<vector<int>> lastGameBoard = gameBoard;

    updateGameBoard();
    box(gameWin, 0, 0);     // Draw a border around the window
    box(sideBarWin, 0, 0);  // Draw a border around the window

    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (gameBoard[i][j] != lastGameBoard[i][j]) {
          char symbol = (gameBoard[i][j] == 1)   ? 'O'
                        : (gameBoard[i][j] == 2) ? '*'
                                                 : ' ';
          mvwprintw(gameWin, i + 1, j + 1, "%c", symbol);
        }
      }
    }

    mvwprintw(sideBarWin, 2, 2, "Score: %d", score);
    mvwprintw(sideBarWin, 4, 2, "Level: %d", level);
    mvwprintw(sideBarWin, 6, 2, "Speed: %d", speed);
    wrefresh(gameWin);
    wrefresh(sideBarWin);

    lastGameBoard = gameBoard;
  }

  static long long GetCurrentTimeInMilliseconds() {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch());
    return duration.count();
  }

  void resetGame() {
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
    currentState = State::PLAYING;
  }

  void handleInput(int ch) {
    if (ch != ERR) {
      switch (ch) {
        case KEY_UP:
          if (currentDirection != Direction::DOWN) {
            currentDirection = Direction::UP;
          }
          break;
        case KEY_DOWN:
          if (currentDirection != Direction::UP) {
            currentDirection = Direction::DOWN;
          }
          break;
        case KEY_LEFT:
          if (currentDirection != Direction::RIGHT) {
            currentDirection = Direction::LEFT;
          }
          break;
        case KEY_RIGHT:
          if (currentDirection != Direction::LEFT) {
            currentDirection = Direction::RIGHT;
          }
          break;
        case ' ':
          isAccelerated = true;
          break;
      }
      flushinp();  // Очистка буфера ввода
    } else {
      isAccelerated = false;
    }
  }

  Point calculateNewHead(const Point& head) {
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

  bool isOutOfBounds(const Point& newHead) {
    return newHead.x < 0 || newHead.x >= width || newHead.y < 0 ||
           newHead.y >= height;
  }

  bool isCollisionWithBody(const Point& newHead) {
    for (const auto& part : snake) {
      if (part == newHead) {
        return true;
      }
    }
    return false;
  }

  void updateSnake(const Point& newHead, bool ateFood) {
    snake.insert(snake.begin(), newHead);
    if (!ateFood) {
      snake.pop_back();
    }
  }

  void handleFoodConsumption() {
    score++;
    if (score % 5 == 0 && level < 10) {
      level++;
      speed -= 40;  // Increase speed with each level
      timeout(speed);
    }
    generateFood();
  }

  void moveSnake() {
    Point newHead = calculateNewHead(snake.front());

    if (isOutOfBounds(newHead) || isCollisionWithBody(newHead)) {
      gameOver = true;
      return;
    }

    bool ateFood = (newHead == food);
    updateSnake(newHead, ateFood);

    if (ateFood) {
      handleFoodConsumption();
    }
  }

 public:
  SnakeGame(int width, int height)
      : width(width),
        height(height),
        gameOver(false),
        score(0),
        level(1),
        speed(300),
        acceleratedSpeed(100),
        isAccelerated(false),
        gameBoard(height, vector<int>(width, 0)) {
    currentState = State::START;
    currentDirection = Direction::LEFT;
    int startX =
        width / 2 -
        2;  // Изменено, чтобы змейка не могла столкнуться с самой собой
    int startY = height / 2;
    for (int i = 0; i < 4; ++i) {
      snake.push_back(Point(startX + i, startY));
    }
    generateFood();
    lastUpdateTime = GetCurrentTimeInMilliseconds();
  }

  void run() {
    initWindow();
    srand(time(0));

    int startX = 2;
    int startY = 2;
    gameWin = newwin(height + 2, width + 2, startY, startX);
    sideBarWin = newwin(height + 2, width - 5, startY, startX + width + 2);

    int ch;
    while (true) {
      if (gameOver) {
        currentState = State::GAME_OVER;
      }
      long long currentTime = GetCurrentTimeInMilliseconds();
      if (currentTime - lastUpdateTime >=
          (isAccelerated ? acceleratedSpeed : speed)) {
        switch (currentState) {
          case State::START:
            draw();
            currentState = State::PLAYING;
            break;
          case State::PLAYING:
            ch = getch();
            handleInput(ch);
            moveSnake();
            draw();
            break;
          case State::GAME_OVER:
            wclear(gameWin);
            wprintw(gameWin, "Game Over! Score: %d  Level: %d\n", score, level);
            wprintw(gameWin, "Start over? (Press Enter)\n");
            wrefresh(gameWin);
            nodelay(stdscr, FALSE);  // Wait for user input
            ch = getch();
            if (ch == KEY_ENTER || ch == '\n') {
              resetGame();
              nodelay(stdscr, TRUE);  // Resume non-blocking input
              wclear(gameWin);
              wrefresh(gameWin);
            }
            break;
        }
        lastUpdateTime = currentTime;
      }
    }
    delwin(gameWin);
    endwin();
  }
};

int main() {
  SnakeGame game(20, 20);
  game.run();
  return 0;
}