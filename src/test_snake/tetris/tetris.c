#include "tetris.h"

int menu(WINDOW *w) {
  nodelay(w, true);
  refresh();

  int c = 0;
  int menu_option = 0;
  bool enter = false;
  char *options[] = {"Tetris", "Snake", "Exit"};
  box(w, 0, 0);

  while (!enter) {
    c = getch();
    switch (c) {
      case KEY_UP:
        if (menu_option != 0) menu_option--;
        break;
      case KEY_DOWN:
        if (menu_option != 2) menu_option++;
        break;
      case ENTER_KEY:
        enter = true;
      default:
        break;
    }
    for (int i = 0; i < 3 && !enter; i++) {
      if (i == menu_option) {
        wattron(w, COLOR_PAIR(2) | A_REVERSE);
        mvwprintw(w, 9 + i * 2, 5 + 6, "%s", options[i]);
        wattroff(w, COLOR_PAIR(2) | A_REVERSE);
      } else {
        mvwprintw(w, 4, 5 + 6, "BRICK GAME");
        mvwprintw(w, 9 + i * 2, 5 + 6, "%s", options[i]);
      }
    }
    wrefresh(w);
  }
  return menu_option + 1;
}

int main() {
  WIN_INIT(1);
  windows w;

  GameInfo_t tetris_graphics = {0};

  w.board = createNewWindow(w.board, BOARD_WIDTH * 2, BOARDS_BEGIN);
  w.sidebar =
      createNewWindow(w.sidebar, HUD_WIDTH, BOARDS_BEGIN + BOARD_WIDTH * 2 + 2);
  w.menu =
      createNewWindow(w.menu, BOARD_WIDTH * 2 + HUD_WIDTH + 2, BOARDS_BEGIN);

  int a = 0;
  while ((a = menu(w.menu)) != 3) {
    switch (a) {
      case 1:
        game tetris = {0};
        gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
        gameLoop(&w, &tetris, &tetris_graphics);
        break;
      case 2:
        wclear(w.menu);
        box(w.menu, 0, 0);
        mvwprintw(w.menu, 7, 8, "Snake Not Found");
        break;
      case 3:
        wclear(w.menu);
        box(w.menu, 0, 0);
        mvwprintw(w.menu, 10, 5, "Exit...");
        break;
      default:

        break;
    }
  }
  endwin();

  return 0;
}

void gameLoop(windows *w, game *tetris, GameInfo_t *data) {
  bool running = TRUE;
  bool game_over = FALSE;
  int signal = 0;
  tetris_state state = START;

  while (running) {
    *data = updateCurrentState(tetris, data);
    signal = getch();
    sigact(get_signal(signal), w, &state, tetris, &game_over);
    graphicProcessing(w->board, w->sidebar, data, &state);
    refresh();
    if (state == EXIT_STATE) running = FALSE;
    delay_output(5);
  }
}

void graphicProcessing(WINDOW *board, WINDOW *sidebar, GameInfo_t *data,
                       tetris_state *state) {
  if (*state == MOVING) {
    displayField(board, data);
    displayNextFigure(sidebar, data);
    printStats(sidebar, data);
  }
  if (*state == GAMEOVER) wclear(sidebar);
}