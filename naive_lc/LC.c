#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct list List;

typedef struct node Node;

List* create_list(size_t number_of_elements);

void move_to_head(List* list, Node* new_head);

void delete_list(List* list);

Node* add_to_head(List* list, int val);

void list_dump (List* list);

Node* get_head(List* list);

Node* get_tail(List* list);

int get_value(Node* node);


struct node {
    struct node* next;
    struct node* prev; 
    size_t val;
};

struct list {
    struct node* head;
    struct node* tail;
    size_t size;
};

const int LISTGARBAGE = -1;

List* create_list (size_t number_of_elements){
    List* list = (List*) calloc(1, sizeof(List));
    Node* list_temp;

    list->size = number_of_elements;

    list->head = (Node*) calloc(1, sizeof(Node));
    list_temp = list->head;
    list_temp->val = LISTGARBAGE;

    for (size_t i = 0; i < number_of_elements - 1; i++) {
        list_temp->next = (Node*) calloc(1, sizeof(Node));
        list_temp->next->prev = list_temp;
        list_temp->next->val = LISTGARBAGE;

        list_temp = list_temp->next;  
    }

    list->tail = list_temp;
    
    list->tail->next = list->head;
    list->head->prev = list->tail;

    return list;
}

void delete_list (List* list) {
    Node* temp = list->head;
    Node* to_free;

    for (size_t i = 0; i < list->size; ++i) {
        to_free = temp;
        temp = temp->next;
        free(to_free);
    }
    // free(list_temp);
    free(list);
}

void move_to_head(List* list, Node* new_head) {
    if (new_head == NULL) {
        return;
    }
    
    if (new_head == list->head) {
        return;
    }
    
    if (new_head == list->tail) {
        list->head = new_head;
        list->tail = new_head->prev;
        return;
    }

    new_head->prev->next = new_head->next;
    new_head->next->prev = new_head->prev;

    list->head->prev = new_head;
    new_head->next = list->head;
    new_head->prev = list->tail;

    list->tail->next = new_head;

    list->head = new_head;
    list->tail = list->head->prev;
}

Node* add_to_head(List* list, int val) {
    list->tail->val = val;
    list->head = list->tail;
    list->tail = list->tail->prev;

    return list->head;
}

Node* get_head(List* list) {
    return list->head;
}

Node* get_tail(List* list) {
    return list->tail;
}

int get_value(Node* node) {
    return node->val;
}

void list_dump(List* list) {
    Node* list_temp = list->head;

    for (size_t i = 0; i < list->size; ++i) {
        printf("%ld ->", list_temp->val);
        list_temp = list_temp->next;
    }

    printf("NULL\n");
}



typedef struct node* THashContent; // == Hash Content Type
//struct list; // included from list.h

typedef size_t THashValue; // == Hash Value Type. Ну то есть то, по чему мы ищем значение в content

typedef struct table TMap; // == Hash Map Type
struct table; // main struct defenition

// creates a list with specified size and returns the link to it
TMap* create_table(size_t size);

void delete_table(TMap* table);

// adds node with value (key == value)
void add_value(TMap* table, THashContent cont, THashValue value);

// find cell with value
THashContent search_cell(TMap* table, THashValue value);

// removes the cell (from local linked list)
// with value == value,
// but not removes the public linked list's node.
// returns link to removed node (belonging to public linked list,
// not to local linked list),
// and NULL if there is no cell with such value
THashContent delete_cell(TMap* table, THashValue value);

void print_hash_table(const TMap* tbl);

typedef struct _hashnode {
    THashContent cont;
    THashValue value;
    struct _hashnode* next;
} t_node;

struct table {
    t_node** cells;
    t_node* accumulating_list;
    size_t size;
};

static size_t get_hash(TMap* table, THashValue value);

static void print_chain(t_node head);

static void delete_chain(t_node* head);

static void push_node(t_node* head, t_node* node);

TMap* create_table(size_t size) {
    t_node* cur = NULL;
    TMap* tbl = NULL;

    tbl = (TMap *) calloc(1, sizeof(TMap));
    tbl->size = size;

    tbl->cells = (t_node**) calloc(size, sizeof(t_node*));

    tbl->accumulating_list = (t_node *) calloc(1, sizeof(t_node));

    cur = tbl->accumulating_list;

    for (size_t i = 0; i < size - 1; ++i) {
        cur->next = (t_node *) calloc(1, sizeof(t_node));
        cur = cur->next;
    }

    return tbl;
}

static size_t get_hash(TMap* table, THashValue value) {
    // можно использовать алгоритм проще: return value % table->size;
    const double a = 0.6180339887;
    size_t pos = 0;

    pos = (int)table->size * ((a * (value)) - (int)((a * (value))));
    return pos;
    // return value % table->size;
}

