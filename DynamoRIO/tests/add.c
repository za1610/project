#include <stdio.h>

   float Add1(float i){ return i+1;}
   void Add1Again(float i, float *j){ *j = i + *j;  }

   float p = 10.123443;
   float y = 0.1231;
   float z = 0;

   void main()
   {
       y = Add1(p);
       printf("p = %f\t\ty = %f\n",p,y);
       Add1Again(p, &z);
       printf("p = %f\t\tz = %f\n",p,z);
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
