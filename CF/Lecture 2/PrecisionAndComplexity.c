#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

/*
  Low level precision floating point lib
  Author: Henrik Schiøler
*/

//low precision floating pt type
typedef struct myfloat
{
        signed char mantissa;
        signed char exponent;
} myfloat_type;


//convert from double to low precision type
void doub2mydouble(double arg, myfloat_type *res)
{
   int exponent;
   double temp;
   exponent = ceil( log(fabs(arg))/log(2) );
   temp=arg*pow(2,7-exponent);
   res->mantissa = (signed char)temp;
   res->exponent = (exponent-7);
}

//convert from low precision type to double
double myfloat2double(myfloat_type *arg1)
{
	// printf("mantissa is: %d\nexponent is: %d\n\n", (arg1->mantissa), (arg1->exponent));
  	double res = (double)(arg1->mantissa) * pow(2,arg1->exponent);
   	return res;
}

//multiply to low precision types
void mult_float(myfloat_type *arg1,myfloat_type *arg2,myfloat_type *result)
{
  int temp;
  unsigned char sign;
  
  sign=0x80 & ((unsigned char)arg1-> mantissa ^ (unsigned char)arg2-> mantissa); //find sign of result 
  
  char i=0;
   temp = (int)(arg1-> mantissa) * (int)(arg2-> mantissa);
   
   temp = temp & 0x7f00; //take away sign from product
  
   while(abs(temp)>128)
   {
      i++;
      temp=temp>>1;
   }
   
   result->mantissa = (unsigned char) temp;
   
   result->mantissa = result->mantissa | sign; //add recorded sign
   
   result->exponent = arg1->exponent + arg2->exponent + i;
   
}

/* generate a random floating point number from min to max */
double randfrom(double min, double max) 
{

    double range = (max - min); 
    double div = RAND_MAX / range;
    /*printf("Devisor is: %lf\n",div);
    printf("Result is -5 + %lf\n\n",(rand() / div));*/
    return min + (rand() / div);
}
void task1(double *array, double min, double max, int size)
{
	printf("size of array is %d\n", size);
	srand (time(NULL));
	for (int i=0; i<size; i++){
		array[i] = randfrom(min, max);
	}
}
void task2(double *double_array, myfloat_type *myfloat_type_array, int size)
{
	for (int i=0; i<size; i++, myfloat_type_array++){
		doub2mydouble(double_array[i], (myfloat_type_array));
	}
}
void task3and6(double *double_array1, double *double_array2, myfloat_type *myfloat_type_array, double *result, int size)
{
	double res_da; 
	double res_mda;
	for (int i=0; i<size; i++, myfloat_type_array++){
		double_array1[i] = myfloat2double(myfloat_type_array);
		res_da += double_array2[i];
		res_mda += double_array1[i];
	}
	
	res_da = res_da/size;
	res_mda = res_mda/size;

	*result = fabs(100-(res_mda/res_da)*100);
}
void task4and5(double *double_array1, double *double_array2, myfloat_type *myfloat_type_array1, myfloat_type *myfloat_type_array2, int size)
{
	for (int i=0; i<size; i++, myfloat_type_array1++, myfloat_type_array2++){
		double_array1[i] = pow(double_array2[i],2);
		mult_float(myfloat_type_array2, myfloat_type_array2, myfloat_type_array1);
	}
}

void task7(double *double_array, struct timeval, double *value, double *result, int size)
{
	gettimeofday(&start, NULL);
	for (int i=0; i<size;i++){
		*value = double_array[i];
		printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% #%d\n\n",i);
		printf("da: %lf \n", double_array[i]);
		printf(" a: %lf\n\n", *value);
	}
	gettimeofday(&end, NULL);
	*result = end.tv_sec + end.tv_usec / 1e6 - start.tv_sec - start.tv_usec/1e6;
}
int main() {
	#define SIZE 100
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	printf("Task 1 & 2\n");
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n");
	double da[SIZE];
	myfloat_type* mda = calloc(SIZE, sizeof(myfloat_type));
	task1(da, -5.000, 5.000, SIZE);
	task2(da, mda, SIZE);
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	// Udregn mean af da og mdaConvert
	

	printf("Task 3\n");
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n");
	double mdaConvert[SIZE];	
	double relError;
	task3and6(mdaConvert, da, mda, &relError, SIZE);
	/*for (int i=0; i<SIZE; i++){
		/printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% #%d\n\n",i);
		printf("  mda.exponent: %d \n", (mda+i)->exponent);
		printf("  mda.mantissa: %d \n", (mda+i)->mantissa);
		printf("            da: %lf \n", da[i]);
		printf("    mdaconvert: %lf\n\n", mdaConvert[i]);
	
	}
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n");*/

	printf("Relative error is: %lf %%\n\n", relError);
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	printf("Task 4 & 5\n");
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n");
	double da2[SIZE];
	myfloat_type* mda2 = calloc(SIZE, sizeof(myfloat_type));
	task4and5(da2, da, mda2, mda, SIZE);
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	printf("Task 6\n");
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n");
	double mdaConvert2[SIZE];
	double relError2;
	task3and6(mdaConvert2, da2, mda2, &relError2, SIZE);
	/*for (int i=0; i<SIZE; i++){
		/printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% #%d\n\n",i);
		printf("  mda2.exponent: %d \n", (mda2+i)->exponent);
		printf("  mda2.mantissa: %d \n", (mda2+i)->mantissa);
		printf("            da2: %lf \n", da2[i]);
		printf("    mda2convert: %lf\n\n", mda2Convert[i]);
	
	}
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n");*/
	printf("Relative error is: %lf %%\n\n", relError2);
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	printf("Task 7\n");
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n");
	double a;
	struct timeval start, end;
	double time_taken;
	task7(da, timeval, &a, &time_taken, SIZE);
	
	printf("Execution time for a*da is: %lf\n", time_taken);
	///// Check execution time?

	return 0;
}