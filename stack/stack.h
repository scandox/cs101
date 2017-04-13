#include <stdbool.h>

struct Stack {
  char * items;
  int size;
  int item_count;
};

struct Stack * init_stack();
void push(struct Stack * stack, char item);
char pop(struct Stack * stack);
void destroy_stack(struct Stack * stack);
