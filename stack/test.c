#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"

int main() {

  struct Stack * rev_stack = init_stack();
  struct Stack * nest_stack = init_stack();
  char * sample = "Now is the winter of our discontent \
Made glorious summer (by this sun of York); \
And all the clouds that lour'd upon our house  \
In the deep bosom of the ocean <buried>. \
Now are our brows bound with victorious wreaths; \
Our bruised arms hung up for monuments;\
Our stern alarums changed to merry meetings,\
Our dreadful marches to delightful measures.\
Grim-visaged war hath smooth'd his wrinkled front;  \
And now, instead of mounting barded steeds \
To fright the souls of fearful adversaries, \
He capers nimbly in a lady's chamber \
To the lascivious pleasing of a lute. \
But I, that am not shaped for sportive tricks,\
Nor made to court an amorous looking-glass; \
I, [that am [rudely] stamp'd], and] want love's majesty  \
To strut before a wanton ambling nymph;\
I, that am curtail'd of this fair proportion, \
Cheated of feature by dissembling nature, \
Deformed, (unfinish'd, [sent before my time \
Into this <breathing> world], scarce half made up), \
And that so lamely and unfashionable \
That dogs bark at me as I halt by them;\
Why, I, in this weak piping time of peace,  \
Have no delight to pass away the time,  \
Unless to spy my shadow in the sun \
And descant on mine own deformity: \
And therefore, since I cannot prove a lover,  \
To entertain these fair well-spoken days, \
I am determined to prove a villain \
And hate the idle pleasures of these days."; 
  char c,p;
  int i = 0;

  // check for malformation
  while ((c=*(sample+i)) != '\0') {
    // also push full text onto other stack for reversal
    push(rev_stack, c);
    
    if (c == '(' || c == '[' || c == '<') {
      push(nest_stack, c);
    }
    if (c == ')' || c == ']' || c =='>') {
      p = pop(nest_stack);
      if (c==')' && p!='(') puts("Malformed!");
      if (c==']' && p!='[') puts("Malformed!");
      if (c=='>' && p!='<') puts("Malformed!");
	}
    i++;
  }

  if (pop(nest_stack)!=-1) puts("Malformed!");

  // Reverse string
  while ((c = pop(rev_stack)) != -1) {
    putchar(c);
  }
  putchar('\n');

  destroy_stack(rev_stack);
  destroy_stack(nest_stack);
}
