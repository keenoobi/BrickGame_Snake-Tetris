#include "../include/TetrisModel.hpp"

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
    case Signals::NONE:
      signal = None;
      break;
    case Signals::UP:
      signal = Rotate;
      break;
    case Signals::DOWN:
      signal = Down;
      break;
    case Signals::LEFT:
      signal = Left;
      break;
    case Signals::RIGHT:
      signal = Right;
      break;
    case Signals::ESC:
      signal = ESC;
      break;
    case Signals::ENTER:
      signal = Enter;
      break;
    case Signals::PAUSE:
      signal = Pause;
    default:
      break;
  }
}

void TetrisModel::handleEvent(Signals undef_signal) {
  RecalibrateSignal(undef_signal);
  sigact(signal, &state, &tetris);
}

GameState TetrisModel::getCurrentState() const {
  GameState new_sate = GameState::START;
  switch (state) {
    case START:
      new_sate = GameState::START;
      break;
    case SPAWN:
      new_sate = GameState::PLAYING;
      break;
    case MOVING:
      new_sate = GameState::PLAYING;
      break;
    case ATTACHING:
      new_sate = GameState::PLAYING;
      break;
    case GAMEOVER:
      new_sate = GameState::GAMEOVER;
      break;
    case EXIT_STATE:
      new_sate = GameState::EXIT;
      break;
    case PAUSE:
      new_sate = GameState::PAUSE;
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