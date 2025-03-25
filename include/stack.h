#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct node {
    struct node* next;
    uint16_t data;
} node;

typedef struct stack {
    node* top;
} stack;

stack* init_stack(void);
stack* construct_stack(uint16_t first);
void print_stack(stack* st);
uint16_t pop(stack* st);
void push(stack* st, uint16_t data);
void free_stack(stack* st);
bool is_empty(stack* st);
