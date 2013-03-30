#include <stdio.h>

   float p = 10.123;
   float y = 0.123;
   float z = 0;

   void main()
   {
	int i;
	for(i = 0; i < 10; i++){
          p = p+y;
	}

       printf("p = %f\t\ty = %f\n",p,y);
   }

