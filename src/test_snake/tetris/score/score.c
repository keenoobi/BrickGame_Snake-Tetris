#include "score.h"

void updateLevel(params_t *prms) {
  int current_level = prms->stats->score / POINTS_PER_LEVEL;
  if (current_level >= 10) current_level = 9;
  prms->stats->level = current_level;
}

void countPoints(params_t *prms, int lines) {
  if (lines <= 0) return;
  int points_number = kScoreForCompleteLiens[lines - 1];
  prms->stats->score += points_number;
  if (prms->stats->score >= POINTS_PER_LEVEL) updateLevel(prms);
  if (prms->stats->score > prms->stats->record) {
    prms->stats->record = prms->stats->score;
    saveHighestScore(&prms->stats->record, RECORDS_FILE);
  }
}