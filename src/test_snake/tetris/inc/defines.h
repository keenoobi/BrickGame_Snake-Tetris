#ifndef DEFINES_H
#define DEFINES_H

#define POINTS_PER_LEVEL 600
#define EMPTY_CELL 0
#define CONVERT_TO_CELL(x) (x + 1)
#define RECORDS_FILE "record.save"

#define ESC_KEY 27
#define ENTER_KEY 10

#define WPRINTW(window, y, x, ...) \
  mvwprintw((window), 1 + (y), 1 + (x), __VA_ARGS__)
#define ADD_BLOCK(win, row, col, x)                                         \
  mvwaddch((win), (row) + 1, (col)*2 + 1, ' ' | A_REVERSE | COLOR_PAIR(x)); \
  mvwaddch((win), (row) + 1, (col)*2 + 2, ' ' | A_REVERSE | COLOR_PAIR(x))

#define ADD_EMPTY(win, row, col)                \
  mvwaddch((win), (row) + 1, (col)*2 + 1, ' '); \
  mvwaddch((win), (row) + 1, (col)*2 + 2, ' ')

#define TRANSFER_DATA(ROWS, COLS, DEST, SRC) \
  {                                          \
    for (int _i = 0; _i < ROWS; _i++) {      \
      for (int _j = 0; _j < COLS; _j++) {    \
        DEST[_i][_j] = SRC[_i][_j];          \
      }                                      \
    }                                        \
  }

#endif