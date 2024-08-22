#ifndef TetrisModel_hpp
#define TetrisModel_hpp

#include "../snake/backend/base_model.h"

extern "C" {
#include "tetris_backend.h"
}

namespace s21 {

class TetrisModel : public BrickGame {
 public:
  TetrisModel();
  void ResetGame();
  void GetData(GameInfo_t &game) const override;
  GameState getCurrentState() const override;
  void handleEvent(Signals) override;
  void RecalibrateSignal(Signals);

 private:
  bool running;
  bool game_over;
  Signals_t signal;
  tetris_state state;
  game tetris;
};

}  // namespace s21
#endif  // TetrisModel_hpp
