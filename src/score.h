/*
 * score.h
 *
 *  Created on: 20 de dez de 2015
 *      Author: cassiano
 */

#include "types.h"

#ifndef SRC_SCORE_H_
#define SRC_SCORE_H_

typedef struct {
	char name[20];
	int score;
} Score;

void save_score(char * name, int score);

Score * load_scores(int * size);

bool is_new_record(int score);

#endif /* SRC_SCORE_H_ */
