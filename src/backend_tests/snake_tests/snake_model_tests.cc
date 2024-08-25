#include "test.h"
namespace s21 {

TEST(SnakeModelTests, Test1) {
  SnakeModel model;
  model.ResetGame();
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
  EXPECT_EQ(model.GetSnake().size(), 4);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);
}

TEST(SnakeModelTests, Test2) {
  SnakeModel model;
  model.setCurrentState(GameState::kPlaying);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
  model.ResetGame();
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
}

TEST(SnakeModelTests, Test3) {
  SnakeModel model;
  model.SetCurrentDirection(Direction::kRight);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kRight);
  model.SetCurrentDirection(Direction::kLeft);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);
}

TEST(SnakeModelTests, Test4) {
  SnakeModel model;
  model.ResetGame();
  EXPECT_GE(model.GetFood().x, 0);
  EXPECT_LE(model.GetFood().y, 20);
}

TEST(SnakeModelTests, Test5) {
  SnakeModel model;
  model.ResetGame();
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);

  EXPECT_TRUE(model.CurrentDirectionIs(Direction::kLeft));
  EXPECT_FALSE(model.CurrentDirectionIs(Direction::kRight));
  model.SetCurrentDirection(Direction::kRight);
  EXPECT_TRUE(model.CurrentDirectionIs(Direction::kRight));
  EXPECT_FALSE(model.CurrentDirectionIs(Direction::kLeft));
}

TEST(SnakeModelTests, Test6) {
  SnakeModel model;
  model.ResetGame();
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
  model.setCurrentState(GameState::kPlaying);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
}

TEST(SnakeModelTests, Test7) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::kEnter;
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
  model.HandleEvent(signal);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);
}

TEST(SnakeModelTests, Test7_1) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::kEsc;
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
  model.HandleEvent(signal);
  EXPECT_EQ(model.GetCurrentState(), GameState::kExit);
}

TEST(SnakeModelTests, Test7_2) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::kEnter;
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
  model.HandleEvent(signal);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);
  model.HandleEvent(signal = Signals::kPause);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPause);
  model.HandleEvent(signal = Signals::kPause);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
}

TEST(SnakeModelTests, Test7_3) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::kEnter;
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
  model.HandleEvent(signal);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);
  model.WriteSnakeRecord();
  model.LoadSnakeRecord();
}

TEST(SnakeModelTests, Test8) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::kEnter;
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
  model.HandleEvent(signal);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);
  model.setSnakeMoved(true);
  model.HandleEvent(signal = Signals::kUp);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kUp);
}

TEST(SnakeModelTests, Test9) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::kEnter;
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
  model.HandleEvent(signal);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);
  model.setSnakeMoved(true);
  model.HandleEvent(signal = Signals::kUp);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kUp);
  model.setSnakeMoved(true);
  model.HandleEvent(signal = Signals::kRight);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kRight);
}

TEST(SnakeModelTests, Test10) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::kEnter;
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
  model.HandleEvent(signal);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);
  model.setSnakeMoved(true);
  model.HandleEvent(signal = Signals::kUp);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kUp);
  model.setSnakeMoved(true);
  model.HandleEvent(signal = Signals::kLeft);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);
}

TEST(SnakeModelTests, Test11) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::kEnter;
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
  model.HandleEvent(signal);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);
  model.setSnakeMoved(true);
  model.HandleEvent(signal = Signals::kDown);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kDown);
}

TEST(SnakeModelTests, Test12) {
  SnakeModel model;
  model.ResetGame();
  int x = model.GetSnake().front().x;
  Signals signal = Signals::kEnter;
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
  model.HandleEvent(signal);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);
  model.setSnakeMoved(true);
  model.HandleEvent(signal = Signals::kLeft);
  model.setSnakeMoved(true);
  EXPECT_EQ(model.GetSnake().front().x, x - 1);
}

