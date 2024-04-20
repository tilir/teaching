//-----------------------------------------------------------------------------
//
// This is the main function for the problem_LC
//
//-----------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//-----------------------------------------------------------------------------

#include "cache.h"
#include "hash.h"
#include <stdio.h>

int main(int argc, char **argv) {
  size_t m, n;
  size_t num_hit = 0;
  struct cache *cch;
  int res;

  res = scanf("%lu%lu", &m, &n);
  if (!res) {
    fprintf(stderr, "Usage: %s <m> <n> <n elts...>", argv[0]);
    return 1;
  }

  cch = create_cache(m);

  for (size_t i = 0; i < n; ++i) {
    int next;
    res = scanf("%d", &next);
    if (!res) {
      fprintf(stderr, "Usage: %s <m> <n> <n elts...>", argv[0]);
      return 1;
    }

    num_hit += cache(cch, next);
  }

  printf("%lu\n", num_hit);

  delete_cache(cch);
}

