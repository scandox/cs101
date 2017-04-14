#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linkedlist.h"

/* Returns a new list

*/
struct List * init_list() {
  
  struct List * list = malloc(sizeof(struct List));
  list->next = NULL;
  list->length = 0;
  return list;

}

/* Prepends item to list

*/
struct ListItem * add_item(struct List * list, char * item) {
  
  size_t item_length = strlen(item);
  struct ListItem * list_item = malloc(sizeof(struct ListItem));
  
  list_item->next=list->next;
  list_item->item = malloc(sizeof(char) * item_length + 1);
  strncpy(list_item->item, item, item_length + 1);

  list->next = list_item;
  list->length++;

  return list_item;
}

/* Remove an item from the list 

*/
void rem_item(struct List * list, struct ListItem * list_item) {

  struct ListItem ** head = &list->next;

  while ((*head) != list_item) {
    head = &(*head)->next;
  }

  (*head) = list_item->next;
  free(list_item->item);
  free(list_item);
  list->length--;

}

/* Print out the list */
void print_list(struct List * list) {
 
  struct ListItem * head = list->next;

  while (head != NULL) {
    printf("%s->", head->item);
    head = head->next;
  }  

  printf("\n");

}

/* Frees memory used by a list 

*/
void destroy_list(struct List * list) {
   
  struct ListItem * head = list->next;
  struct ListItem * remove;

  while (head != NULL) {
    remove = head; 
    head = head->next;
    free(remove->item);
    free(remove);
  }

  free(list);
}
