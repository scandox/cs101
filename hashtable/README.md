# Basic Hashtable Library in C

```
make
./test
```

You ought to compile and run this. You may learn something to your advantage.

In terms of usage it follows a pretty basic API:

```
struct Dictionary * my_dict = init_danhash(500, NULL);

add_danhash(my_dict, "key", "value");
add_danhash(my_dict, "key1", "value1");

char * value = get_danhash(my_dict, "key");
if (value!=NULL) {
   puts(value);
   free(value);
}

destroy_danhash(mydict);
```

It also supports removal of individual items by key and can print out a textual representation of dictionary contents.  `test.c` shows various operations. Oh and you can plugin your own hash function too by passing a pointer to an appropriate function in the `init_danhash` function. 
