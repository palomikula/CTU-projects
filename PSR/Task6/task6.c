#include <stdio.h>
#include <sysLib.h>
#include <intLib.h>
#include <lite5200b.h>
#include <arch/ppc/ppc5200.h>
#include <taskLib.h>
#include <iv.h>

SEM_ID sem;
int task_time;
int ISR_time;
int *task_times;
int *ISR_times;

int to_usec(int ticks){
	return ticks*30/1000;
}

/* Interrupt handler */
void slicetimer_irq_handler(int val)
{
   //int value = *SLT1_VALUE;

   // Do something with the value.
   //ISR_time=*SLT1_VALUE;
   ISR_times[to_usec(*SLT1_VALUE)]++;
   
   // Release the semaphore to trigger the "application" thread
   semGive(sem);

   // Reset the timer status to prevent infinite interrupting
   *SLT1_STATUS = SLT_STATUS_RESET;
}


void slicetimer_init(unsigned long period_ns)
{
   // HW: Configure our timer
   *SLT1_VALUE = 0;
   *SLT1_TCNT = period_ns / 30;    // One HW clock tick is 30 nanoseconds long
   *SLT1_CTRL |= SLT_CTRL_ENABLE | SLT_CTRL_INT_ENABLE | SLT_CTRL_RUN_WAIT;

   // WindAPI: Register interrupt service routine with the corresponding interrupt vector
   intConnect(INUM_TO_IVEC(INUM_SLT1),  slicetimer_irq_handler, 0);

   // HW: Configure the Interrupt Controller to allow interrupts from Slice Timer 1
   *ICTL_CPMIMR |= ICTL_CPMIMR_MAIN_MASK0;

   // WindAPI: Enable the interrupt
   intEnable(INUM_SLT1);
}


void slicetimer_disable()
{
   // HW: Configure the Interrupt Controller to disable interrupts from Slice Timer 1
   *ICTL_CPMIMR &= ~ICTL_CPMIMR_MAIN_MASK0;

   // HW: Disable our timer by writing zero to its configuration register
   *SLT1_CTRL = 0;

   // WindAPI: Disable the interrupt
   intDisable(INUM_SLT1);

   // WindAPI: Disconnect the interrupt service routine
   intDisconnect(INUM_TO_IVEC(INUM_SLT1),  slicetimer_irq_handler, 0);
}


void service_thread(){
	while(1){
		semTake(sem,WAIT_FOREVER);
		task_times[to_usec(*SLT1_VALUE)]++;
	}
	
}

int main(){
	int id;
	int minute=60;
	int seconds,i;
	
	task_times=(int*)calloc(1000,sizeof(int));
	ISR_times=(int*)calloc(1000,sizeof(int));
	sem = semCCreate(SEM_Q_FIFO,SEM_EMPTY);
	slicetimer_init(300000);
	id=taskSpawn("service thread", 0, 0, 4096, (FUNCPTR) service_thread, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	int max_isr = 0, max_task = 0;
	for (seconds = 0; seconds < minute; ++seconds) {
		printf("number %d\n",seconds);
		sleep(1);
		for (i = 0; i < 300; ++i) {
			printf("%d	",i);
			printf("%d	",ISR_times[i]);
			printf("%d	\n",task_times[i]);
			if (ISR_times[i] > 0 && i > max_isr)
				max_isr = i; 
			if (task_times[i] > 0 && i > max_task)
				max_task = i; 
		}
		printf("\n");
		printf("max:%d max:%d\n", max_isr, max_task);

	}
	taskDelete(id);

	printf("\n");
	return 0;
}






