#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "stack.h"

struct Stack * init_stack() {
  struct Stack * stack = malloc(sizeof(struct Stack));
  stack->items = malloc(50 * sizeof(char));
  stack->size = 50;
  stack->item_count = 0;

  return stack;
}

void push(struct Stack * stack, char item) {
  (stack->item_count)++;
  if (stack->item_count > stack->size) {
    stack->items = realloc(stack->items, stack->size * 2 * sizeof(char));
    stack->size = stack->size * 2;
  }
  *(stack->items + stack->item_count - 1) = item;
}

char pop(struct Stack * stack) {
  if (stack->item_count == 0) return -1;
  
  char item = *(stack->items + stack->item_count - 1);
  (stack->item_count)--;

  return item;
}

void destroy_stack(struct Stack * stack) {
  free(stack->items);
  free(stack);
}
