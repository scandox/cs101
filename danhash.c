#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "danhash.h"

/* PRIVATE FUNCTIONS */

/* Default hash function 

   The "Jenkins One-at-a-time hash", from an article by Bob Jenkins in Dr. Dobb's September 1997.

*/
uint32_t hash(const char *key)
{
    uint32_t hash, i;
    size_t len = strlen(key);
    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}


uint32_t get_bucket_index(struct Dictionary * dict, const char * key)
{
  return ((*(dict->hash_function))(key) % dict->size);
}

/* Doubles the size of the Dictionary  */
void expand_dictionary(struct Dictionary * dict) {

  /* Define a new table of double size */
  struct Entry ** new_table = malloc(dict->size * 2 * sizeof(struct Entry));

  /* Store reference to old table and size */
  uint32_t old_size = dict->size;
  struct Entry ** old_table = dict->table;

  /* Point to new table, set new size, reset elements and rehash every item from old table */
  dict->table = new_table;
  dict->size = old_size*2;
  dict->elements = 0;
 
  /* Initialize each head entry with NULL */
  for (int i = 0; i < dict->size; i++) {
    *(dict->table+i) = NULL;
  }

  uint32_t i;
  struct Entry * entry, * old_entry;
  for (i = 0; i < old_size; i++) {
    entry = *(old_table+i);
    while (entry!=NULL) {
      add_danhash(dict, entry->key, entry->value);
      old_entry = entry;
      entry = entry->next;

      free(old_entry->value);
      free(old_entry->key);
      free(old_entry);
    }
  }

  free(old_table);

}

/* PUBLIC API */

/*  Initializes a dictionary

    Call this first. Use a size that is approximately 1.5 X the number of elements
    you expect to add to the Dictionary
    
    Pass resulting dictionary object to each subsequent function call 
    
    The hash_function value can be NULL, in which case default hash function
    is used.

*/
struct Dictionary * init_danhash(uint32_t size, uint32_t (*hash_function)(const char * key)) {
  struct Dictionary * new_dict;
  uint32_t i;

  /* Initialize table struct */
  new_dict = malloc(sizeof(struct Dictionary));
  new_dict->size = size;
  new_dict->elements = 0;
  
  // If no hash function supplied then attach default
  if (hash_function == NULL) {
    new_dict->hash_function = &hash;
  }else{
    new_dict->hash_function = hash_function;
  }

  /* Initialize memory for size items */
  new_dict->table = malloc(size * sizeof(struct Entry));

  /* Initialize each head entry with NULL */
  for (i = 0; i < size; i++) {
    *(new_dict->table+i) = NULL;
  }

  return new_dict;

}

/* Inserts a Key-Value pair into the dictionary

   A few notes:
   *  Duplicate Key means Entry is replaced
   *  Entries are added to head of list
   *  When load factor is no longer optimal dictionary size is doubled, this can
      lead to occasional pauses on insert

*/
void add_danhash(struct Dictionary * dict, const char * key, const char * value)
{
  uint32_t bucket_index = get_bucket_index(dict, key);
  struct Entry * new_entry;
  size_t value_length = strlen(value);
  size_t key_length = strlen(key);
  char * current_value = get_danhash(dict, key);


  if (current_value != NULL) {
    rem_danhash(dict, key);
  }
  
  free(current_value);

  new_entry = malloc(sizeof(struct Entry));
  new_entry->key = malloc(sizeof(char) * key_length + 1);
  new_entry->value = malloc(sizeof(char) * value_length + 1);
  strncpy(new_entry->key, key, key_length + 1); 
  strncpy(new_entry->value, value, value_length + 1);

  if (*(dict->table+bucket_index)==NULL) {
    new_entry->next = NULL;
  }else{
    // Because we're prepending, we want the next 
    // entry to point to the start of the current list
    new_entry->next = *(dict->table+bucket_index);
  }

  *(dict->table+bucket_index) = new_entry;
  dict->elements++;

  /* If load factor exceeds optimal range expand Dictionary */
  if (((double)dict->elements/dict->size) > 0.75) {
    expand_dictionary(dict);
  }

}

/* Get an entry value, corresponding to the supplied Key 

   You need to free the value returned. This is still true even if you do
   call destroy_danhash or rem_danhash on the specific key - i.e. the string
   returned is a copy independent of the Dict.

*/ 
char * get_danhash(struct Dictionary * dict, const char * key)
{
  char * result = NULL;
  uint32_t bucket_index = get_bucket_index(dict, key);
  struct Entry * entry;

  for (entry=*(dict->table+bucket_index); entry!= NULL; entry = entry->next){
    if (strcmp(entry->key, key)==0) {
      size_t value_length = strlen(entry->value);
      result = malloc(sizeof(char) * value_length + 1);
      strncpy(result, entry->value, value_length + 1);
      break;
    }
  }

  return result;
}

/* 
Removes an entry 

   Removes item from linked list and frees entry
   Returns -1 if the Entry does not exist
   Returns 0 if successfully removed

*/
int rem_danhash(struct Dictionary * dict, const char * key) {
  uint32_t bucket_index = get_bucket_index(dict, key);
  struct Entry ** entry = &(*(dict->table+bucket_index));
  struct Entry * removed = NULL;

  // The Bucket is completely empty
  if (*entry == NULL) return -1;

  do {
    
    // If keys match remove and free
    if (strcmp((*entry)->key, key)==0) {
      removed = *entry;
      *entry = (*entry)->next;

      free(removed->value);
      free(removed->key);
      free(removed);
      dict->elements--;
      break;
    }
    
    // Move to next entry in list
    entry = &((*entry)->next);

    // If it's NULL quit
  } while(*entry!=NULL); 

  return (removed==NULL) ? -1 : 0;
}

/* Prints a textual representation of Hashtable contents 

   Entries are printed something like:

   Bucket 1: [ key, value ] -> [ key1, value1 ]
   ...
 
*/
void print_danhash(struct Dictionary * dict) {
  
  uint32_t i;
  struct Entry * entry;
  
  for (i = 0; i < (dict->size); i++) {
    entry = *(dict->table+i);
    printf("Bucket %d: ", i);
    while(entry != NULL) {
      printf("[%s,%s]->", entry->key, entry->value); 
      entry = entry->next;
    }
    printf("\n");
  }

}

/*  Removes and frees all entries and cleans up Dictonary object

    Always call this when you're finished with Dictionary

*/
void destroy_danhash(struct Dictionary * dict) {
  
  uint32_t i;
  struct Entry * entry, * remove;
  
  for (i = 0; i < (dict->size); i++) {
    entry = *(dict->table+i);
    while(entry != NULL) {
      remove = entry;
      entry = entry->next;
      rem_danhash(dict, remove->key);
    }
  }
  
  free(dict->table);
  free(dict);

}
