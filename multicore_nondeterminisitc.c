/* 2018-05-10
 * 
 * Compile with:
 * gcc -o multicore_nondeterminisitc -fopenmp multicore_nondeterminisitc.c -lm
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



#include <stdio.h>
#include <math.h>
#include <omp.h>



int Nsum = 1000;
int Nruns = 6;


/* Calculates the sum mentiones in the comment above.
 * */
double calc_sum()
{
	int i;
	double sum = 0.;
	
	#pragma omp parallel reduction (+ : sum)
	for(i=-Nsum/2; i<=Nsum/2; i++){
        sum += (+1) * i/2 * 1e-1;
	}

	return sum;
}


/* Calculates the mean and standard deviaton 
 * for the values of an array x with length N.
 * 
 * @param N			input, number of elements in array
 * @param x			input, pointer of first element of array
 * @param *mean		output, mean value of x
 * @param *stddev	output, standard deviation of x
 * */
double calc_mean_and_stddev(int N, double* x, double* mean, double* stddev)
{
    int i;
    *mean = 0.;
    *stddev = 0.;
    
    // calculate mean
    for(i=0; i<N; i++){
        *mean += x[i];
    }
    *mean = *mean/N;
    
    // calculate standard deviation
    for(i=0; i<N; i++){
        *stddev += (x[i]-*mean)*(x[i]-*mean);
    }
    *stddev = sqrt(*stddev/(N-1));
}


// main method
int main()
{
    int i;
    double value;
    double values[Nruns];
    double mean, stddev;
    
    // print number of threads first
    #pragma omp parallel
	{
		#pragma omp single
		printf("Number of threads:\t %d\n\n", omp_get_num_threads());
	}
    
    for(i=0; i<Nruns; i++){
       value = calc_sum();
       values[i] = value;
       printf("Result for run: %2d:\t % -6.3e\n", i+1, value);
    }
    
    calc_mean_and_stddev(Nruns, values, &mean, &stddev);
    
    printf("\nMean:\t\t\t % -6.3e\nStandard deviation:\t % -6.3e\n", mean, stddev);
    
	return 0;
}
