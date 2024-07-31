#ifndef DATA_H
#define DATA_H

namespace s21 {

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

}  // namespace s21

#endif  // DATA_H