#include <stdint.h>

struct Entry {
  char * key;
  char * value;
  struct Entry * next; 
};

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
struct Entry * iterate_danhash(struct Dictionary * dict);
