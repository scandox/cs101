struct List {
  int length;
  struct ListItem * next;
};

struct ListItem {
  char * item;
  struct ListItem * next;
};

struct List * init_list();
struct ListItem * add_item(struct List * list, char * item);
void rem_item(struct List * list, struct ListItem * list_item);
void destroy_list(struct List * list);
void print_list(struct List * list);
