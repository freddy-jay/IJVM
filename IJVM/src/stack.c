#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

struct stack *init_stack(int max_size)
{
        stack_ijvm = (struct stack *)malloc(sizeof(struct stack));

        stack_ijvm->max_size = max_size;
        stack_ijvm->current_size = -1;
        stack_ijvm->elements = (word_t *)malloc(sizeof(word_t) * max_size);
        printf("\n--> init_stack() <--\n\n - Addr %x \n - max_size %d \n - Stack Addr %x \n--- \n", stack_ijvm, stack_ijvm->max_size, stack_ijvm->elements);

        return stack_ijvm;
}

void push(word_t element)
{
        stack_ijvm->elements[stack_ijvm->current_size+1] = element;
        stack_ijvm->current_size++;
        printf("- Pushed: %d\n", top());
}

word_t pop()
{
        word_t pop_element = stack_ijvm->elements[stack_ijvm->current_size];
        stack_ijvm->elements[stack_ijvm->current_size] = NULL;
        stack_ijvm->current_size--;

        return pop_element;
}

word_t top()
{
        return stack_ijvm->elements[stack_ijvm->current_size];
}

word_t size()
{
        return stack_ijvm->current_size;
}

void print_stack()
{
        printf("\n--> print_stack() <--\n\n");

        if(stack_ijvm->current_size != -1) {
                for (size_t i = 0; i <= stack_ijvm->current_size; i++) {
                        printf("[%d] - %d\n", i, stack_ijvm->elements[i]);
                }
        } else {
                printf("Stack is empty\n");
        }

        printf("\n---\n");
}
