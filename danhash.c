#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "danhash.h"

/* Default hash function 

   The "Jenkins One-at-a-time hash", from an article by Bob Jenkins in Dr. Dobb's September 1997.

*/
uint32_t hash(char *key)
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


uint32_t getBucketIndex(struct Dictionary * dict, char * key)
{
  return ((*(dict->hash_function))(key) % dict->size);
}

/*  Initializes a dictionary

*/
struct Dictionary * init_danhash(uint32_t size, uint32_t (*hash_function)(char * key)) {
  struct Dictionary * newDict;
  uint32_t i;

  /* Initialize table struct */
  newDict = malloc(sizeof(struct Dictionary));
  newDict->size = size;
  newDict->elements = 0;
  
  // If no hash function supplied then attach default
  if (hash_function == NULL) {
    newDict->hash_function = &hash;
  }else{
    newDict->hash_function = hash_function;
  }

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
void expand_dictionary(struct Dictionary * dict) {

  /* Define a new table of double size */
  struct Entry ** newTable = malloc(dict->size * 2 * sizeof(struct Entry));

  /* Store reference to old table and size */
  uint32_t old_size = dict->size;
  struct Entry ** oldTable = dict->table;

  /* Point to new table, set new size, reset elements and rehash every item from old table */
  dict->table = newTable;
  dict->size = old_size*2;
  dict->elements = 0;

  uint32_t i;
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
}

/* Inserts a KV pair

   Duplicates get value replaced
   Entries added to head of list

*/
void add_danhash(struct Dictionary * dict, char * key, char * value)
{
  uint32_t bucketIndex = getBucketIndex(dict, key);
  struct Entry * newEntry;
  char * currentValue;

  currentValue = get_danhash(dict, key);

  if (currentValue != NULL) {
    rem_danhash(dict, key);
  }

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

}

/* Get an entry value 

   Caller needs to free the returned string themselves

*/ 
char * get_danhash(struct Dictionary * dict, char * key)
{
  char * result = NULL;
  uint32_t bucketIndex = getBucketIndex(dict, key);
  struct Entry * entry;

  for (entry=*(dict->table+bucketIndex); entry!= NULL; entry = entry->next){
    if (strcmp(entry->key, key)==0) {
      size_t value_length = strlen(key);
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

*/
int rem_danhash(struct Dictionary * dict, char * key) {
  uint32_t bucketIndex = getBucketIndex(dict, key);
  struct Entry ** entry = &(*(dict->table+bucketIndex));
  struct Entry * removed;

  // The Bucket is completely empty
  if (*entry == NULL) return -1;

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
