#ifndef based
#define based

#include "../data/data.h"

namespace s21 {
enum class Direction { UP, DOWN, LEFT, RIGHT };
enum class GameState { START, PLAYING, GAMEOVER, PAUSE, EXIT };
enum class Signals { NONE, UP, DOWN, RIGHT, LEFT, ESC, ENTER, PAUSE };

class BrickGame {
 public:
  virtual void GetData(GameInfo_t& game) const = 0;
  virtual GameState getCurrentState() const = 0;
  virtual void handleEvent(Signals) = 0;
  virtual ~BrickGame() = default;
};
}  // namespace s21

#endif  // based