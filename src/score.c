/*
 * score.c
 *
 *  Created on: 20 de dez de 2015
 *      Author: cassiano
 */

#include "score.h"

#define FILENAME ".scores"
#define PATTERN "%s %d\n"

#define MAX_SCORE_SIZE 10

int comp(const void * elem1, const void * elem2) {
	Score s1 = *((Score *) elem1);
	Score s2 = *((Score *) elem2);

	if (s1.score > s2.score)
		return 1;
	if (s1.score < s2.score)
		return -1;

	return 0;
}

bool file_exist() {

	FILE * f = fopen(FILENAME, "r");

	if (f != NULL) {

		fseek(f, 0L, SEEK_END);
		int s = ftell(f);

		fclose(f);

		return s > 0;
	}

	return false;
}

void save_score(char * name, int score) {

	FILE * file;

	file = fopen(FILENAME, "a+");

	int i, size;

	if (file_exist()) {

		fscanf(file, "%d\n", &size);

		if (size < MAX_SCORE_SIZE) {
			fprintf(file, PATTERN, name, score);
		} else {
			Score scores[size + 1];

			char name[20];
			int score;

			for (i = 0; i < size; ++i) {
				fscanf(file, PATTERN, name, &score);

				Score s;
				strcpy(s.name, name);
				s.score = score;

				scores[i] = s;
			}

			qsort(scores, sizeof(scores) / sizeof(*scores), sizeof(*scores),
					comp);
		}
	} else {
		fprintf(file, "%d\n", 1);
		fprintf(file, PATTERN, name, score);
	}

	fclose(file);
}

Score * load_scores(int * size) {
	return NULL;
}

bool is_new_record(int score) {
	return false;
}
