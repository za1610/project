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
	//float s = 0.0f;
	for (int i=0; i<size; ++i) {
		r += A[i];
	//	s += A[i];
	}
	return r;
}

float fsumUp2(FLOATTYPE A[], int size)
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

float fsumDown2(FLOATTYPE A[], int size)
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

float fsumRecursive2(FLOATTYPE A[], int size) {
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

	float fr1 = fsumUp(A, size);
        float ffr1 = fsumUp2(A, size);
	float fr2 = fsumDown(A, size);
        float ffr2 = fsumDown2(A, size);
//float a = 9 + fr1;	
	double dr1 = dsumUp(A, size);
	double dr2 = dsumDown(A, size);
	float kr1 = kahan_summationUp(A, size);
	float kr2 = kahan_summationDown(A, size);	
	float rr = fsumRecursive(A, size);
//	float frr = fsumRecursive2(A, size);

//	printf("Float Up: %.13f\n  Down: %.13f\n  Difference %.13f  Diff double UP %.13lf Diff double DOWN %.13lf\n; Double Up: %.13lf\n  Down: %.13lf\n  Difference %.13lf\n; Kahan Up: %.13f\n  Down: %.13f\n  Difference %.13f Diff double Kahan %.13lf\n; Recursive: %.13f\n Difference %.13lf\n", 
//		   fr1, fr2, fr2-fr1, fr1-dr1, fr2 - dr1,dr1, dr2, dr2-dr1, kr1, kr2, kr2-kr1, kr1 - dr1,  rr, rr-dr1);


printf("Result: Float Up: %.13f\n", fr1);
//printf("Result 2: Float Up: %.13f\n", ffr1);

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
//float fffr1 = fsumUp(A, SIZE);
	printf("Ones:      \n");
	fillOnes(A, 0, SIZE, -1.0f, 1.0f);
	DoTheSummation(A, SIZE);
*/


	printf("Rand:      \n");
	fillRand(A, 0, SIZE, 0.0f, 1000.0f);
	DoTheSummation(A, SIZE);
	printItForDebug(A, SIZE);


/*
	printf("RandSorted:\n");
	fillRandSorted(A, 0, SIZE, -1.0f, 1.0f);
	DoTheSummation(A, SIZE);
	printItForDebug(A, SIZE);
*/
    return 0;
}





