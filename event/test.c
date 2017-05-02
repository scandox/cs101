#include <stdlib.h>
#include <stdio.h>
#include "event.h"

void cb1(void * data) {
  printf("Data: %s", data);
}

void cb2(void * data) {
  printf("Data: %s", data);
}

void cb3(void * data) {
  printf("Data: %s", data);
}

int main() {
  struct Event * my_event = init_event("my_event");
  struct EventHandler * item, * item1, * item2, * item3;
  
  item1 = add_handler(my_event, &cb1);
  item2 = add_handler(my_event, &cb2);
  item3 = add_handler(my_event, &cb3);

  // Walk the list
  for (item = my_event->head; item!=NULL; item=item->next) {
    item->callback("Shite");
  }

  // Remove Items
  remove_handler(my_event, item1);
  remove_handler(my_event, item2);
  remove_handler(my_event, item3);

  // Ensure newly emptied list still works
  item1 = add_handler(my_event, &cb1);
  item2 = add_handler(my_event, &cb2);
  item3 = add_handler(my_event, &cb3);

  print_event(my_event);

  destroy_event(my_event);
}
