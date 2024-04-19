//-----------------------------------------------------------------------------
//
//  List source
//
//-----------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct node {
  struct node *next;
  struct node *prev;
  size_t val;
};

struct list {
  struct node *head;
  struct node *tail;
  size_t size;
};

const int LISTGARBAGE = -1;

List *create_list(size_t number_of_elements) {
  List *list = (List *)calloc(1, sizeof(List));
  Node *list_temp;

  list->size = number_of_elements;

  list->head = (Node *)calloc(1, sizeof(Node));
  list_temp = list->head;
  list_temp->val = LISTGARBAGE;

  for (size_t i = 0; i < number_of_elements - 1; i++) {
    list_temp->next = (Node *)calloc(1, sizeof(Node));
    list_temp->next->val = LISTGARBAGE;
    list_temp->next->prev = list_temp;

    list_temp = list_temp->next;
  }

  list->tail = list_temp;

  list->tail->next = list->head;
  list->head->prev = list->tail;

  return list;
}

void delete_list(List *list) {
  Node *temp = list->head;
  Node *to_free;

  for (size_t i = 0; i < list->size; ++i) {
    to_free = temp;
    temp = temp->next;
    free(to_free);
  }

  free(list);
}

void move_to_head(List *list, Node *new_head) {
  if (new_head == NULL)
    return;

  if (new_head == list->head)
    return;

  if (new_head == list->tail) {
    list->head = new_head;
    list->tail = new_head->prev;
    return;
  }

  new_head->prev->next = new_head->next;
  new_head->next->prev = new_head->prev;

  new_head->next = list->head;
  new_head->prev = list->tail;

  list->head->prev = new_head;
  list->tail->next = new_head;

  list->head = new_head;
}

Node *add_to_head(List *list, int val) {
  list->tail->val = val;
  list->head = list->tail;
  list->tail = list->tail->prev;

  return list->head;
}

Node *get_head(const List *list) { return list->head; }

Node *get_tail(const List *list) { return list->tail; }

int get_value(const Node *node) { return node->val; }

void list_dump(const List *list) {
  Node *list_temp = list->head;

  for (size_t i = 0; i < list->size; ++i) {
    printf("%ld ->", list_temp->val);
    list_temp = list_temp->next;
  }

  printf("NULL\n");
}

