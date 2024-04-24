//-----------------------------------------------------------------------------
//
//  Cache source
//
//-----------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//-----------------------------------------------------------------------------

#include "cache.h"
#include "hash.h"
#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct cache {
  struct table *tbl;
  struct list *lst;
};

struct cache *create_cache(int size) {
  struct cache *cch = (struct cache *)calloc(1, sizeof(struct cache));
  if (!cch)
    return NULL;
  cch->tbl = create_table(size);
  cch->lst = create_list(size);
  return cch;
}

int cache(struct cache *cch, CacheValueType value) {
  struct node *nd; // C90 style, but problem_LC requires it...

  assert(cch);
  nd = search_cell(cch->tbl, value);

  // if the suitable cell found just move it to the head
  if (nd != NULL) {
    move_to_head(cch->lst, nd);
    return 1;
  }

  // if there is no such cell in the table delete tail from table
  delete_cell(cch->tbl, get_value(get_tail(cch->lst)));

  // Then, adding this value to the list's head
  // (the tail removes automatically, see the list's implementation)
  add_to_head(cch->lst, value);

  // and, finally, adds this node to hashmap
  add_value(cch->tbl, get_head(cch->lst), value);
  return 0;
}

void delete_cache(struct cache *cch) {
  delete_table(cch->tbl);
  delete_list(cch->lst);
  free(cch);
}
