#include <stdint.h>

/*  An Entry in dictionary

*/
struct Entry {
  char * key;
  char * value;
  struct Entry * next; 
};

/*  A dictionary containing a table of Entries

*/
struct Dictionary {
  uint32_t size;
  uint32_t elements;
  uint32_t (*hash_function)(const char * key);
  struct Entry **table;
};

struct Dictionary * init_danhash(uint32_t size, uint32_t (*hash_function)(const char * key));
void add_danhash(struct Dictionary * dict, const char * key, const char * value);
int rem_danhash(struct Dictionary * dict, const char * key);
char * get_danhash(struct Dictionary * dict, const char * key);
void print_danhash(struct Dictionary * dict);
void destroy_danhash(struct Dictionary * dict);
