#include <stdio.h>

int main() {

  /* Loop over 1 .. 100. For multiples of 3 print Fizz, 5 print FizzBuzz and
     for multiples of both print FizzBuzz */
  for (int i = 1; i < 101; i++) {
     // Apparently better manners than % 15
     if (((i % 5) == 0 && (i % 3) ==0)) {
       printf("FizzBuzz\n");
     }else if ((i % 5) == 0) {
       printf("Buzz\n");
     }else if ((i % 3) == 0) {
       printf("Fizz\n");
     }else{
       printf("%d\n", i);
     }
  }

}
