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
  virtual GameState GetCurrentState() const = 0;
  virtual void HandleEvent(Signals) = 0;
  virtual ~BrickGame() = default;
};
}  // namespace s21

#endif  // based