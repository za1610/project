#include <stdio.h>
#include <stdlib.h>

//#define DEBUG

//#ifdef DEBUG
//#define SIZE 11
//#else
#define SIZE 1000
//#endif

#define FLOATTYPE float


FLOATTYPE A[SIZE];
FLOATTYPE B[SIZE];

float rand_FloatRange(float a, float b)
{
	return ((b-a)*((float)rand()/RAND_MAX))+a;
}

void fillRand(FLOATTYPE A[], int start, int end, FLOATTYPE from, FLOATTYPE to)
{
	srand(123);	// re-seed so we get the same numbers every time
	for (int i=start; i<end; ++i)
	{
		A[i] = (FLOATTYPE)rand_FloatRange(from, to);
	}
}
int compare(const void *av, const void *bv) { float af = *((float *)av); float bf = *((float*)bv); /*printf("%g>%g?\n",af,bf);*/return (af > bf); }
void fillRandSorted(FLOATTYPE A[], int start, int end, FLOATTYPE from, FLOATTYPE to)
{
	fillRand(A, start, end, from, to);
//	qsort(&(A[start]), end-start, sizeof(FLOATTYPE), &compare);
	qsort(A, SIZE, sizeof(FLOATTYPE), &compare);
}

void fillOnes(FLOATTYPE A[], int start, int end, FLOATTYPE from, FLOATTYPE to)
{
	for (int i=start; i<end; ++i)
	{
		A[i] = 1.0f;
	}
}

float fsumUp(FLOATTYPE A[], int size)
{
	float r = 0.0f;
	for (int i=0; i<size; ++i) {
		r += A[i];
	}
	return r;
}

float fsumDown(FLOATTYPE A[], int size)
{
	float r = 0.0f;
	for (int i=size-1; i>=0; --i) {
		r += A[i];
	}
	return r;
}

double dsumUp(FLOATTYPE A[], int size)
{
	double r = 0.0f;
	for (int i=0; i<size; ++i) {
		r += A[i];
	}
	return r;
}

double dsumDown(FLOATTYPE A[], int size)
{
	double r = 0.0f;
	for (int i=size-1; i>=0; --i) {
		r += A[i];
	}
	return r;
}

float kahan_summationUp(FLOATTYPE A[], int size) {
	float result = 0.f;
	
	float c = 0.f;
	for(int i=0;i<size; ++i) {
		float y = A[i] - c;
		float t = result + y;
		c = (t - result) - y;
		result = t;
	}
	return result;
}
float kahan_summationDown(FLOATTYPE A[], int size) {
	float result = 0.f;
	
	float c = 0.f;
	for(int i=size-1;i>=0; --i) {
		float y = A[i] - c;
		float t = result + y;
		c = (t - result) - y;
		result = t;
	}
	return result;
}
float fsumRecursive(FLOATTYPE A[], int size) {
	if (size==0) return 0.0f;
	if (size==1) return A[0];
	else {
		float r1 = fsumRecursive(A, size/2);
		float r2 = fsumRecursive(&(A[size/2]), size/2+size%2);
		return r1+r2;
	}
}


void DoTheSummation(FLOATTYPE A[], int size)
{
/*	float fr1 = fsumUp(A, size);
	float fr2 = fsumDown(A, size);
	float dr1 = dsumUp(A, size);
	float dr2 = dsumDown(A, size);
	float kr1 = kahan_summationUp(A, size);
	float kr2 = kahan_summationDown(A, size);
	float rr = fsumRecursive(A, size);
	printf("Float Up: %g  Down: %g  Difference %g; Double Up: %g  Down: %g  Difference %g; Kahan Up: %g  Down: %g  Difference %g; Recursive: %f Difference %g\n", 
		   fr1, fr2, fr2-fr1, dr1, dr2, dr2-dr1, kr1, kr2, kr2-kr1, rr, rr-dr1);
*/

	float fr1 = fsumUp(A, size);
	//float ffr1 = fsumUp(A, size);
	float fr2 = fsumDown(A, size);
//	float ffr2 = fsumDown(A, size);
	
	//double dr1 = dsumUp(A, size);
	//double dr2 = dsumDown(A, size);
	//float kr1 = kahan_summationUp(A, size);
	//float kr2 = kahan_summationDown(A, size);	
	//float rr = fsumRecursive(A, size);
	//printf("Float Up: %g  Down: %g  Difference %g; Double Up: %g  Down: %g  Difference %g; Kahan Up: %g  Down: %g  Difference %g; Recursive: %f Difference %g\n", 
	//	   fr1, fr2, fr2-fr1, dr1, dr2, dr2-dr1, kr1, kr2, kr2-kr1, rr, rr-dr1);




}

void printItForDebug(FLOATTYPE A[], int size)
{
#ifdef DEBUG
	for(int i=0;i<size; ++i) {
		printf("%g\n", A[i]);
	}
#endif
}

int main (int argc, const char * argv[]) {
/*	
	printf("Ones:      ");
	fillOnes(A, 0, SIZE, -1.0f, 1.0f);
	DoTheSummation(A, SIZE);
	printItForDebug(A, SIZE);
*/	
/*	printf("Rand:      ");
	fillRand(A, 0, SIZE, -1.0f, 1.0f);
	DoTheSummation(A, SIZE);
	printItForDebug(A, SIZE);
*/
	printf("Rand B:      ");
	fillRand(B, 0, SIZE/2, 0.0f, 10.0f);
	fillRand(B, SIZE/2, SIZE, 1000.0f, 1000000.0f);
	DoTheSummation(B, SIZE);
	printItForDebug(B, SIZE);



/*	
	printf("RandSorted:");
	fillRandSorted(A, 0, SIZE, -1.0f, 1.0f);
	DoTheSummation(A, SIZE);
	printItForDebug(A, SIZE);
*/
    return 0;
}





