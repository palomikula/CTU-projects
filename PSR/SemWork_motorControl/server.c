#include <taskLib.h>
#include <stdio.h>
#include <kernelLib.h>
#include <semLib.h>
#include <intLib.h>
#include <iv.h>
#include <xlnx_zynq7k.h>
#include <lite5200b.h>
#include <arch/ppc/ppc5200.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <inetLib.h>
#include <sockLib.h>
#include <math.h>

#define SERVER_PORT             80 /* Port 80 is reserved for HTTP protocol */
#define SERVER_MAX_CONNECTIONS  20
#define K                       48
#define MAX_SPEED               0b00000000000000000000001100000000
#define CW                      0b01000000000000000000000000000000
#define ACW                     0b10000000000000000000000000000000
#define SPIN                    *(volatile uint32_t *) (0x43c20000 + 0x000C)

SEM_ID move_sem;
volatile int irc_a, irc_b;
volatile int p_irc=0;
int sockd;
struct sockaddr_in my_addr, srv_addr;
struct sockaddr_in my_name, cli_name;
char buf[32];
int addrlen;
int motorPos;
int recvPos;
short dir = -1;
int motorPosBuf[50];
int recvPosBuf[50];
int pointerBuf;


void www(void)
{
  int i;
  int pos;
  int s;
  int tmpPointer;
  int newFd;
  struct sockaddr_in serverAddr;
  struct sockaddr_in clientAddr;
  int sockAddrSize;

  sockAddrSize = sizeof(struct sockaddr_in);
  bzero((char *) &serverAddr, sizeof(struct sockaddr_in));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(SERVER_PORT);
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  s=socket(AF_INET, SOCK_STREAM, 0);
  if (s<0)
  {
    printf("Error: www: socket(%d)\n", s);
    return;
  }


  if (bind(s, (struct sockaddr *) &serverAddr, sockAddrSize) == ERROR)
  {
    printf("Error: www: bind\n");
    return;
  }

  if (listen(s, SERVER_MAX_CONNECTIONS) == ERROR)
  {
    perror("www listen");
    close(s);
    return;
  }

  printf("www server running\n");

  while(1)
  {
    /* accept waits for somebody to connect and the returns a new file descriptor */
    if ((newFd = accept(s, (struct sockaddr *) &clientAddr, &sockAddrSize)) == ERROR)
    {
      perror("www accept");
      close(s);
      return;
    }

    FILE *f = fdopen(newFd, "w");
    fprintf(f, "HTTP/1.0 200 OK\r\n\r\n");
    fprintf(f, "<html>\
				  <head>\
					<title>Motor status</title>\
				  </head>\
				  <body onload=\"setTimeout(function(){location.reload()}, 100);\">\
				    <svg width=\"600\" height=\"400\" xmlns='http://www.w3.org/2000/svg'>\
					  <g transform=\"translate(50,380) scale(1)\">\
						<!-- Now Draw the main X and Y axis -->\
						<g style=\"stroke-width:2; stroke:black\">\
						  <!-- X Axis -->\
						  <path d=\"M 0 0 L 500 0 Z\"/>\
						  <!-- Y Axis -->\
						  <path d=\"M 0 -360 L 0 0 Z\"/>\
						</g>\
						<g style=\"fill:none; stroke:#B0B0B0; stroke-width:1; stroke-dasharray:2 4;text-anchor:end; font-size:15\">\
						  <text style=\"fill:black; stroke:none\" x=\"-1\" y=\"-360\" >360</text>\
						  <text style=\"fill:black; stroke:none\" x=\"-1\" y=\"0\" >0</text>\
						  <g style=\"text-anchor:middle\">\
							<text style=\"fill:black; stroke:none\" x=\"100\" y=\"20\" >100</text>\
							<text style=\"fill:black; stroke:none\" x=\"200\" y=\"20\" >200</text>\
							<text style=\"fill:black; stroke:none\" x=\"300\" y=\"20\" >300</text>\
							<text style=\"fill:black; stroke:none\" x=\"400\" y=\"20\" >400</text>\
							<text style=\"fill:black; stroke:none\" x=\"500\" y=\"20\" >500</text>\
						  </g>\
						</g>\
						<polyline\
							  points=\"");
    tmpPointer = pointerBuf;
	for (i=0; i<50; i++) {
		if (motorPosBuf[tmpPointer] > 0) pos = (int)ceil((((motorPosBuf[tmpPointer])%514)/514.0)*360-360);
		else                             pos = (int)ceil((((motorPosBuf[tmpPointer])%514)/514.0)*360);
		fprintf(f, "%d, %d\r\n", i*10, pos );
		tmpPointer++;
		if (tmpPointer==50) tmpPointer=0;
	}
	fprintf(f, 							"\"\
							  style=\"stroke:red; stroke-width: 1; fill : none;\"/>\
						<polyline\
							  points=\"");
    tmpPointer = pointerBuf;
	for (i=0; i<50; i++) {
		if (recvPosBuf[tmpPointer] > 0) pos = (int)ceil((((recvPosBuf[tmpPointer])%514)/514.0)*360-360);
		else                            pos = (int)ceil((((recvPosBuf[tmpPointer])%514)/514.0)*360);
		fprintf(f, "%d, %d\r\n", i*10, pos);
		tmpPointer++;
		if (tmpPointer==50) tmpPointer=0;
	}
	fprintf(f, 							"\"\
							  style=\"stroke:blue; stroke-width: 1; fill : none;\"/>\
					  </g>\
					</svg>\
					<svg width=\"600\" height=\"550\" xmlns='http://www.w3.org/2000/svg'>\
					  <g transform=\"translate(50,530) scale(1)\">\
						<!-- Now Draw the main X and Y axis -->\
						<g style=\"stroke-width:2; stroke:black\">\
						  <!-- X Axis -->\
						  <path d=\"M 0 0 L 500 0 Z\"/>\
						  <!-- Y Axis -->\
						  <path d=\"M 0 -500 L 0 0 Z\"/>\
						</g>\
						<g style=\"fill:none; stroke:#B0B0B0; stroke-width:1; stroke-dasharray:2 4;text-anchor:end; font-size:15\">\
						  <text style=\"fill:black; stroke:none\" x=\"-1\" y=\"0\" >0</text>\
						  <text style=\"fill:black; stroke:none\" x=\"-1\" y=\"-500\" >100</text>\
						  <g style=\"text-anchor:middle\">\
							<text style=\"fill:black; stroke:none\" x=\"100\" y=\"20\" >100</text>\
							<text style=\"fill:black; stroke:none\" x=\"200\" y=\"20\" >200</text>\
							<text style=\"fill:black; stroke:none\" x=\"300\" y=\"20\" >300</text>\
							<text style=\"fill:black; stroke:none\" x=\"400\" y=\"20\" >400</text>\
							<text style=\"fill:black; stroke:none\" x=\"500\" y=\"20\" >500</text>\
						  </g>\
						</g>\
						<polyline\
							  points=\"");
    tmpPointer = pointerBuf;
	for (i=0; i<50; i++) {
		fprintf(f, "%d, %d\r\n", i*10, (int)ceil(((abs(motorPosBuf[tmpPointer]-recvPosBuf[tmpPointer])*K)/(double)MAX_SPEED)*-100) );
		tmpPointer++;
		if (tmpPointer==50) tmpPointer=0;
	}
	if (motorPos > 0) pos = (int)ceil((((motorPos)%514)/514.0)*360-360)*-1;
	else              pos = (int)ceil((((motorPos)%514)/514.0)*360)*-1;
	fprintf(f, 							"\"\
							  style=\"stroke:red; stroke-width: 1; fill : none;\"/>\
					  </g>\
					</svg>\
					<br/>\
					<br/>\
					<br/>\
					<br/>\
					<table>\
    		          <tr><td>Actual motor position</td><td>%d</td><tr>", pos);
	if (recvPos > 0) pos = (int)ceil((((recvPos)%514)/514.0)*360-360)*-1;
	else             pos = (int)ceil((((recvPos)%514)/514.0)*360)*-1;
	fprintf(f,       "<tr><td>Received position</td><td>%d</td><tr>\
    				  <tr><td>Motor speed</td><td>%d</td></tr>\
    			    </table>\
				  </body>\
				</html>", pos, abs(motorPos-recvPos)*K);
    
    fclose(f);
  }
}

