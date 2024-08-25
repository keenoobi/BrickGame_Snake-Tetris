#include "tetris_model.h"

namespace s21 {
TetrisModel::TetrisModel()
    : running(true), game_over(false), signal(None), state(START) {
  ResetGame();
}

void TetrisModel::ResetGame() {
  tetris = {};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
}

void TetrisModel::RecalibrateSignal(Signals new_signal) {
  switch (new_signal) {
    case Signals::kNone:
      signal = None;
      break;
    case Signals::kUp:
      signal = Rotate;
      break;
    case Signals::kDown:
      signal = Down;
      break;
    case Signals::kLeft:
      signal = Left;
      break;
    case Signals::kRight:
      signal = Right;
      break;
    case Signals::kEsc:
      signal = ESC;
      break;
    case Signals::kEnter:
      signal = Enter;
      break;
    case Signals::kPause:
      signal = Pause;
    default:
      break;
  }
}

void TetrisModel::HandleEvent(Signals undef_signal) {
  RecalibrateSignal(undef_signal);
  sigact(signal, &state, &tetris);
}

GameState TetrisModel::GetCurrentState() const {
  GameState new_sate = GameState::kStart;
  switch (state) {
    case START:
      new_sate = GameState::kStart;
      break;
    case SPAWN:
      new_sate = GameState::kPlaying;
      break;
    case MOVING:
      new_sate = GameState::kPlaying;
      break;
    case ATTACHING:
      new_sate = GameState::kPlaying;
      break;
    case GAMEOVER:
      new_sate = GameState::kGameOver;
      break;
    case EXIT_STATE:
      new_sate = GameState::kExit;
      break;
    case PAUSE:
      new_sate = GameState::kPause;
      break;
    default:
      break;
  }
  return new_sate;
}

void TetrisModel::GetData(GameInfo_t &data) const {
  TRANSFER_DATA(BOARD_HEIGHT, BOARD_WIDTH, data.field, tetris.board);
  TRANSFER_DATA(TETROMINO_SIZE, TETROMINO_SIZE, data.next, tetris.next_figure);
  data.score = tetris.score;
  data.level = tetris.level;
  data.high_score = tetris.record;
}

}  // namespace s21