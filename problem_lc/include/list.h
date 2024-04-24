//-----------------------------------------------------------------------------
//
// Linked list header
//
//-----------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//-----------------------------------------------------------------------------

#pragma once

#include <stddef.h>

// handles for list and for single node
struct node;
struct list;

typedef struct list List;
typedef struct node Node;

// creates list or returns NULL
List *create_list(int number_of_elements);

// moves node to list head
void move_to_head(List *list, Node *new_head);

// destructor
void delete_list(List *list);

// pushes node to list head
Node *add_to_head(List *list, int val);

// for debug purposes
void list_dump(const List *list);

// get list head
Node *get_head(const List *list);

// get list tail
Node *get_tail(const List *list);

// extract value from node
int get_value(const Node *node);
