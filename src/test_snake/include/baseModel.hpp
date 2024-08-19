#ifndef based
#define based

#include "data.h"

extern "C" {
#include "../tetris/tetris_backend.h"
}

namespace s21 {
enum class Direction { UP, DOWN, LEFT, RIGHT };
enum class GameState { START, PLAYING, GAMEOVER, PAUSE, EXIT };
enum class Signals { NONE, UP, DOWN, RIGHT, LEFT, ESC, ENTER, PAUSE };

class BrickGame {
 public:
  virtual void GetData(GameInfo_t& game) const = 0;
  virtual GameState getCurrentState() const = 0;
  virtual void handleEvent(Signals) = 0;
  // virtual const std::vector<std::vector<int>>& getGameBoard() const = 0;
  // virtual int getScore() const = 0;
  // virtual int getLevel() const = 0;
  // virtual int getSpeed() const = 0;
  virtual ~BrickGame() = default;
};
}  // namespace s21

#endif  // based