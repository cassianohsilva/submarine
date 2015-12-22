/*
 * score.h
 *
 *  Created on: 20 de dez de 2015
 *      Author: cassiano
 */

#include "types.h"

#ifndef SRC_SCORE_H_
#define SRC_SCORE_H_

#define MAX_NAME_SIZE 21

typedef struct {
	char name[MAX_NAME_SIZE];
	int value;
} Score;

Score Score_create(const char name[MAX_NAME_SIZE], const int score);

void Score_save(const Score score);

Score * Score_load(int * size);

bool Score_is_new_record(const int score);

#endif /* SRC_SCORE_H_ */