void init_connection() {
	/* Create a UDP socket */
	sockd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockd == -1)
	{
		perror("Socket creation error");
		exit(1);
	}

	/* Configure client address */
	my_name.sin_family = AF_INET;
	my_name.sin_addr.s_addr = INADDR_ANY;
	my_name.sin_port = htons(5000);

	bind(sockd, (struct sockaddr*)&my_name, sizeof(my_name));
    addrlen = sizeof(cli_name);
}

void recv_position () {
	while(1) {
		int status = recvfrom(sockd, buf, 32, 0,(struct sockaddr*)&cli_name, &addrlen);
		int i;
		recvPos=0;
		if (buf[0]=='-') {
			for(i=1; buf[i]!=' '; i++) {
				recvPos*=10;
				recvPos+=buf[i]-'0';
			}
			recvPos*=-1;
		} else {
			for(i=0; buf[i]!=' '; i++) {
				recvPos*=10;
				recvPos+=buf[i]-'0';
			}
		}
	}
}

void set_position () {
	while(1) {
		//printf("Motor position %d\n", motorPos);
		//printf("Received position: %d\n",recvPos);
		if (recvPos < motorPos) {
			dir = -1;
			unsigned int speed = ((motorPos-recvPos)*K); 
			if (speed > MAX_SPEED) speed = MAX_SPEED;
			SPIN = CW | speed;
			while (1) {
				printf("RM Actual position %d / %d\n", motorPos, recvPos);
				if (motorPos <= recvPos) break;
				speed = ((motorPos-recvPos)*K); 
				if (speed > MAX_SPEED) speed = MAX_SPEED;
				SPIN = CW | speed;
			}
			SPIN=0;
		} else if (recvPos > motorPos) {
			dir = 1;
			unsigned int speed = ((recvPos-motorPos)*K);
			if (speed > MAX_SPEED) speed = MAX_SPEED;
			SPIN = ACW | speed;
			while (1) {
				printf("MR Actual position %d / %d\n", motorPos, recvPos);
				if (motorPos >= recvPos) break;
				speed = ((recvPos-motorPos)*K);
				if (speed > MAX_SPEED) speed = MAX_SPEED;
				SPIN = ACW | speed;
			}
			SPIN=0;
		}
		SPIN=0;
		//printf("Final position %d\n", motorPos);
		taskDelay(2);
	}
}

