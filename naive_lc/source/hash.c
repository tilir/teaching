//-----------------------------------------------------------------------------
//
//  Hash source
//
//-----------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//-----------------------------------------------------------------------------

#include "hash.h"
#include "list.h"
#include <assert.h>
#include <malloc.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>

typedef struct hashnode {
  THashContent cont;
  THashValue value;
  struct hashnode *next;
} t_node;

struct table {
  t_node **cells;
  t_node *accumulating_list;
  size_t size;
};

static size_t get_hash(TMap *table, THashValue value);

static void delete_chain(t_node *head);

TMap *create_table(size_t size) {
  t_node *cur = NULL;
  TMap *tbl = NULL;

  tbl = (TMap *)calloc(1, sizeof(TMap));
  if (!tbl)
    return NULL;
  tbl->size = size;

  tbl->cells = (t_node **)calloc(size, sizeof(t_node *));
  if (!tbl->cells)
    return NULL;

  tbl->accumulating_list = (t_node *)calloc(1, sizeof(t_node));
  if (!tbl->accumulating_list)
    return NULL;

  cur = tbl->accumulating_list;

  for (size_t i = 0; i < size - 1; ++i) {
    cur->next = (t_node *)calloc(1, sizeof(t_node));
    if (!cur->next)
      return NULL;
    cur = cur->next;
  }

  return tbl;
}

static size_t get_hash(TMap *table, THashValue value) {
  const double a = 0.6180339887;
  size_t pos = 0;
  pos = (int)table->size * ((a * (value)) - (int)((a * (value))));
  return pos;
}

static void delete_chain(t_node *head) {
  t_node *tofree;

  while (head != NULL) {
    tofree = head;
    head = head->next;
    free(tofree);
  }
}

void delete_table(TMap *tbl) {
  for (size_t i = 0; i < tbl->size; ++i)
    delete_chain(tbl->cells[i]);
 
  free(tbl->cells);
  free(tbl->accumulating_list);
  free(tbl);
}

void add_value(TMap *table, THashContent cont, THashValue value) {
  size_t position = get_hash(table, value);
  t_node *old_head = table->cells[position];
  table->cells[position] = table->accumulating_list;
  table->accumulating_list = table->accumulating_list->next;
  table->cells[position]->next = old_head;
  table->cells[position]->cont = cont;
  table->cells[position]->value = value;
}

THashContent delete_cell(TMap *table, THashValue value) {
  size_t position = get_hash(table, value);
  t_node *cur = NULL;
  t_node *prev = NULL;
  THashContent save = NULL;

  cur = table->cells[position];

  while (cur != NULL) {
    if ((cur->value == value) && (prev != NULL)) {
      prev->next = cur->next;
      save = cur->cont;
      cur->next = table->accumulating_list;
      table->accumulating_list = cur;
      return save;

    } else if ((cur->value == value) && (prev == NULL)) {
      save = cur->cont;
      table->cells[position] = cur->next;
      cur->next = table->accumulating_list;
      table->accumulating_list = cur;

      return save;
    }
    prev = cur;
    cur = cur->next;
  }
  return NULL;
}

THashContent search_cell(TMap *table, THashValue value) {
  size_t position = get_hash(table, value);
  t_node *cur = table->cells[position];
  while (cur != NULL) {
    if (cur->value == value)
      return cur->cont;
    cur = cur->next;
  }
  return NULL;
}

static void print_hash_list(t_node *head) {
  t_node *cur = head;

  while (cur != NULL) {
    printf("|| value: %lu ", cur->value);
    cur = cur->next;
  }

  printf("||\n");
}

void print_hash_table(const TMap *table) {
  assert(table);
  printf("table size: %lu\n", table->size);
  for (int i = 0; i < table->size; ++i) {
    if (table->cells[i] != NULL) {
      printf("//////////////////////////////\n");
      printf("cell number %i\n", i);
      print_hash_list(table->cells[i]);
      printf("//////////////////////////////\n");
    }
  }
}
