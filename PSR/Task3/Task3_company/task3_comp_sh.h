#ifndef TASK3_SH_H
#define TASK3_SH_H
#include <stdbool.h>

typedef struct {
	char name[20];
	int work_done;
	bool taken;
} company;

typedef struct {
	company companies[50];
} company_registry;

//company_registry *ptr;

company* init_shm(company *this_comp);

#endif

