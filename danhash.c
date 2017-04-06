#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "danhash.h"

unsigned long hash(char *key)
{
    unsigned long hash, i;
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

unsigned int getBucketIndex(unsigned int table_size, char * key)
{
  return (hash(key)%table_size);
}

/*  Initializes a dictionary

*/
struct Dictionary * init_danhash(unsigned int size) {
  struct Dictionary * newDict;
  int i;

  /* Initialize table struct */
  newDict = malloc(sizeof(struct Dictionary));
  newDict->size = size;
  newDict->elements = 0;

  /* Initialize memory for size items */
  newDict->table = malloc(size * sizeof(struct Entry));

  /* Initialize each head entry with NULL */
  for (i = 0; i < size; i++) {
    *(newDict->table)=NULL;
    (newDict->table)++;
  }

  return newDict;

}

/* Doubles the size of the Dictionary  */
int expand_dictionary(struct Dictionary * dict) {

  /* Define a new table of double size */
  struct Entry ** newTable = malloc(dict->size * 2 * sizeof(struct Entry));

  /* Store reference to old table and size */
  int old_size = dict->size;
  struct Entry ** oldTable = dict->table;

  /* Point to new table, set new size, reset elements and rehash every item from old table */
  dict->table = newTable;
  dict->size = old_size*2;
  dict->elements = 0;

  int i;
  struct Entry * entry, * oldEntry;
  for (i = 0; i < old_size; i++) {
    entry = *(oldTable+i);
    while (entry!=NULL) {
      add_danhash(dict, entry->key, entry->value);
      oldEntry = entry;
      entry = entry->next;
      free(oldEntry);
    }
  }

  return 0;

}

/* Inserts a KV pair

   Duplicates get value replaced
   Entries added to head of list
   Added entries can be freed with rem

*/
int add_danhash(struct Dictionary * dict, char * key, char * value)
{
  unsigned int bucketIndex = getBucketIndex(dict->size, key);
  struct Entry * newEntry;
  struct Entry * existingEntry=NULL;

  existingEntry = get_danhash(dict, key);

  if (existingEntry == NULL) {

    newEntry = malloc(sizeof(struct Entry));
    newEntry->key = strdup(key);
    newEntry->value = strdup(value);

    if (*(dict->table+bucketIndex)==NULL) {
      newEntry->next = NULL;
    }else{
      // Because we're prepending, we want the next 
      // entry to point to the start of the current list
      newEntry->next = *(dict->table+bucketIndex);
    }

    *(dict->table+bucketIndex) = newEntry;
    dict->elements++;

    /* If load factor exceeds optimal range expand Dictionary */
    if (((double)dict->elements/dict->size) > 0.75) {
      expand_dictionary(dict);
    }

  }else{
  
    free(existingEntry->value);
    existingEntry->value = malloc(sizeof(char) * strlen(value));
    strncpy(existingEntry->value, value, strlen(value));
  
  }

  return 0;
}

/* Get an entry 

*/ 
struct Entry * get_danhash(struct Dictionary * dict, char * key)
{
  struct Entry * result = NULL;
  unsigned int bucketIndex = getBucketIndex(dict->size, key);
  struct Entry * entry;

  for (entry=*(dict->table+bucketIndex); entry!= NULL; entry = entry->next){
    if (strcmp(entry->key, key)==0) {
      result = malloc(sizeof(struct Entry));
      memcpy(result, entry, sizeof(struct Entry));
      break;
    }
  }

  return result;
}

/* 
Removes an entry 

   Removes item from linked list and frees entry

*/
int rem_danhash(struct Dictionary * dict, char * key) {
  unsigned int bucketIndex = getBucketIndex(dict->size, key);
  struct Entry ** entry = &(*(dict->table+bucketIndex));
  struct Entry * removed;

  // The Bucket is completely empty
  if (*entry == NULL) return 0;

  do {
    // If keys match remove and free
    if (strcmp((*entry)->key, key)==0) {
      removed = *entry;
      *entry = (*entry)->next;
      free(removed);
      dict->elements--;
      break;
    }
    
    // Move to next entry in list
    entry = &((*entry)->next);

    // If it's NULL quit
  } while(*entry!=NULL); 

  return 0;
}


/* Outputs a basic representation of Hashtable contents 

*/
void print_danhash(struct Dictionary * dict) {
  unsigned int i;
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
