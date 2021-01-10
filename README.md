# multicore_nondeterministic
This is a small C-program for Linux that shows the nondeterministic behavior in OpenMP multithread programs.

## Description

The program calculates the following sum:

    sum_{i=-Nsum/2}^{Nsum/2} i*1e-1
 
in multiple runs with multiple threads using OpenMP.

The sum from above is analytially 0. However the summands spread over 
several orders of magnitude and change their signs which makes the problem
numerically ill-conditioned.
Thus, the program will get different results due to 
rounding errors and the nondeterministic behavior of 
parallel computing.


## Compiltation
Compile with OpenMP and -lm flag:

    gcc -o multicore_nondeterminisitc -fopenmp multicore_nondeterminisitc.c -lm
 

## Execution and Results Interpretation:

### Run with 1 Thread

Run command:

    export OMP_NUM_THREADS=1 && ./multicore_nondeterminisitc

The result is different than 0 since 1e-1 does not have an 
exact representation in floating point arithmetic. 
However, the results do not (yet) differ 
for different program runs.

### Run with 2 Threads

Run command:  

    export OMP_NUM_THREADS=2 && ./multicore_nondeterminisitc

Now, the results differ heavily due to the nondeterministic 
nature of parallel computing (mainly due to the fact that 
the distribution of for loop chunks to the several 
threads/cores is random).


## Example Outputs

Example output for 1 thread:

    $ export OMP_NUM_THREADS=1 && ./multicore_nondeterminisitc  
    Number of threads:       1  
    
    Result for run:  1:      -2.451e-13  
    Result for run:  2:      -2.451e-13  
    Result for run:  3:      -2.451e-13  
    Result for run:  4:      -2.451e-13  
    Result for run:  5:      -2.451e-13  
    Result for run:  6:      -2.451e-13  
    
    Mean:                    -2.451e-13  
    Standard deviation:       0.000e+00  

One can see that the sum has a small numerical value different than 0, but for each run the results are reliable.


Example output for 2 threads:

    $ export OMP_NUM_THREADS=2 && ./multicore_nondeterminisitc   
    Number of threads:       2  

    Result for run:  1:      -2.522e+03  
    Result for run:  2:       3.925e+03  
    Result for run:  3:       1.929e+03  
    Result for run:  4:       2.500e+03  
    Result for run:  5:       3.187e+03  
    Result for run:  6:       4.463e+03  
    
    Mean:                     2.247e+03  
    Standard deviation:       2.511e+03  

Now, the situation has changed. The variation of the sum from 0 is much higher, and for each run one gets different result which is completely random.
