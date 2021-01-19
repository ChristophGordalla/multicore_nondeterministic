/* 2018-05-10
 * 
 * Compile with:
 * gcc -o multicore_nondeterminisitc -fopenmp multicore_nondeterministic.c -lm
 * 
 * This is a small program for Linux that calculates the sum
 * 
 * sum_{i = -Nsum/2}^{Nsum/2} i*1e-1 
 * 
 * with multiple threads using OpenMP and in multiple runs. 
 * 
 * The sum from above is analytially 0. However the summands
 * spread over several orders of magnitude and change 
 * their signs which makes the problem numerically
 * ill-conditioned.
 * Thus, the program will get different results due to 
 * rounding errors and the nondeterministic behavior of 
 * parallel computing.
 * 
 * Run with:  
 * export OMP_NUM_THREADS=1 && ./multicore_nondeterminisitc
 * 
 * The result is different than 0 since 1e-1 does not have an 
 * exact representation in floating point arithmetic. 
 * However, the results do not (yet) differ 
 * for different program runs.
 * 
 * Run with:  
 * export OMP_NUM_THREADS=2 && ./multicore_nondeterminisitc
 *  
 * Now, the results differ heavily due to the nondeterministic 
 * nature of parallel computing (mainly due to the fact that 
 * the distribution of for loop chunks to the several 
 * threads/cores is random).
 * */


#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>



const uint16_t Nsum = 1000;
const uint8_t Nruns = 6;


/* Calculates the sum mentioned in the comment above.
 * */
double calc_sum()
{
	int16_t i;
	double sum = 0.;
	
	#pragma omp parallel reduction (+ : sum)
	for(i=-Nsum/2; i<=Nsum/2; ++i){
        sum += i * 1e-1;
	}

	return sum;
}


/* Calculates the mean and standard deviaton 
 * for the values of an array x with length N.
 * 
 * @param x		input, pointer of first element of array
 * @param mean		output, pointer to the mean value of x
 * @param stddev	output, pointer to the standard deviation of x
 * */
double calc_mean_and_stddev(const double *x, double *mean, double *stddev)
{
    uint8_t i;
    
    // calculate mean
    for(i=0; i<Nruns; ++i){
        *mean += x[i];
    }
    *mean = *mean/Nruns;
    
    // calculate standard deviation
    for(i=0; i<Nruns; ++i){
        *stddev += (x[i]-*mean)*(x[i]-*mean);
    }
    *stddev = sqrt(*stddev/(Nruns-1));
}


// main method
int main()
{
    uint8_t i;
    double mean = 0, stddev = 0;
    double values[Nruns];
    
    // print number of threads first
    #pragma omp parallel
	{
		#pragma omp single
		printf("Number of threads:\t %d\n\n", omp_get_num_threads());
	}
    
    for(i=0; i<Nruns; ++i){
       values[i] = calc_sum();
       printf("Result for run: %2d:\t % -6.3e\n", i+1, values[i]);
    }
    
    calc_mean_and_stddev(values, &mean, &stddev);
    
    printf("\nMean:\t\t\t % -6.3e\nStandard deviation:\t % -6.3e\n", mean, stddev);
    
	return 0;
}
