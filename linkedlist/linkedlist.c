#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linkedlist.h"

/* Returns a new list

*/
struct List * init_list() {
  
  struct List * list = malloc(sizeof(struct List));
  list->head = NULL;
  list->length = 0;
  return list;

}

/* Appends item to list

*/
struct ListItem * add_item(struct List * list, char * item) {
  
  size_t item_length = strlen(item);
  struct ListItem * list_item = malloc(sizeof(struct ListItem));

  list_item->next=NULL;
  list_item->item = malloc(sizeof(char) * item_length + 1);
  strncpy(list_item->item, item, item_length + 1);

  struct ListItem ** head = &list->head;
  while ((*head) != NULL) {
    head = &((*head)->next);
  }
  (*head) = list_item;
  list->length++;

  return list_item;
}

/* Remove an item from the list 

*/
void rem_item(struct List * list, struct ListItem * list_item) {

  // Pointer to pointer contains address of pointer to first struct
  struct ListItem ** head = &list->head;

  // Dereferencing head gives us the address of the current struct
  while ((*head) != list_item) {
    // sets contents of head to address of pointer to next struct
    head = &(*head)->next;
  }

  // Sets the address in the pointer to current struct to be the address of the
  // next struct instead. We repoint the relevant pointer
  (*head) = list_item->next;
  free(list_item->item);
  free(list_item);
  list->length--;

}

/* Print out the list */
void print_list(struct List * list) {
 
  struct ListItem * head = list->head;

  while (head != NULL) {
    printf("%s->", head->item);
    head = head->next;
  }  

  printf("\n");

}

/* Frees memory used by a list 

*/
void destroy_list(struct List * list) {
   
  struct ListItem * head = list->head;
  struct ListItem * remove;

  while (head != NULL) {
    remove = head; 
    head = head->next;
    free(remove->item);
    free(remove);
  }

  free(list);
}
