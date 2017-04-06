#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <time.h>
#include "danhash.h"

int main()
{

  char c;
  char line[100];
  int i = 0;
  FILE * f;
  
  // Start the dictionary at a too small size so we test the
  // hashtable expansion feature
  struct Dictionary * dandict = init_danhash(50000);
  char * key, * value;
  
  // Open test data file
  f = fopen("relatively-big-data.csv", "r");
  if (f==NULL) {
    printf("Massive test file not found!\n");
    exit(1);
  }

  // Add 1,000,000 key-value pairs
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

  // Get an entry we know exists
  struct Entry * s = get_danhash(dandict, "imoce@fehmomhow.net");
  assert(s != NULL);
  char * offer = strdup(s->value);
  free(s);

  puts("Successfully retrieved a value");

  // Remove that entry
  rem_danhash(dandict, "ziwhub@ba.net");
 
  // Try getting that entry again
  s = get_danhash(dandict, "ziwhub@ba.net");
  assert(s == NULL);

  puts("Successfully removed a value");

  puts(offer);
}
