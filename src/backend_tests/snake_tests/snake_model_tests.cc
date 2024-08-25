#include "test.h"
namespace s21 {

TEST(SnakeModelTests, Test1) {
  SnakeModel model;
  model.ResetGame();
  EXPECT_EQ(model.getCurrentState(), GameState::START);
  EXPECT_EQ(model.getSnake().size(), 4);
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);
}

TEST(SnakeModelTests, Test2) {
  SnakeModel model;
  model.setCurrentState(GameState::PLAYING);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
  model.ResetGame();
  EXPECT_EQ(model.getCurrentState(), GameState::START);
}

TEST(SnakeModelTests, Test3) {
  SnakeModel model;
  model.setCurrentDirection(Direction::RIGHT);
  EXPECT_EQ(model.getCurrentDirection(), Direction::RIGHT);
  model.setCurrentDirection(Direction::LEFT);
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);
}

TEST(SnakeModelTests, Test4) {
  SnakeModel model;
  model.ResetGame();
  EXPECT_GE(model.getFood().x, 0);
  EXPECT_LE(model.getFood().y, 20);
}

TEST(SnakeModelTests, Test5) {
  SnakeModel model;
  model.ResetGame();
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);

  EXPECT_TRUE(model.CurrentDirectionIs(Direction::LEFT));
  EXPECT_FALSE(model.CurrentDirectionIs(Direction::RIGHT));
  model.setCurrentDirection(Direction::RIGHT);
  EXPECT_TRUE(model.CurrentDirectionIs(Direction::RIGHT));
  EXPECT_FALSE(model.CurrentDirectionIs(Direction::LEFT));
}

TEST(SnakeModelTests, Test6) {
  SnakeModel model;
  model.ResetGame();
  EXPECT_EQ(model.getCurrentState(), GameState::START);
  model.setCurrentState(GameState::PLAYING);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
}

TEST(SnakeModelTests, Test7) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::ENTER;
  EXPECT_EQ(model.getCurrentState(), GameState::START);
  model.handleEvent(signal);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);
}

TEST(SnakeModelTests, Test7_1) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::ESC;
  EXPECT_EQ(model.getCurrentState(), GameState::START);
  model.handleEvent(signal);
  EXPECT_EQ(model.getCurrentState(), GameState::EXIT);
}

TEST(SnakeModelTests, Test7_2) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::ENTER;
  EXPECT_EQ(model.getCurrentState(), GameState::START);
  model.handleEvent(signal);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);
  model.handleEvent(signal = Signals::PAUSE);
  EXPECT_EQ(model.getCurrentState(), GameState::PAUSE);
  model.handleEvent(signal = Signals::PAUSE);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
}

TEST(SnakeModelTests, Test7_3) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::ENTER;
  EXPECT_EQ(model.getCurrentState(), GameState::START);
  model.handleEvent(signal);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);
  model.WriteSnakeRecord();
  model.LoadSnakeRecord();
}

TEST(SnakeModelTests, Test8) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::ENTER;
  EXPECT_EQ(model.getCurrentState(), GameState::START);
  model.handleEvent(signal);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);
  model.setSnakeMoved(true);
  model.handleEvent(signal = Signals::UP);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
  EXPECT_EQ(model.getCurrentDirection(), Direction::UP);
}

TEST(SnakeModelTests, Test9) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::ENTER;
  EXPECT_EQ(model.getCurrentState(), GameState::START);
  model.handleEvent(signal);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);
  model.setSnakeMoved(true);
  model.handleEvent(signal = Signals::UP);
  EXPECT_EQ(model.getCurrentDirection(), Direction::UP);
  model.setSnakeMoved(true);
  model.handleEvent(signal = Signals::RIGHT);
  EXPECT_EQ(model.getCurrentDirection(), Direction::RIGHT);
}

TEST(SnakeModelTests, Test10) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::ENTER;
  EXPECT_EQ(model.getCurrentState(), GameState::START);
  model.handleEvent(signal);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);
  model.setSnakeMoved(true);
  model.handleEvent(signal = Signals::UP);
  EXPECT_EQ(model.getCurrentDirection(), Direction::UP);
  model.setSnakeMoved(true);
  model.handleEvent(signal = Signals::LEFT);
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);
}

TEST(SnakeModelTests, Test11) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::ENTER;
  EXPECT_EQ(model.getCurrentState(), GameState::START);
  model.handleEvent(signal);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);
  model.setSnakeMoved(true);
  model.handleEvent(signal = Signals::DOWN);
  EXPECT_EQ(model.getCurrentDirection(), Direction::DOWN);
}