void irc_isr(void)
{
	int sr; /* status register */
	sr = *(volatile uint32_t *) (0x43c20000 + 0x0004);
	irc_a = (sr & 0x100) >> 8;
	irc_b = (sr & 0x200) >> 9;
	int irc = 0;
	irc = (irc_a << 1) + irc_b;
	if ((p_irc == 0 && irc == 1) ||
		(p_irc == 1 && irc == 3) ||
		(p_irc == 3 && irc == 2) ||
		(p_irc == 2 && irc == 0)) motorPos++;
	if ((p_irc == 1 && irc == 0) ||
		(p_irc == 3 && irc == 1) ||
		(p_irc == 2 && irc == 3) ||
		(p_irc == 0 && irc == 2)) motorPos--;
	p_irc = irc;
	*(volatile uint32_t *) (ZYNQ7K_GPIO_BASE + 0x00000298) = 0x4; /* reset (stat) */
	//SPIN=0;
}

void irc_init(void)
{
        *(volatile uint32_t *) (ZYNQ7K_GPIO_BASE + 0x00000298) = 0x4; /* reset (stat) */
        *(volatile uint32_t *) (ZYNQ7K_GPIO_BASE + 0x00000284) = 0x0; /* set as input (dirm) */
        *(volatile uint32_t *) (ZYNQ7K_GPIO_BASE + 0x0000029c) = 0x4; /* rising edge (type) */
        *(volatile uint32_t *) (ZYNQ7K_GPIO_BASE + 0x000002a0) = 0x0; /* rising edge (polarity) */
        *(volatile uint32_t *) (ZYNQ7K_GPIO_BASE + 0x000002a4) = 0x0; /* rising edge (any) */
        *(volatile uint32_t *) (ZYNQ7K_GPIO_BASE + 0x00000290) = 0x4; /* enable interrupt (en) GPIO2 */

        intConnect(INUM_TO_IVEC(INT_LVL_GPIO), irc_isr, 0);
        intEnable(INT_LVL_GPIO);
}

void irc_disable(void)
{
        *(volatile uint32_t *) (ZYNQ7K_GPIO_BASE + 0x00000294) = 0x4; /* disable interrupt (dis) */

        intDisable(INT_LVL_GPIO);
        intDisconnect(INUM_TO_IVEC(INT_LVL_GPIO), irc_isr, 0);
}

void buffer(void) {
	pointerBuf = 0;
	
	while(1) {
		motorPosBuf[pointerBuf] = motorPos;
		recvPosBuf[pointerBuf] = recvPos;
		pointerBuf++;
		if (pointerBuf == 50) pointerBuf = 0;
		taskDelay(2);
	}
}

void motor(){

	TASK_ID wwwTask;
	TASK_ID recvData;
	TASK_ID moveMotor;
	TASK_ID bufferTask;
	motorPos=0;
	pointerBuf = 0;

	/* IRC init */
	irc_init();
	/* webserver task */
	wwwTask = taskSpawn("wwwTask", 210, 0, 4096, (FUNCPTR) www, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	/* PWM init */
	*(volatile uint32_t *) (0x43c20000 + 0x0000) = 0b01000000;
	/* Set PWM freq 20 kHz */
	*(volatile uint32_t *) (0x43c20000 + 0x0008) = 5000;
	/* UDP connection init */
	init_connection();
	recvData = taskSpawn("recvData", 50, 0, 4096, (FUNCPTR) recv_position, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	moveMotor = taskSpawn("moveMotor", 40, 0, 4096, (FUNCPTR) set_position, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	bufferTask = taskSpawn("bufferTask", 100, 0, 4096, (FUNCPTR) buffer, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	
	while(1);
	
	taskDelay(10000);
	printf("Out of play time.\n");
	
	irc_disable();
	taskDelete(bufferTask);
	taskDelete(recvData);
	taskDelete(moveMotor);
	taskDelete(wwwTask);
}
