#ifndef FSM_H
#define FSM_H

#include "../tetris_backend.h"

typedef void (*action)(params_t *prms);

void sigact(Signals_t signal, tetris_state *state, game *tetris);

void exitstate(params_t *prms);
void spawn(params_t *prms);
void start(params_t *prms);
void shifting(params_t *prms);
void moveDown(params_t *prms);
void moveRight(params_t *prms);
void moveLeft(params_t *prms);
void rotate(params_t *prms);
void attaching(params_t *prms);
void pauseGame(params_t *prms);
void gameOver(params_t *prms);

#endif