TEST(SnakeModelTests, Test13) {
  SnakeModel model;
  model.ResetGame();
  int x = model.GetSnake().front().x;
  Signals signal = Signals::kEnter;
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
  model.HandleEvent(signal);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);
  model.setSnakeMoved(true);
  model.HandleEvent(signal = Signals::kNone);
  std::this_thread::sleep_for(std::chrono::milliseconds(310));
  model.setSnakeMoved(true);
  model.HandleEvent(signal = Signals::kNone);
  EXPECT_EQ(model.GetSnake().front().x, x - 1);
}

TEST(SnakeModelTests, Test14) {
  SnakeModel model;
  GameInfo_t info = {};
  model.ResetGame();
  int y = model.GetSnake().front().y;
  Signals signal = Signals::kEnter;
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
  model.HandleEvent(signal);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);
  model.setSnakeMoved(true);
  model.HandleEvent(signal = Signals::kUp);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kUp);
  model.setSnakeMoved(true);
  model.HandleEvent(signal = Signals::kUp);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kUp);
  EXPECT_EQ(model.GetSnake().front().y, y - 1);
  info.field = new int *[20]();
  for (int i = 0; i < 20; ++i) {
    info.field[i] = new int[10]();
  }
  model.GetData(info);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.speed, 300);
  EXPECT_EQ(info.high_score, 0);
  for (int i = 0; i < 20; ++i) {
    delete[] info.field[i];
  }
  delete[] info.field;
}

TEST(SnakeModelTests, Test15) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::kEnter;
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
  model.HandleEvent(signal);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);
  for (int i = 0; i < 5; i++) {
    model.setSnakeMoved(true);
    model.HandleEvent(signal = Signals::kLeft);
  }
  EXPECT_EQ(model.GetCurrentState(), GameState::kGameOver);
}

TEST(SnakeModelTests, Test16) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::kEnter;
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
  model.HandleEvent(signal);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);
  for (int i = 0; i < 12; i++) {
    model.setSnakeMoved(true);
    model.HandleEvent(signal = Signals::kDown);
  }
  EXPECT_EQ(model.GetCurrentState(), GameState::kGameOver);
}

TEST(SnakeModelTests, Test17) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::kEnter;
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
  model.HandleEvent(signal);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);
  model.setSnakeMoved(true);
  model.HandleEvent(signal = Signals::kUp);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kUp);
  for (int i = 0; i < 6; i++) {
    model.setSnakeMoved(true);
    model.HandleEvent(signal = Signals::kRight);
  }
  EXPECT_EQ(model.GetCurrentState(), GameState::kGameOver);
}

TEST(SnakeModelTests, Test18) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::kEnter;
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
  model.HandleEvent(signal);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);
  model.setSnakeMoved(true);
  model.HandleEvent(signal = Signals::kUp);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kUp);
  for (int i = 0; i < 6; i++) {
    model.setSnakeMoved(true);
    model.HandleEvent(signal = Signals::kRight);
  }
  EXPECT_EQ(model.GetCurrentState(), GameState::kGameOver);
}

TEST(SnakeModelTests, Test19) {
  SnakeModel model;
  GameInfo_t info = {};
  model.ResetGame();
  Signals signal = Signals::kEnter;
  EXPECT_EQ(model.GetCurrentState(), GameState::kStart);
  model.HandleEvent(signal);
  EXPECT_EQ(model.GetCurrentState(), GameState::kPlaying);
  EXPECT_EQ(model.GetCurrentDirection(), Direction::kLeft);
  model.setFood(model.GetSnake().front().x - 1, model.GetSnake().front().y);
  model.setSnakeMoved(true);
  model.HandleEvent(signal = Signals::kLeft);
  info.field = new int *[20]();
  for (int i = 0; i < 20; ++i) {
    info.field[i] = new int[10]();
  }
  model.GetData(info);
  EXPECT_EQ(info.score, 1);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.speed, 300);
  EXPECT_EQ(info.high_score, 1);
  for (int i = 0; i < 20; ++i) {
    delete[] info.field[i];
  }
  delete[] info.field;
}
}  // namespace s21