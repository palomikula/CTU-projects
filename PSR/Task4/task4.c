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

SEM_ID mutex;
int delay1 = 20;
int delay2 = 50;
int delay3 = 1;
int worst_time=0;


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

void do_work_for_x_milliseconds(int x)
{
    struct timespec begin, end, result;
    int milliseconds = 0;
    clock_gettime(CLOCK_MONOTONIC,&begin);
    while (milliseconds < x)
    {
        clock_gettime(CLOCK_MONOTONIC,&end);
        timespec_subtract(&result, &end, &begin);
        milliseconds = result.tv_sec*1000 + result.tv_nsec/1000000;
    }
}

void high(){
	int i;
	while(1) {
	  struct timespec tstart, tend, result;
	  clock_gettime(CLOCK_MONOTONIC, &tstart);
	  semTake(mutex, WAIT_FOREVER);
	  clock_gettime(CLOCK_MONOTONIC, &tend);
	  semGive(mutex);
	  timespec_subtract(&result, &tend, &tstart);
	  taskDelay(delay1); /* let other tasks run */

	  if(worst_time < result.tv_nsec) {
		  worst_time=result.tv_nsec;
	  }
	  printf("worst time: %d\n",worst_time);
	}
}


void medium(){
	while (1) {
	  do_work_for_x_milliseconds(500);
	  taskDelay(delay3); /* wait to let the low priority task run */
	}
}

void low(){
	while (1) {
	  semTake(mutex, WAIT_FOREVER);
	  do_work_for_x_milliseconds(10);
	  semGive(mutex);
	  taskDelay(delay2); /* this delay can be even zero - do you know why? */
	}
}

int main(){

	int id1, id2, id3;

	mutex = semMCreate(SEM_Q_PRIORITY);
	//mutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
	sysClkRateSet(1000);
    id1=taskSpawn("High", 100, 0, 4096, (FUNCPTR) high, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	id2=taskSpawn("Medium", 110, 0, 4096, (FUNCPTR) medium, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    id3=taskSpawn("Low", 120, 0, 4096, (FUNCPTR) low, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	return 0;
}
