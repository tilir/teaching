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

#include <stdio.h>
#include "cache.h"

int main(int argc, char **argv) {
  int m, n;
  int num_hit = 0;
  struct cache *cch;
  int res;

  res = scanf("%d%d", &m, &n);
  if (!res) {
    fprintf(stderr, "Usage: %s <m> <n> <n elts...>\n", argv[0]);
    return 1;
  }

  cch = create_cache(m);

  for (int i = 0; i < n; ++i) {
    int next;
    res = scanf("%d", &next);
    if (!res) {
      fprintf(stderr, "Failed to read %d out of %d elements\n", i, n);
      fprintf(stderr, "Usage: %s <m> <n> <n elts...>\n", argv[0]);
      return 1;
    }

    num_hit += cache(cch, next);
  }

  printf("%d\n", num_hit);

  delete_cache(cch);
}

