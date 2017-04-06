struct Entry {
  char * key;
  char * value;
  struct Entry * next; 
};

struct Dictionary {
  unsigned int size;
  unsigned int elements;
  struct Entry **table;
};

struct Dictionary * init_danhash(unsigned int size);
int add_danhash(struct Dictionary * dict, char * key, char * value);
int rem_danhash(struct Dictionary * dict, char * key);
struct Entry * get_danhash(struct Dictionary * dict, char * key);
