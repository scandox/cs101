#include <stdlib.h>
#include <stdio.h>

struct List {
  int length;
  struct ListItem * first;
  struct ListItem * last;
};
