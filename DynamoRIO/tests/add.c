#include <stdio.h>

   float Add1(float i, float j){ return i+j;}
 /*  void Add1Again(float i, float *j){ *j = i + *j;  }
*/
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
//				float b = 0;
//				b++;
     //  Add1Again(p, &z);
     //  printf("p = %f\t\tz = %f\n",p,z);
   }
/*
int main(){

float a = 2.854;
float b = 1.5;
float c = a/b;
float d = sinf(2);
printf("Result is %f %f \n",c, d);

return 0;
}*/
