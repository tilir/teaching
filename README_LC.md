# Cache project

## Brief description:
This is a school project to implement cache functions. In the last implementation it uses [a doubly linked lsit](https://en.wikipedia.org/wiki/Doubly_linked_list) and a hash table with a separate chaining collision resolution (see [hash table](https://en.wikipedia.org/wiki/Hash_table)).

## Usage 
To install this repository tap in your console:
```
$ git clone git@github.com:UsoltsevI/Cache.git
```

The [tests](/tests) folder contatins testing programs for hashtable and for linked list. Also, there are may be some folders with testing data. 

The section will be finalized later...

### Cache:
Included files:
* <stdio.h>
* <stdlib.h>
* ["/include/list.h"](/include/list.h)
* ["/include/hash.h"](/include/hash.h)
* ["/include/cache.h"](/include/cache.h)

Files for joint linking:
* [/source/list.c](/source/list.c)
* [/source/hash.c](/source/hash.c)

Typedefs:
```
typedef int CacheValueType;
```

Structures:
```
struct cache;
```

Functions:
```
// creates cache with specified size and returns link to it
struct cache* create_cache(size_t size);

// This function adds the value to cache 
// and returns 1 if hit, 0 if miss
int cache(struct cache* cch, CacheValueType value);

void delete_cache(struct cache* cch);
```

### List:
Included files:
* <stdio.h>
* <stdlib.h>
* ["/include/list.h"](/include/list.h)

Typedefs:
```
typedef int ListValueType;
```

Structures:
```
struct node;
struct list;
```

Functions:
```
// creates list with specified size and returns link to it
struct list* create_list(size_t number_of_elements);

// move node from it's porition to list's head
void move_to_head(struct list* lst, struct node* nd);

// adds value to list's head and returns link to head
struct node* add_to_head(struct list* lst, ListValueType value);

// returns link to head
struct node* get_head(struct list* lst);

// returns link to tail
struct node* get_tail(struct list* lst);

// returns value from node
ListValueType get_value(struct node* nd);

void delete_list(struct list* lst);
```

### Hashmap:
Included files:
* <stdio.h>
* <stdlib.h>
* ["/include/list.h"](/include/list.h)
* ["/include/hash.h"](/include/hash.h)

Files for joint linking:
* [/source/list.c](/source/list.c)

Typedefs:
```
typedef struct node* HashValueType;
typedef int HashKeyType;
```

Structures:
```
struct table;
```

Functions:
```
// creates a list with specified size and returns the link to it
struct table* create_table(size_t size);

// adds node with value (key == value)
void add_value(struct table* tbl, HashValueType value, HashKeyType key);

// returns link to node if it is founded
// returns NULL if not founded
struct node* search_cell(struct table* tbl, HashKeyType key);

// removes the cell (from local linked list)
// with value == value,
// but not removes the public linked list's node.
// returns link to removed node (belonging to public linked list, 
// not to local linked list), 
// and NULL if there is no cell with such value
struct node* delete_cell(struct table* tbl, HashKeyType key);

void delete_table(struct table* tbl);
```

## Useful links:
* [Cache replacement policies](https://en.wikipedia.org/wiki/Cache_replacement_policies)
* [Hash table](https://en.wikipedia.org/wiki/Hash_table)
* [Linked list](https://en.wikipedia.org/wiki/Linked_list)
* [Cache_(computing)](https://en.wikipedia.org/wiki/Cache_(computing))
* [Full Stack Development](https://roadmap.sh/full-stack)
* [How to write README.md](https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax)

In Russian:
* [Алгоритмы кэширования](https://ru.wikipedia.org/wiki/Алгоритмы_кэширования)
* [Хэш-таблица](https://ru.wikipedia.org/wiki/Хеш-таблица)
* [Связный список](https://ru.wikipedia.org/wiki/Связный_список)
* [Кэш](https://ru.wikipedia.org/wiki/Кэш)
