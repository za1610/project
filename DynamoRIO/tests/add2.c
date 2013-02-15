#include <stdio.h>

   void main()
   {

float e = 0.00000006f;
float x = 0.5f;
float y = 1.0f + x;
float more = y + e;
float diff_e = more - y;
printf("diff_e %.10f\n", diff_e);
float diff_o = diff_e - e;
printf("diff_o %.10f\n", diff_o);
float zero = diff_o + diff_o;
printf("zero %.10f\n", zero);
float result = 2 * zero;
printf("result %.10f\n", result);
/*   float p = 10.123;
   float y = 0.123;
   float z = p+y;
		int bz = *(int*)&z;
		printf("float %f %x\n", z, bz);
	 double dp = 10.123;
   double dy = 0.123;
   double dz = dp+dy;
	long int b = *(long int*)&dz;
		printf("double %lf %lx\n", dz, b);
		double tmp = (double)z;
		long int tmpb = *(long int*)&tmp;
		printf("double %lf %lx\n", tmp, tmpb);
		double r = dz - z;
		printf("result is %lf\n", r);
*/
   }