static void delete_chain(t_node* head) {
    t_node* tofree;

    while (head != NULL) {
        tofree = head;
        head = head->next;
        free(tofree);
    }
}

void delete_table(TMap* tbl) {
    for (size_t i = 0; i < tbl->size; ++i) {
        delete_chain(tbl->cells[i]);
    }

    free(tbl->cells);
    delete_chain(tbl->accumulating_list);
    free(tbl);
}

void add_value(TMap* table, THashContent cont, THashValue value) {
    size_t position = get_hash(table, value);
    
    t_node* old_head = table->cells[position];
    table->cells[position] = table->accumulating_list;
    table->accumulating_list = table->accumulating_list->next;
    table->cells[position]->next = old_head;
    table->cells[position]->cont = cont;
    table->cells[position]->value = value;
}

THashContent delete_cell(TMap* table, THashValue value) {
    size_t position = get_hash(table, value);
    t_node* cur = NULL;
    t_node* prev = NULL;
    THashContent save = NULL;

    cur = table->cells[position];

    while(cur != NULL) {
        if ((cur->value == value) && (prev != NULL)) {
            prev->next = cur->next;
            save = cur->cont;
            cur->next = table->accumulating_list;
            table->accumulating_list = cur;

            // printf("delete success\nvalue: %lu\n", value);
            return save;

        } else if ((cur->value == value) && (prev == NULL)) {
            save = cur->cont;
            table->cells[position] = cur->next;
            cur->next = table->accumulating_list;
            table->accumulating_list = cur;
            // printf("delete success\nvalue: %lu\n", value);

            return save;
        }

        prev = cur;
        cur = cur->next;
    }

    return NULL;
}

THashContent search_cell(TMap* table, THashValue value) {
    size_t position = get_hash(table, value);
    t_node* cur = table->cells[position];

    while(cur != NULL) {
        if (cur->value == value) {
            // printf("search success\nvalue: %lu\n", value);
            return cur->cont;
        }

        cur = cur->next;
    }

    return NULL;
}

static void print_hash_list(t_node* head) {
    t_node* cur = head;
    while(cur != NULL) {
        printf("|| value: %lu ",cur->value);
        cur = cur->next;
    }
    printf("||\n");
}

void print_hash_table(const TMap* table) {
    printf("table size: %lu\n", table->size);
    for(int i = 0; i < table->size; ++i) {
        if (table->cells[i] != NULL) {
            printf("//////////////////////////////\n");
            printf("cell number %i\n", i);
            print_hash_list(table->cells[i]);
            printf("//////////////////////////////\n");
        }
    }
}

typedef int CacheValueType;
struct cache;

// constructor
struct cache* create_cache(size_t size);

// This function adds the value to cache 
// and returns 1 if hit, 0 if miss
int cache(struct cache* cch, CacheValueType value);

// destructor
void delete_cache(struct cache* cch);

struct cache {
    struct table* tbl;
    struct list* lst;
};

// constructor
struct cache* create_cache(size_t size) {
    struct cache* cch = (struct cache* ) calloc(1, sizeof(struct cache));

    cch->tbl = create_table(size);
    cch->lst = create_list(size);

    return cch;
}

int cache(struct cache* cch, CacheValueType value) {
    struct node* nd; // C90 style, but problem_LC requires it...

    nd = search_cell(cch->tbl, value);
    
    // if the sutable cell has been founded
    // just move it to the head
    if (nd != NULL) {
        move_to_head(cch->lst, nd);
        return 1;
    }

    // if there is no such cell in the table
    // delete tail from table
    delete_cell(cch->tbl, get_value(get_tail(cch->lst)));
    // Then, adding this value to the list's head
    // (the tail removes automatically, see the list's implementation)
    add_to_head(cch->lst, value);
    // and, finally, adds this node to hashmap
    add_value(cch->tbl, get_head(cch->lst), value);

    return 0;
}

// destructor
void delete_cache(struct cache* cch) {
    delete_table(cch->tbl);
    delete_list(cch->lst);
    free(cch);
}

// This is the main function for the problem_LC
// and separate compilation task. 
// You just need to copy it to LC.c before submitting.
int main() {
    size_t m, n;
    size_t num_hit = 0;
    struct cache* cch;
    int res = 0;

    res = scanf("%lu%lu", &m, &n);

    cch = create_cache(m);

    for (size_t i = 0; i < n; ++i) {
        int next;
        res = scanf("%d", &next);
        // printf("ghg\n");

        num_hit += cache(cch, next);
    }

    printf("%lu\n", num_hit);

    delete_cache(cch);

    return 0;
}
