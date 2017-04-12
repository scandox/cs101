#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <unistd.h>
#include "danhash.h"

int main()
{

    char c;
  char line[100];
  int i = 0;
  FILE * f;
  
  // Start the dictionary at an arbitrary size
  struct Dictionary * dandict = init_danhash(5, NULL);
  char * key, * value;
  
  // Open test data file
  f = fopen("relatively-big-data.csv", "r");
  if (f==NULL) {
    printf("Massive test file not found!\n");
    exit(1);
  }

  // Add 1,000,000 key-value pairs and time it
  while((c = fgetc(f)) != EOF) {
    if (c == '\n') {
      key = strtok(line, ",");
      value = strtok(NULL, ",");
      add_danhash(dandict, key, value);
      memset(line, 0, 100);
      i = 0;
    }else{
      line[i] = c;
      i++;
    }
  }
  puts("Successfully Added 1,000,000 records");

  free(f);

  // Get an entry we know exists
  char * s = get_danhash(dandict, "imoce@fehmomhow.net");
  assert(s != NULL);
  char * offer = strdup(s);
  free(s);

  puts("Successfully retrieved a value");

  // Add duplicate entry
  add_danhash(dandict, "ziwhub@ba.net", "new value");

  // Ensure duplicate has replaced value
  s = get_danhash(dandict, "ziwhub@ba.net");
  assert(s != NULL);
  assert(strcmp(s, "new value")==0);
  free(s);

  puts("Successfully added duplicate");

  // Remove that entry
  rem_danhash(dandict, "ziwhub@ba.net");
 
  // Try getting that entry again
  s = get_danhash(dandict, "ziwhub@ba.net");
  assert(s == NULL);
  free(s);

  puts("Successfully removed a value");

  puts(offer);
  free(offer);

  struct Entry * entry, * remove;
  for (i = 0; i < (dandict->size); i++) {
    entry = *(dandict->table+i);
    while(entry != NULL) {
      remove = entry;
      entry = entry->next;
      rem_danhash(dandict, remove->key);
    }
  }

  free(dandict->table);
  free(dandict);

}
