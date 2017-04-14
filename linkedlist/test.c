#include "linkedlist.h"

int main() {
  struct List * my_list = init_list();
  struct ListItem * item1, * item2, * item3;
  
  item1 = add_item(my_list, "One");
  item2 = add_item(my_list, "Two");
  item3 = add_item(my_list, "three");

  print_list(my_list);

  rem_item(my_list, item1);
  rem_item(my_list, item2);
  rem_item(my_list, item3);

  print_list(my_list);

  destroy_list(my_list);
}
