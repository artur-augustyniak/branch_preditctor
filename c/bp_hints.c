#include <stdio.h>
#include <stdlib.h>

#define ARR_S 32768

/*
gcc -DSORT_O -DBN_TRUE -std=c99 -Wall -pedantic -O0 bp_hints.c && time ./a.out && perf stat ./a.out
root@bender:/home/aaugustyniak# cat /proc/sys/kernel/perf_event_paranoid 
3
root@bender:/home/aaugustyniak# echo 1 > /proc/sys/kernel/perf_event_paranoid 
root@bender:/home/aaugustyniak# cat /proc/sys/kernel/perf_event_paranoid 
1
*/

int comp (const void * elem1, const void * elem2) 
{
    int f = *((int*)elem1);
    int s = *((int*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

int main(void)
{	
	
	// half 128
	int expected_division = 64;
    
    int data[ARR_S];

    for (unsigned c = 0; c < ARR_S; ++c)
	{
        data[c] = rand() % 256;
		//printf("%i\n", data[c]);
	}
    // !!! With this, the next loop runs faster
#ifdef SORT
    qsort (data, sizeof(data)/sizeof(*data), sizeof(*data), comp);
#endif
    // Test
    long long sum = 0;

    for (unsigned i = 0; i < 10000; ++i)
    {
        // Primary loop
        for (unsigned c = 0; c < ARR_S; ++c)
        {
            
#ifdef BN_TRUE
            if (__builtin_expect (data[c] >= expected_division, 1))
#elif BN_FALSE
            if (__builtin_expect (data[c] >= expected_division, 0))
#else
            if (data[c] >= expected_division)
#endif
                sum += data[c];
        }
    }
    printf("sum: %lld\n", sum);
    

}
