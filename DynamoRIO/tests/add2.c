#include <stdio.h>
#include <math.h>

void addFloats(){
float e = 0.00000006f;
printf("e %.13f\n", e);
float x = 0.5f;
printf("x %.13f\n", x);
float y = 1.0f + x;
printf("y %.13f\n", y);
float more = y + e;
printf("more %.13f\n", more);
int i;
float diff_e;
for(i = 0; i < 10; i++){
	diff_e = more - y;
	more = more + e;
}
printf("diff_e %.13f\n", diff_e);
float diff_o;
//for(i = 0; i < 10; i++)
	diff_o = diff_e - e;
printf("diff_o %.13f\n", diff_o);
float zero = diff_o + diff_o;
printf("zero %.13f\n", zero);
float result = 2 * zero;
printf("result %.13f\n", result);

}
/*
void addDoubles(){
double de = 0.00000006f;
double dx = 0.5f;
double dy = 1.0f + dx;
double dmore = dy + de;
double ddiff_e = dmore - dy;
printf("diff_e %.13lf\n", ddiff_e);
double ddiff_o = ddiff_e - de;
printf("diff_o %.13lf\n", ddiff_o);
double dzero = ddiff_o + ddiff_o;
printf("zero %.13lf\n", dzero);
double dresult = 2 * dzero;
printf("result %.13lf\n", dresult);
 

}
*/
   void main()
   {
	addFloats();
//	addDoubles();
/*
float e = 0.00000006f;
printf("e %.13f\n", e);
float x = 0.5f;
printf("x %.13f\n", x);
float y = 1.0f + x;
printf("y %.13f\n", y);
float more = y + e;
printf("more %.13f\n", more);
float diff_e = more - y;
printf("diff_e %.13f\n", diff_e);
float diff_o = diff_e - e;
printf("diff_o %.13f\n", diff_o);
float zero = diff_o + diff_o;
printf("zero %.13f\n", zero);
float result = 2 * zero;
printf("result %.13f\n", result);
float op1 = diff_e;
float op2 = e;

		int exp1, exp2;
		float mant1, mant2;
		mant1 = frexpf(op1, &exp1);
		mant2 = frexpf(op2, &exp2);
		int bits = abs(exp1-exp2);
		printf("op1 %.13f mantissa %.13f exp %d\n", op1, mant1, exp1);
		printf("op2 %.13f mantissa %.13f exp %d\n", op2, mant2, exp2);

		int mask = 0;
		int ind = 0;
		for(ind= 0; ind < bits; ind++){
			mask = mask << 1;
			mask = mask | 1;
		}
		printf("mask value in hex %x\n", mask);
		
		unsigned int expmask = 0xFF800000;
		unsigned int totalmask = expmask | mask;
		printf("mask value in hex %x exp %x total %x\n", mask, expmask, totalmask);
		if(exp2 < exp1){
			int intop2 = *(int*)&op2;
			printf("op2 in hex %x\n", intop2);
			int bin = intop2 & totalmask;
			printf("lost in binary %x\n", bin);
			float lostbits = *(float*)&bin;
			printf("lost bits are %.13f\n", lostbits);
		}
		else{
			int intop2 = *(int*)&op1;
			printf("op1 in hex %x\n", intop2);
			int bin = intop2 & totalmask;
			printf("lost in binary %x\n", bin);
			float lostbits = *(float*)&bin;
			printf("lost bits are %f\n", lostbits);
		}


double de = 0.00000006f;
double dx = 0.5f;
double dy = 1.0f + dx;
double dmore = dy + de;
double ddiff_e = dmore - dy;
printf("diff_e %.13lf\n", ddiff_e);
double ddiff_o = ddiff_e - de;
printf("diff_o %.13lf\n", ddiff_o);
double dzero = ddiff_o + ddiff_o;
printf("zero %.13lf\n", dzero);
double dresult = 2 * dzero;
printf("result %.13lf\n", dresult);
 

double dr = dzero - zero;
printf("!!! %.13f\n", dr);
*/
   }

