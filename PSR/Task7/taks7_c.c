#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inetLib.h>
#include <sockLib.h>
#include <time.h>

#define MAX_BUF 1400
//#define MAX_BUF 4

int array[50];

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

int main(){
	
	int sockd;
	struct sockaddr_in my_addr, srv_addr;
	char buf[MAX_BUF];
	int count;
	int addrlen;
	struct timespec start,end,time;
	sysClkRateSet(5000);
	
	/*if (argc < 3)
	{
		fprintf(stderr, "Usage: %s ip_address port_number\n", argv[0]);
		exit(1);
	}*/
	/* Create a UDP socket */
	sockd = socket(AF_INET, SOCK_DGRAM, 0);
	/*if (sockd == -1)
	{
		perror("Socket creation error");
		exit(1);
	}*/
	
	/* Configure client address */
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = 0;
	
	bind(sockd, (struct sockaddr*)&my_addr, sizeof(my_addr));
	
	strcpy(buf, "Hi\n");
	
	/* Set server address */
	srv_addr.sin_family = AF_INET;
	inet_aton("192.168.202.63", &srv_addr.sin_addr);
	srv_addr.sin_port = htons(5000);
	int i,j;
	for (i = 0; i < 100000; ++i) {
		j=0;
		clock_gettime(CLOCK_MONOTONIC,&start);
		//printf("sending");
		sendto(sockd, buf, MAX_BUF, 0,
			(struct sockaddr*)&srv_addr, sizeof(srv_addr));
		
		addrlen = sizeof(srv_addr);
		//printf("receiving");
		count = recvfrom(sockd, buf, MAX_BUF, 0,
			(struct sockaddr*)&srv_addr, &addrlen);
		//write(1, buf, count);
		clock_gettime(CLOCK_MONOTONIC,&end);
		timespec_subtract(&time,&end,&start);
		/*time.tv_nsec/=2;
		if(time.tv_sec%2==1){
			time.tv_nsec+=500000000;
		}*/
		time.tv_sec/=2;
		//printf("time %ld ns\n",time.tv_nsec);
		while((j+1)<=(int)time.tv_nsec/200000){
			//printf("j=%d\n",j);
			array[j]++;
			j++;
		}
		
	}
	for(i=0;i<50;i++){
		printf("%d	%d	%d\n",i,(i+1)*200,array[i]);
	}
	printf("\n\n");
	
	
	close(sockd);
	return 0;
}
