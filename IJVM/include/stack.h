#ifndef ijvm_stack
#define ijvm_stack
#include <stdio.h>
#include "ijvm.h"

struct stack {
        int max_size;
        int current_size;
        word_t *elements;
} *stack_ijvm;

struct stack * init_stack(int max_size);

void push(word_t element);

word_t pop();

word_t top();

word_t size();

void print_stack();

#endif
