#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "event.h"

/* Returns a new list

*/
struct Event * init_event(char * name) {
  
  size_t name_length = strlen(name);
  struct Event * event = malloc(sizeof(struct Event));
  
  event->head = NULL;
  event->last = NULL;
  event->handlers = 0;
  event->name = malloc(sizeof(char) * name_length + 1);
  strncpy(event->name, name, name_length + 1);
  return event;

}

/* Appends item to list

*/
struct EventHandler * add_handler(struct Event * event, void (*callback)(void * data)) {
  
  struct EventHandler * event_handler = malloc(sizeof(struct EventHandler));

  event_handler->next=NULL;
  event_handler->callback = callback;

  if (event->last == NULL) {
    event->head = event_handler;
  }else{ 
    event->last->next = event_handler;
  }
  event->last = event_handler;
  event->handlers++;

  return event_handler;
}

/* Remove an item from the list 

*/
void remove_handler(struct Event * event, struct EventHandler * event_handler) {

  // Pointer to pointer contains address of pointer to first struct
  struct EventHandler ** head = &event->head;

  // Dereferencing head gives us the address of the current struct
  while ((*head) != event_handler) {
    // sets contents of head to address of pointer to next struct
    head = &(*head)->next;
  }

  // Sets the address in the pointer to current struct to be the address of the
  // next struct instead. We repoint the relevant pointer
  (*head) = event_handler->next;
  
  // Ensure the last item pointer is still pointing at the last item
  if (event_handler == event->last) event->last = (*head);
  
  free(event_handler);
  event->handlers--;

}

/* Print out the list */
void print_event(struct Event * event) {
 
  struct EventHandler * head = event->head;
  
  printf("Event: %s", event->name);

  while (head != NULL) {
    printf("%p->", head->callback);
    head = head->next;
  }  

  printf("\n");

}

/* Frees memory used by a list 

*/
void destroy_event(struct Event * event) {
   
  struct EventHandler * head = event->head;
  struct EventHandler * remove;

  while (head != NULL) {
    remove = head; 
    head = head->next;
    free(remove);
  }

  free(event->name);
  free(event);
}
