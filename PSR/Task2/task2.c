#include <taskLib.h>
#include <stdio.h>
#include <kernelLib.h>


int lower_count = 0;
int upper_count = 0;


SEM_ID semShovels, semSoilHeap;

int WORK_TIME = 100;
int BREAK_TIME = 40;


void lower_digger(int n)
{
  while (1) {
	taskSafe();
    semTake(semShovels, WAIT_FOREVER);
    printf("lower digger %d: working\n", n);
    taskDelay(WORK_TIME);
    semGive(semSoilHeap);
    semGive(semShovels);
    taskUnsafe();
    printf("lower digger %d: taking break\n", n);
    taskDelay(BREAK_TIME);
  }
}

void upper_digger(int n)
{
  while (1) {
	taskSafe();
    semTake(semShovels, WAIT_FOREVER);
    if(semTake(semSoilHeap, NO_WAIT) == -1){
    	printf("upper digger %d: can't work\n", n);
    	semGive(semShovels);
    	taskUnsafe();
    	taskDelay(WORK_TIME);
    }else{
		printf("upper digger %d: working\n", n);
		taskDelay(WORK_TIME);
		semGive(semShovels);
		taskUnsafe();
		printf("upper digger %d: taking break\n", n);
		taskDelay(BREAK_TIME);
    }
    
  }
}

void control()
{
	semShovels = semCCreate(SEM_Q_FIFO, 3);
	semSoilHeap = semCCreate(SEM_Q_FIFO, 0);
	int key,i;
	int l=1;
	int u=1;
	int idl[1000];
	int idu[1000];
	char taskName[13];
	sprintf(taskName, "lower_digger%d", 0);
	idl[0]=taskSpawn(taskName, 210, 0, 4096, (FUNCPTR) lower_digger, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	
	sprintf(taskName, "upper_digger%d", 0);
	idu[0]=taskSpawn(taskName, 210, 0, 4096, (FUNCPTR) upper_digger, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	
	lower_count++;
	upper_count++;
	
	while(1){
		key = getchar();
		
		switch(key){
			case (int)'i':
				if(lower_count + upper_count <= 50 ){
					sprintf(taskName, "lower_digger%d", l);
					idl[l]=taskSpawn(taskName, 210, 0, 4096, (FUNCPTR) lower_digger, l, 0, 0, 0, 0, 0, 0, 0, 0, 0);
					printf("lower_digger%d added\n", l);
					lower_count++;
					l++;
				}
				break;
			
			case (int)'I':
				if(lower_count + upper_count <= 50 ){
					sprintf(taskName, "upper_digger%d", u);
					idu[u]=taskSpawn(taskName, 210, 0, 4096, (FUNCPTR) upper_digger, u, 0, 0, 0, 0, 0, 0, 0, 0, 0);
					printf("upper_digger%d added\n", u);
					upper_count++;
					u++;
				}
				break;
				
			case (int)'o':
				if(lower_count>0){
					taskDelete(idl[l - lower_count]);
					printf("lower_digger%d left\n", l - lower_count);
					lower_count--;
				}
				break;
				

			case (int)'O':
				if(upper_count>0){
					taskDelete(idu[u - upper_count]);
					printf("upper_digger%d left\n", u - upper_count);
					upper_count--;
				}
				break;
			
			case (int)'E':
				for(i=0; i<50; i++){
					taskDelete(idl[i]);
					taskDelete(idu[i]);
				}
			return;
			break;
		}
	}
}
	




