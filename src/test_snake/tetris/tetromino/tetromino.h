#ifndef TETROMINO_H
#define TETROMINO_H

#include "../tetris.h"

bool tetrominoFits(game *tetris, tetris_block block);
int randomTetromino();
void newFallingFigure(game *tetris);
void placeTetromino(game *tetris, tetris_block piece);
void placeNextTetromino(game *tetris, tetris_block piece);
void removeNextTetromino(game *tetris);
void removeTetromino(game *tetris, tetris_block piece);

#endif