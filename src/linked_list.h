/*
 * linked_list.h
 *
 *  Created on: 20 de nov de 2015
 *      Author: cassiano
 */

#include "types.h"

#ifndef SRC_LINKED_LIST_H_
#define SRC_LINKED_LIST_H_

typedef struct _node {
	void * value;
	struct _node * next;
} Node;

typedef struct {
	Node * begin;
} List;

List * List_create();

void List_insert(List * list, void * value);

bool List_remove(List * list, void * value);

void List_destroy(List * list);

#endif /* SRC_LINKED_LIST_H_ */
