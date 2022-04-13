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
   //printf("exponent = ceil(%lf/%lf) = %lf\n",log(fabs(arg)),log(2), ceil( log(fabs(arg))/log(2) ));
   exponent = ceil( log(fabs(arg))/log(2) ); //base 2 logarithm
   //printf("mantissa = %lf*2^(7-%d)\n\n",arg, exponent);
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

int main() {
	double da[100];
	// Pas på her, dunno hvad det her gør! <<<<<<<<<<
	myfloat_type* mda = calloc(100, sizeof(myfloat_type));
   	// Pas på her, dunno hvad det her gør! <<<<<<<<<<

	srand (time(NULL));
	for (int i=0; i<100; i++){
		da[i] = randfrom(-5.000, 5.000);
		doub2mydouble(da[i], (mda+i));
	}
	
	double mdaConvert[100];
	for (int i=0; i<100; i++){
		mdaConvert[i] = myfloat2double(mda+i);
	}
	// Udregn mean af da og mdaConvert
	double res_da; 
	double res_mda;
	for (int i = 0; i<100; i++){
		res_da += da[i];
		res_mda += mdaConvert[i];
	}
	res_da = res_da/100;
	res_mda = res_mda/100;

	double relError;

	relError = fabs(100-(res_mda/res_da)*100);

	printf("Task 3\n");
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n");
	/*for (int i=0; i<100; i++){
		/printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% #%d\n\n",i);
		printf("  mda.exponent: %d \n", (mda+i)->exponent);
		printf("  mda.mantissa: %d \n", (mda+i)->mantissa);
		printf("            da: %lf \n", da[i]);
		printf("    mdaconvert: %lf\n\n", mdaConvert[i]);
	
	}
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n");*/
	
	printf("result of da[0]+da[1]+........+da[n]/n                       : %lf\n", res_da);
	printf("result of mdaconvert[0]+mdaconvert[1]+........+mdaconvert[n] : %lf\n\n", res_mda);
	printf("Relative error is: %lf %%\n\n", relError);
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n");
	

	////
	double da2[100];
	for (int i=0; i<100; i++){
		//printf("Iteration: %d:\n", i);
		//printf("Før pow: %lf \n", da[i]);
		da2[i] = pow(da[i],2);
		//printf("Efter pow: %lf \n\n", da2[i]);
	}

	/////

	myfloat_type* mda2 = calloc(100, sizeof(myfloat_type));

	for (int i=0; i<100; i++){
		mult_float(mda+i,mda+i,mda2+i);
	}
	/////
	double mdaConvert2[100];
	for (int i=0; i<100; i++){
		mdaConvert2[i] = myfloat2double(mda2+i);
	}
	// Udregn mean af da og mdaConvert
	double res_da2; 
	double res_mda2;
	for (int i = 0; i<100; i++){
		res_da2 += da2[i];
		res_mda2 += mdaConvert2[i];
	}
	res_da2 = res_da2/100;
	res_mda2 = res_mda2/100;

	double relError2;

	relError2 = fabs(100-(res_mda2/res_da2)*100);
	printf("Task 6\n");
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n");
	/*for (int i=0; i<100; i++){
		/printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% #%d\n\n",i);
		printf("  mda2.exponent: %d \n", (mda2+i)->exponent);
		printf("  mda2.mantissa: %d \n", (mda2+i)->mantissa);
		printf("            da2: %lf \n", da2[i]);
		printf("    mda2convert: %lf\n\n", mda2Convert[i]);
	
	}
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n");*/
	
	printf("result of da2[0]+da2[1]+........+da2[n]/n                       : %lf\n", res_da2);
	printf("result of mda2convert[0]+mda2convert[1]+........+mda2convert[n] : %lf\n\n", res_mda2);
	printf("Relative error is: %lf %%\n\n", relError2);
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n");
	/////
	double * a = malloc(sizeof(double));
	struct timeval start, end;
	gettimeofday(&start, NULL);
	for (int i=0; i<100;i++){
		a = &da[i];
		printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% #%d\n\n",i);
		printf("da: %lf \n", da2[i]);
		printf(" a: %lf\n\n", *(double*)a);
	}
	gettimeofday(&end, NULL);
	double time_taken = end.tv_sec + end.tv_usec / 1e6 - start.tv_sec - start.tv_usec/1e6;
	printf("Execution time for a*da is: %lf\n", time_taken);
	///// Check execution time?

	return 0;
}