#ifndef TetrisModel_hpp
#define TetrisModel_hpp

#include "baseModel.hpp"

namespace s21 {

class TetrisModel : public BrickGame {
 public:
  TetrisModel();
  void ResetGame() override;
  void GetData(GameInfo_t &game) const override;
  GameState getCurrentState() const;
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