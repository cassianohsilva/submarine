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

bool file_exist();

int comp(const void * elem1, const void * elem2) {
	Score s1 = *((Score *) elem1);
	Score s2 = *((Score *) elem2);

	if (s1.value < s2.value)
		return 1;
	if (s1.value > s2.value)
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

Score create_score(char * name, int score) {
	Score s;
	strncpy(s.name, name, 19);
	s.value = score;

	return s;
}

void save_score(char * name, int score) {

	FILE * file;

	file = fopen(FILENAME, "r+");

	int i, size;

	if (file_exist()) {

		fscanf(file, "%d\n", &size);

		Score scores[size + 1];

		char n[20];
		int s;

		scores[size] = create_score(name, score);

		printf("%d\n", size);

		for (i = 0; i < size; ++i) {
			fscanf(file, PATTERN, n, &s);

			Score sc;
			strcpy(sc.name, n);
			sc.value = s;

			scores[i] = sc;
		}

		fclose(file);

		FILE * file = fopen(FILENAME, "w");

		scores[size] = create_score(name, score);

		fprintf(file, "%d\n", (size < MAX_SCORE_SIZE) ? ++size : size);

		qsort(scores, sizeof(scores) / sizeof(*scores), sizeof(*scores), comp);

		for (i = 0; i < size; ++i) {
			fprintf(file, PATTERN, scores[i].name, scores[i].value);
		}

		fclose(file);

	} else {
		fclose(file);

		FILE * file = fopen(FILENAME, "w");

		fprintf(file, "%d\n", 1);
		fprintf(file, PATTERN, name, score);

		fclose(file);
	}
}

Score * load_scores(int * size) {

	FILE * file = fopen(FILENAME, "r+");
	Score * scores = NULL;

	int i;

	if (file_exist()) {
		fscanf(file, "%d\n", size);

		scores = malloc(*size * sizeof(Score));

		char n[20];
		int s;

		printf("%d\n", *size);

		for (i = 0; i < *size; ++i) {
			fscanf(file, PATTERN, n, &s);

			Score sc;
			strcpy(sc.name, n);
			sc.value = s;

			scores[i] = sc;
		}

		fclose(file);

	} else {
		*size = 0;
	}

	return scores;
}

bool is_new_record(int score) {

	FILE * file = fopen(FILENAME, "r+");

	int i = 0, size;

	bool is_greater = false;

	if (file_exist()) {
		fscanf(file, "%d\n", &size);

		char n[20];
		int s;

		printf("%d\n", size);

		while (i < size) {
			fscanf(file, PATTERN, n, &s);

			if (score > s) {
				is_greater = true;
				break;
			}
			++i;
		}

		fclose(file);
	} else {
		is_greater = true;
	}

	return is_greater;
}
