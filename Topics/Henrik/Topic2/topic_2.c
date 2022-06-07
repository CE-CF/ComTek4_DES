#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

int i;
double a = 1.001;
int array_size = 10;




//low precision floating pt type
typedef struct myfloat{
        signed char mantissa;
        signed char exponent;
} myfloat_type;


//convert from double to low precision type = opg 2
void doub2mydouble(double arg, myfloat_type *res){
   int exponent;
   double temp;
   exponent = ceil( log(abs(arg))/log(2) ); //base 2 logarithm
   temp=arg*pow(2,7-exponent);
   printf("%f\n", temp);
   printf("%d\n", exponent);
   res->mantissa = (signed char)temp;
   res->exponent = exponent-7;
}

//convert from low precision type to double = opg 3
double myfloat2double(myfloat_type *arg1){
    double res = (double)(arg1->mantissa) * pow(2,arg1->exponent);
    return res;
}

//multiply to low precision types
void mult_float(myfloat_type *arg1,myfloat_type *arg2,myfloat_type *result){
    int temp;
    unsigned char sign;
  
    sign=0x80 & ((unsigned char)arg1-> mantissa ^ (unsigned char)arg2-> mantissa); //find sign of result 

    char i=0;
    temp = (int)(arg1-> mantissa) * (int)(arg2-> mantissa);
   
    temp = temp & 0x7f00; //take away sign from product (0x7F00 = 32512)
  
    while(abs(temp)>128){
        i++;
        temp=temp>>1;
    }
   
    result->mantissa = (unsigned char) temp;
   
    result->mantissa = result->mantissa | sign; //add recorded sign
   
    result->exponent = arg1->exponent + arg2->exponent + i;
   
}

/* generate a random floating point number from min to max */
double randfrom(double min, double max) {
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}


int main(){
    time_t t1;
    srand((unsigned)time(&t1)); // Seed for random generate
    
    //       #### OPG 1 ####
    // Generate array with random doubles
    printf("TASK 1:\n");
    double da[array_size];  
    for(i=0;i<array_size;i++){
        da[i] = randfrom(-5,5);
        printf("%lf\n",da[i]);                          // OPG 1 print
    }
  
    //      #### OPG 2 ####
    // Insert da array in the mda array
    printf("\nTASK 2:\n");
    myfloat_type * struct_pointer;
    struct_pointer = calloc(array_size, sizeof(myfloat_type));
    for(i=0;i<array_size;i++){
        printf("%lf\n",da[i]);                         // OPG 2 print
        doub2mydouble(da[i], (struct_pointer+i));
    }
    // Print opg 2
   /* for(i=0;i<array_size;i++){
        printf("%c\n",(struct_pointer+i)->exponent);
        printf("%c\n",(struct_pointer+i)->mantissa);
    }
*/
    //     #### OPG 3 ####
    printf("\nTASK 3:\n");
    double mda_double[array_size];
    for(i=0;i<array_size;i++){
        mda_double[i] = myfloat2double((struct_pointer+i));
        //printf("%f\n", mda_double[i]);
        //printf("%d\n",i);
    }
    /*Compute the mean relative error between da and mda*/
    double mean_da=0; 
    double mean_mda=0;
    for(i=0;i<array_size;i++){
        mean_da += da[i];
        mean_mda += mda_double[i];
        //printf("mean_da %f - mean_mda %lf\n", da[i], mda_double[i]);
        }
    mean_da = mean_da/array_size;     // Udregner gennemsint på alle tal i array'et.
    mean_mda = mean_mda/array_size;     // Udregner gennemsint på alle tal i array'et.
    printf("Mean relative error: %lf percent\n", ((mean_mda/mean_da)*100));

    //    #### OPG 4 og 5 ####
    // Compute da2=da^2 and compute mda2=mda^2
    double da2[array_size];
    double mda2_double[array_size];

    printf("\nTASK 4 and 5:\n");
    for(i=0;i<array_size;i++){
        da2[i] = pow(da[i],2);
        //printf("%f\n",da2[i]);
        mda2_double[i] = pow(mda_double[i],2);
        //printf("%f\n",mda2[i]);
    }

    // #### OPG 6 ####
    // Compute the mean relative error between da2 and mda2
    double mean_da2=0; 
    double mean_mda2=0;

    printf("\nTASK 6:\n");
    for(i=0;i<array_size;i++){
        mean_da2 += da2[i];
        mean_mda2 += mda2_double[i];
        
    }
    //printf("%f\n",mean_da2);
    //printf("%f\n",mean_da2);
    mean_da2 = mean_da/array_size;     // Udregner gennemsint på alle tal i array'et.
    mean_mda2 = mean_mda/array_size;     // Udregner gennemsint på alle tal i array'et.
    printf("Mean^2 relative error: %lf percent\n", ((mean_mda/mean_da)*100));

    //   #### OPG 7 ####
    //Iterate "a*=da[i];" over the length of da and measure the execution time (micros())
    printf("\nTASK 7:\n");

    clock_t time_7;
    time_7 = clock(); // time start
    for(i=0;i<array_size;i++){
        a *= da[i];
    }
    time_7 = clock() - time_7; // time end

    double time_taken_7 = ((double)time_7)/CLOCKS_PER_SEC; // in seconds
    printf("Time (seconds) for task 7: %f\n", time_taken_7);

    //  #### OPG 8 ####
    // convert "a" into "myfloat_type f1
    printf("\nTASK 8:\n");

    // create f1 structure 
    myfloat_type * f1;
    f1 = calloc(1, sizeof(myfloat_type));
    doub2mydouble(a, (f1));

    
    //   #### OPG 9 ####
    // Iterate "mult_float(&f1,&mda[i],&f); memcpy(&f1,&f,2);" over the length of mda and measure the execution time

    // create result structure 
    myfloat_type * f;
    f = calloc(1, sizeof(myfloat_type));

    printf("\nTASK 9:\n");

    clock_t time_9;
    time_9 = clock(); // time start
    for(i=0;i<array_size;i++){
        mult_float(f1,(struct_pointer+i),f);    
    }
    time_9 = clock() - time_9; // time end

    double time_taken_9 = ((double)time_9)/CLOCKS_PER_SEC; // in seconds
    printf("Time (seconds) for task 9: %f\n", time_taken_9);


    //   #### OPG 10 ####
    // Compute the relative difference between results from (7) and (9)





    //free(f);
    //free(f1);
    free(struct_pointer);

    return 0;
}

