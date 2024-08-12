#ifndef DATA1_H
#define DATA1_H

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

#endif  // DATA1_H