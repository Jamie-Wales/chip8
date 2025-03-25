#include "stack.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

stack* init_stack(void)
{
    stack* st = malloc(sizeof(stack));
    if (!st) {
        fprintf(stderr, "Error: could not allocate memory for stack");
        exit(EXIT_FAILURE);
    }
    st->top = NULL;
    return st;
}

stack* construct_stack(uint16_t first)
{
    stack* st = init_stack();
    push(st, first);
    return st;
}

void print_stack(stack* st)
{
    if (!st || !st->top) {
        printf("Stack is empty\n");
        return;
    }

    node* copy = st->top;
    while (copy) {
        printf("%04x\n", copy->data);
        copy = copy->next;
    }
}

uint16_t pop(stack* st)
{
    if (!st || !st->top) {
        fprintf(stderr, "Error: cannot pop from empty stack\n");
        exit(EXIT_FAILURE);
    }
    node* temp = st->top;
    uint16_t data = temp->data;
    st->top = st->top->next;
    free(temp);
    return data;
}

void push(stack* st, uint16_t data)
{
    if (!st) {
        fprintf(stderr, "Error: stack is NULL\n");
        exit(EXIT_FAILURE);
    }

    node* n = malloc(sizeof(node));
    if (!n) {
        fprintf(stderr, "Error: cannot allocate memory\n");
        exit(EXIT_FAILURE);
    }

    n->data = data;
    n->next = st->top;
    st->top = n;
}

void free_stack(stack* st)
{
    if (!st)
        return;

    while (st->top) {
        pop(st);
    }
    free(st);
}

bool is_empty(stack* st)
{
    return st && st->top;
}

uint16_t peek(stack* st)
{
    if (!st || !st->top) {
        fprintf(stderr, "Error: cannot peek from empty stack\n");
        exit(EXIT_FAILURE);
    }
    return st->top->data;
}
