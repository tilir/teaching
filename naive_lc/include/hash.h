//-----------------------------------------------------------------------------
//
// Hash map header
//
//-----------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//-----------------------------------------------------------------------------

#pragma once

#include "list.h"
#include <stddef.h>
#include <stdlib.h>

// Handles to internal structures
struct table;
struct node;

// Hashed content type
typedef struct node *THashContent;

// Hashed key type
typedef size_t THashValue;

// Hash map type
typedef struct table TMap;

// creates a list with specified size and returns the link to it
TMap *create_table(size_t size);

// deletes table
void delete_table(TMap *table);

// adds node with value (key == value)
void add_value(TMap *table, THashContent cont, THashValue value);

// find cell with value
THashContent search_cell(TMap *table, THashValue value);

// removes the cell (from local linked list)
// with value == value,
// but not removes the public linked list's node.
// returns link to removed node (belonging to public linked list,
// not to local linked list),
// and NULL if there is no cell with such value
THashContent delete_cell(TMap *table, THashValue value);

// dump for debug purposes
void print_hash_table(const TMap *tbl);
