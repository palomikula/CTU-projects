#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <semLib.h>
#include <fcntl.h>
#include <stdbool.h>


typedef struct {
	char name[20];
	int work_done;
	bool taken;
} company;

typedef struct {
	company companies[50];
} company_registry;

company_registry *ptr;
SEM_ID lock;

company* init_shm(char name[20]) {

	int fd;
	bool FIRST_COMP = false;
	//printf("%s\n",this_comp->name);
	/* Lock to protect manipulations with shared memory - accessible from multiple processes */
	lock = semOpen("/complock", SEM_TYPE_MUTEX, SEM_FULL, SEM_Q_FIFO, OM_CREATE,
	NULL);

	/* use semTake() and semGive() to protect the relevant code below */

	semTake(lock, WAIT_FOREVER);
	//printf("%s\n",this_comp->name);
	if((fd = shm_open("/company", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR))!=-1){
		FIRST_COMP = true;
	}else{
		fd = shm_open("/company", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	}
	/* or consider using O_EXCL flag to find whether the memory
	 * needs to be initialized (see memset below) or not */
	/*if(FIRST_COMP){
		fd = shm_open("/company", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	}*/

	/* set the size of shared memory block */
	if (ftruncate(fd, sizeof(company_registry)) == -1) {
		perror("ftruncate");
		semGive(lock);
		exit(1);
	}

	/* Map shared memory object in the process address space */
	ptr = (company_registry *) mmap(0, sizeof(company_registry),
	PROT_READ | PROT_WRITE,
	MAP_SHARED, fd, 0);

	if (ptr == (company_registry *) MAP_FAILED)
		exit(1);

	/* close the file descriptor; the mapping is not impacted by this */
	close(fd);

	/* ... */

	/* the fist company should zero the memory this way: */
	if(FIRST_COMP){
		memset(ptr, 0, sizeof(company_registry));
	}

	/* ... register this company to the memory ... */
	int i=0;
	while(ptr->companies[i].taken)i++;
	printf("%d\n",i);
	if(i<50){
		ptr->companies[i].work_done = 0;
		strcpy(ptr->companies[i].name,name);
		ptr->companies[i].taken=true;
	}
	semGive(lock);

	return &(ptr->companies[i]);
	//msync(ptr,sizeof(company_registry),0);


}
