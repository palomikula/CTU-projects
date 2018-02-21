#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <semLib.h>
#include <fcntl.h>
#include <stdbool.h>

typedef struct{
    char name[20];
    int work_done;
    bool taken;
}company;

typedef struct{
    company companies[50];
}company_registry;

company_registry *ptr;
SEM_ID lock;

company_registry* init_shm(void)
{
	bool EMPTY = 0;
    int fd;

    /* Lock to protect manipulations with shared memory - accessible from multiple processes */
    lock = semOpen("/complock", SEM_TYPE_MUTEX, SEM_FULL, SEM_Q_FIFO, OM_CREATE, NULL);
    /*if(lock == SEM_ID_NULL){
    		EMPTY = 1;
    		lock = semOpen("/complock", SEM_TYPE_MUTEX, SEM_FULL, SEM_Q_FIFO, OM_CREATE | OM_EXCL, NULL);
    	}*/
    /* use semTake() and semGive() to protect the relevant code below */

    semTake(lock, WAIT_FOREVER);
    	
    if((fd = shm_open("/company", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR))!=-1){
    	EMPTY = true;
    }else{
   		fd = shm_open("/company", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
   	}
    	/* or consider using O_EXCL flag to find whether the memory
    	 * needs to be initialized (see memset below) or not */
    	/*if(EMPTY){
    		fd = shm_open("/company", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    	}*/
    	
    /* set the size of shared memory block */
    if (ftruncate (fd, sizeof(company_registry)) == -1) {
        perror("ftruncate");
        exit (1);
    }

    /* Map shared memory object in the process address space */
    ptr = (company_registry *)mmap(0, sizeof(company_registry),
                          PROT_READ | PROT_WRITE,
                          MAP_SHARED, fd, 0);

    if (ptr == (company_registry *)MAP_FAILED)
        exit (1);

    /* close the file descriptor; the mapping is not impacted by this */
    close (fd);

    /* ... */
    if(EMPTY){	
   		memset(ptr, 0, sizeof(company_registry));
    }

    semGive(lock);
    return(ptr);
}

