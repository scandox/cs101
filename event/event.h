struct Event {
  int handlers;
  char * name;
  struct EventHandler * head;
  struct EventHandler * last;
};

struct EventHandler {
  void (*callback)(void * data);
  struct EventHandler * next;
};

struct Event * init_event(char * name);
struct EventHandler * add_handler(struct Event * event, void (*callback)(void* data));
void remove_handler(struct Event * event, struct EventHandler * handler);
void destroy_event(struct Event * event);
void print_event(struct Event * event);
