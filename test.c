#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "danhash.h"

int main()
{

  char c;
  char line[100];
  int i = 0;
  FILE * f;
   
  // Start the dictionary at an arbitrary size
  struct Dictionary * dandict = init_danhash(50000, NULL);
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
  fclose(f);
  puts("Successfully Added 1,000,000 records");

  // Get an entry we know exists
  char * s = get_danhash(dandict, "imoce@fehmomhow.net");
  assert(s != NULL);
  char * offer = malloc(sizeof(char) * strlen(s) + 1);
  strncpy(offer, s, strlen(s) + 1);
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
  int removed = rem_danhash(dandict, "ziwhub@ba.net");
  assert(removed==0); 

  // Try getting that entry again
  s = get_danhash(dandict, "ziwhub@ba.net");
  assert(s == NULL);
  puts("Successfully removed a value");

  // Try removing a value we know doesn't exist
  removed = rem_danhash(dandict, "Macavity");
  assert(removed==-1);

  // Make offer
  puts(offer);
  free(offer);

  destroy_danhash(dandict);
}
