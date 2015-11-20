/*
 * linked_list.c
 *
 *  Created on: 20 de nov de 2015
 *      Author: cassiano
 */

#include "linked_list.h"

List * List_create() {

	List * l = (List *) malloc(sizeof(List));

	if (l != NULL) {
		l->begin = NULL;
	}

	return l;
}

void List_insert(List * l, void * value) {

	Node * node = (Node *) malloc(sizeof(Node));

	if(node != NULL) {
		node->value = value;

		node->next = l->begin;
		l->begin = node;
	}
}

bool List_remove(List * l, void * value) {

	Node * node = l->begin;
	bool is_modified = false;

	while(node->next != NULL) {
		if(node->next->value == value) {
			Node * aux = node->next;

			node->next = node->next->next;
			free(aux);

			is_modified = true;

			break;
		}
	}

	return is_modified;
}


void List_destroy(List * list) {

	Node * node = list->begin;
	Node * aux = NULL;

	while(node != NULL) {
		aux = node;
		node = node->next;

		free(aux);
	}

	free(list);
}
