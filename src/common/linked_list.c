#include "linked_list.h"
#include <stdlib.h>
#include <assert.h>

struct linked_list_cell
{
    void *value;
    struct linked_list_cell *next;
    struct linked_list_cell *prev;
};
struct linked_list
{
    linked_list_cell_t *first;
    linked_list_cell_t *last;
    size_t length;
    void (*free_func)(void *);
};

linked_list_cell_t *linked_list_cell_next(linked_list_cell_t *c)
{
    return c->next;
}

linked_list_cell_t *linked_list_cell_prev(linked_list_cell_t *c)
{
    return c->prev;
}

void *linked_list_cell_get_data(linked_list_cell_t *c)
{
    return c->value;
}

linked_list_cell_t *create_cell(void *val)
{
    linked_list_cell_t *cell = malloc(sizeof(linked_list_cell_t));
    cell->value = val;
    cell->next = NULL;
    cell->prev = NULL;
    return cell;
}

void cell_free(linked_list_cell_t *c, linked_list_t *l)
{
    l->free_func(c->value);
    free(c);
}

linked_list_t *linked_list_create(void (*free_func)(void *))
{
    linked_list_t *l = malloc(sizeof(linked_list_t));
    l->first = NULL;
    l->last = NULL;
    l->length = 0;
    l->free_func = free_func;
    return l;
}

void linked_list_free(linked_list_t *l)
{
    while (linked_list_remove_last(l))
        ;
    free(l);
}

size_t linked_list_length(linked_list_t *l)
{
    return l->length;
}
void *linked_list_get_first(linked_list_t *l)
{
    if (l->first == NULL)
    {
        return NULL;
    }
    return l->first->value;
}
void *linked_list_get_last(linked_list_t *l)
{
    if (l->last == NULL)
    {
        return NULL;
    }
    return l->last->value;
}

linked_list_cell_t *linked_list_first_cell(linked_list_t *l){
    return l->first;
}

linked_list_cell_t *linked_list_last_cell(linked_list_t *l){
    return l->last;
}

void linked_list_push(linked_list_t *l, void *val)
{
    linked_list_cell_t *c = create_cell(val);
    c->prev = l->last;
    l->last = c;
    if (c->prev == NULL)
    {
        l->first = c;
    }
    else
    {
        c->prev->next = c;
    }
    ++l->length;
}

void linked_list_queue(linked_list_t *l, void *val)
{
    linked_list_cell_t *c = create_cell(val);
    c->next = l->first;
    l->first = c;
    if (c->next == NULL)
    {
        l->last = c;
    }
    else
    {
        c->next->prev = c;
    }
    ++l->length;
}

bool linked_list_remove_last(linked_list_t *l)
{
    if (l->last == NULL)
    {
        return false;
    }

    linked_list_cell_t *new_last = l->last->prev;
    cell_free(l->last, l);
    l->last = new_last;
    if (l->last != NULL)
    {
        l->last->next = NULL;
    }
    --l->length;
    return true;
}
bool linked_list_remove_first(linked_list_t *l)
{
    if (l->first == NULL)
    {
        return false;
    }
    linked_list_cell_t *new_first = l->first->next;
    cell_free(l->first, l);
    l->first = new_first;
    if (l->first != NULL)
    {
        l->first->prev = NULL;
    }
    --l->length;
    return true;
}