#include "record_handling.h"

#include <stdio.h>

bool loadHighestScore(int *record, const char *filename) {
  bool state = true;
  FILE *load = fopen(filename, "rb");
  if (!load || record == NULL) return state = false;

  size_t count = fread(record, sizeof(int), 1, load);
  fclose(load);

  if (!count) state = false;
  return state;
}

bool saveHighestScore(int *record, const char *filename) {
  bool state = true;
  FILE *save = fopen(filename, "wb");
  if (!save || record == NULL) return state = false;

  size_t count = fwrite(record, sizeof(int), 1, save);
  fclose(save);

  if (!count) state = false;
  return state;
}