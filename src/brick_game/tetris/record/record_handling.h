#ifndef RECORD_HANDLING_H
#define RECORD_HANDLING_H

#include "../tetris_backend.h"

bool loadHighestScore(int *record, const char *filename);
bool saveHighestScore(int *record, const char *filename);

#endif