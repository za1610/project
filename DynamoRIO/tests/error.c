#include <stdio.h>

int main()
{
   float x = -3.0;
//   double dx = -3.0;
   int n = -30;
   for ( n = -30; n < 1; n++){
	double	dx =  x;
//   printf("%.13f %.13f\n",x , 0.1*n);
	float x1 = 0.1;
	//printf("op1 is %.13f op2 is %.13f \n", x, x1);
	x = x + x1;
	double dx1 = x1;
	dx = dx + dx1;
	double res = dx - x;
   	printf("%.13f %.13lf %.13f  %.13lf\n",x,dx,res, 0.1*n);
	
	}
   return 0;
}
