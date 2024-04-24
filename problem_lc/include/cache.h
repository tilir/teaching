//-----------------------------------------------------------------------------
//
//  Cache header
//
//-----------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//-----------------------------------------------------------------------------

#pragma once

#include <stddef.h>

// cached type
typedef int CacheValueType;

// handle to cache
struct cache;

// constructor
struct cache *create_cache(int size);

// This function adds the value to cache returns 1 if hit, 0 if miss
int cache(struct cache *cch, CacheValueType value);

// destructor
void delete_cache(struct cache *cch);