TEST(SnakeModelTests, Test12) {
  SnakeModel model;
  model.ResetGame();
  int x = model.getSnake().front().x;
  Signals signal = Signals::ENTER;
  EXPECT_EQ(model.getCurrentState(), GameState::START);
  model.handleEvent(signal);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);
  model.setSnakeMoved(true);
  model.handleEvent(signal = Signals::LEFT);
  model.setSnakeMoved(true);
  EXPECT_EQ(model.getSnake().front().x, x - 1);
}

TEST(SnakeModelTests, Test13) {
  SnakeModel model;
  model.ResetGame();
  int x = model.getSnake().front().x;
  Signals signal = Signals::ENTER;
  EXPECT_EQ(model.getCurrentState(), GameState::START);
  model.handleEvent(signal);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);
  model.setSnakeMoved(true);
  model.handleEvent(signal = Signals::NONE);
  std::this_thread::sleep_for(std::chrono::milliseconds(310));
  model.setSnakeMoved(true);
  model.handleEvent(signal = Signals::NONE);
  EXPECT_EQ(model.getSnake().front().x, x - 1);
}

TEST(SnakeModelTests, Test14) {
  SnakeModel model;
  GameInfo_t info = {};
  model.ResetGame();
  int y = model.getSnake().front().y;
  Signals signal = Signals::ENTER;
  EXPECT_EQ(model.getCurrentState(), GameState::START);
  model.handleEvent(signal);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);
  model.setSnakeMoved(true);
  model.handleEvent(signal = Signals::UP);
  EXPECT_EQ(model.getCurrentDirection(), Direction::UP);
  model.setSnakeMoved(true);
  model.handleEvent(signal = Signals::UP);
  EXPECT_EQ(model.getCurrentDirection(), Direction::UP);
  EXPECT_EQ(model.getSnake().front().y, y - 1);
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
  Signals signal = Signals::ENTER;
  EXPECT_EQ(model.getCurrentState(), GameState::START);
  model.handleEvent(signal);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);
  for (int i = 0; i < 5; i++) {
    model.setSnakeMoved(true);
    model.handleEvent(signal = Signals::LEFT);
  }
  EXPECT_EQ(model.getCurrentState(), GameState::GAMEOVER);
}

TEST(SnakeModelTests, Test16) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::ENTER;
  EXPECT_EQ(model.getCurrentState(), GameState::START);
  model.handleEvent(signal);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);
  for (int i = 0; i < 12; i++) {
    model.setSnakeMoved(true);
    model.handleEvent(signal = Signals::DOWN);
  }
  EXPECT_EQ(model.getCurrentState(), GameState::GAMEOVER);
}

TEST(SnakeModelTests, Test17) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::ENTER;
  EXPECT_EQ(model.getCurrentState(), GameState::START);
  model.handleEvent(signal);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);
  model.setSnakeMoved(true);
  model.handleEvent(signal = Signals::UP);
  EXPECT_EQ(model.getCurrentDirection(), Direction::UP);
  for (int i = 0; i < 6; i++) {
    model.setSnakeMoved(true);
    model.handleEvent(signal = Signals::RIGHT);
  }
  EXPECT_EQ(model.getCurrentState(), GameState::GAMEOVER);
}

TEST(SnakeModelTests, Test18) {
  SnakeModel model;
  model.ResetGame();
  Signals signal = Signals::ENTER;
  EXPECT_EQ(model.getCurrentState(), GameState::START);
  model.handleEvent(signal);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);
  model.setSnakeMoved(true);
  model.handleEvent(signal = Signals::UP);
  EXPECT_EQ(model.getCurrentDirection(), Direction::UP);
  for (int i = 0; i < 6; i++) {
    model.setSnakeMoved(true);
    model.handleEvent(signal = Signals::RIGHT);
  }
  EXPECT_EQ(model.getCurrentState(), GameState::GAMEOVER);
}

TEST(SnakeModelTests, Test19) {
  SnakeModel model;
  GameInfo_t info = {};
  model.ResetGame();
  Signals signal = Signals::ENTER;
  EXPECT_EQ(model.getCurrentState(), GameState::START);
  model.handleEvent(signal);
  EXPECT_EQ(model.getCurrentState(), GameState::PLAYING);
  EXPECT_EQ(model.getCurrentDirection(), Direction::LEFT);
  model.setFood(model.getSnake().front().x - 1, model.getSnake().front().y);
  model.setSnakeMoved(true);
  model.handleEvent(signal = Signals::LEFT);
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