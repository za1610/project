#include <stdio.h>

// needed for math functions such as sqrt()

#include <math.h>

 

int main(void)

{

      // study this funny part, add 1 before and add 1 after...

      float i = 1, j;

      ++i; // i = i + 1

      j = ++i; // j = i = i + 1 - i already equal to 2

      j = i++; // assign i to j and then i = i + 1

      // using functions from standard library, math.h

			double d = 2.55;
			double k = 2.0;
			double c = d*d;
      printf("2 raised to the power of 3 is %f\n", pow(2, 3.3));
return 0;
}
