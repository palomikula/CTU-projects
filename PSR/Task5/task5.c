#include <taskLib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <semLib.h>
#include <fcntl.h>
#include <time.h>

#define GAP 0
//#define MAX_ARRAY_SIZE (1024*1024/sizeof(struct elem))

typedef struct elem{
  struct elem *next;
  int gap[GAP];
}elem_t;

int timespec_subtract (struct timespec *result, struct timespec *x, struct timespec *y)
{
  /* Perform the carry for the later subtraction by updating Y. */
  if (x->tv_nsec < y->tv_nsec) {
    int num_sec = (y->tv_nsec - x->tv_nsec) / 1000000000 + 1;
    y->tv_nsec -= 1000000000 * num_sec;
    y->tv_sec += num_sec;
  }
  if (x->tv_nsec - y->tv_nsec > 1000000000) {
    int num_sec = (x->tv_nsec - y->tv_nsec) / 1000000000;
    y->tv_nsec += 1000000000 * num_sec;
    y->tv_sec -= num_sec;
  }

  /* Compute the time remaining to wait.
     `tv_nsec' is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_nsec = x->tv_nsec - y->tv_nsec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

void shuffle(elem_t *array, int n) {
    srand((unsigned)time(NULL));
    int i;
    for (i = 0; i < n - 1; i++) {
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
        elem_t tmp;
        tmp.next = array[j].next;
        array[j].next = array[i].next;
        array[i].next = tmp.next;
    }
}

int main(){
	//printf("hello");
	elem_t* arr;
	unsigned int size = 1024, nElem, i,j;
	struct timespec start_s,end_s,time_s,start_r,end_r,time_r;
	sysClkRateSet(1000);
	for(j=1;j<=1024;j*=2){
		arr = (elem_t*)malloc(j*size);
		
		nElem = j*size/sizeof(elem_t);
		for (i=0; i < nElem - 1; i++) {
			arr[i].next = &arr[i+1];
		}
		arr[i].next = &arr[0];
		
		int i = 1000000;
		
		elem_t *p = &arr[0];
		clock_gettime(CLOCK_MONOTONIC,&start_s);
		while (--i) p = p->next;
		clock_gettime(CLOCK_MONOTONIC,&end_s);
		timespec_subtract(&time_s,&end_s,&start_s);
		
		shuffle(arr,nElem);
		
		i = 1000000;
		p = &arr[0];
		clock_gettime(CLOCK_MONOTONIC,&start_r);
		while (--i) p = p->next;
		clock_gettime(CLOCK_MONOTONIC,&end_r);
		timespec_subtract(&time_r,&end_r,&start_r);
		
		for (i=0; i < nElem - 1; i++) {
					arr[i].next = &arr[i+1];
				}
				arr[i].next = &arr[0];
				
				i = 1000000;
				
				p = &arr[0];
				clock_gettime(CLOCK_MONOTONIC,&start_s);
				while (--i) p = p->next;
				clock_gettime(CLOCK_MONOTONIC,&end_s);
				timespec_subtract(&time_s,&end_s,&start_s);
				
		
		
		printf("%d	%f	%f\n",j*1024,(float)time_s.tv_nsec/1000000,(float)time_r.tv_nsec/1000000);
		
		free(arr);
	}
	return 0;
}
