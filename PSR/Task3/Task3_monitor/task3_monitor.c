#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <semLib.h>
#include <fcntl.h>
#include <stdbool.h>
#include "task3_sh.h"

company_registry *reg;

int main(){
	//reg = (company_registry*)malloc(sizeof(company_registry));
	reg = init_shm();
	int i;
	bool empty;
	while(1){
		//msync(reg,sizeof(company_registry),0);
		empty = true;
		for(i=0;i<50;++i){
			if(reg->companies[i].taken){
				printf("%s %d\n",reg->companies[i].name,reg->companies[i].work_done);
				empty = false;
			}
		}
		if(empty){
			printf("Nobody is working");
		}
		sleep(1);
		printf("\n\n");
	}



	return 0;
}
