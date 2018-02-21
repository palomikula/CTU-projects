#include <taskLib.h>
#include <stdio.h>
#include <kernelLib.h>
#include <semLib.h>
#include <intLib.h>
#include <iv.h>
#include <xlnx_zynq7k.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <inetLib.h>
#include <sockLib.h>

volatile int irc_a, irc_b;
volatile int p_irc=0;
int sockd;
struct sockaddr_in my_addr, srv_addr;
char buf[32];
int motorPos;

void init_connection() {
	/* Create a UDP socket */
	sockd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockd == -1)
	{
		perror("Socket creation error");
		exit(1);
	}

	/* Configure client address */
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = 0;

	bind(sockd, (struct sockaddr*)&my_addr, sizeof(my_addr));

	srv_addr.sin_family = AF_INET;
	inet_aton("192.168.202.203", &srv_addr.sin_addr);
	srv_addr.sin_port = htons(5000);
}

void send_data () {
	int i;
	
	while (1) {
		//printf("Motor position %d\n", motorPos);
		snprintf(buf, 32, "%d ", motorPos);
		//printf("Send %s => ", buf);
		i = sendto(sockd, buf, 32, 0, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
		//printf("%d\n", i);
		taskDelay(1);
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


void motor(void)
{
        TASK_ID sendData;
        motorPos = 0;

        irc_init();
        init_connection();
        //motorGetPosTask = taskSpawn("motorGetPos", 50, 0, 4096, (FUNCPTR) get_position, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        sendData = taskSpawn("sendData", 50, 0, 4096, (FUNCPTR) send_data, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

        while(1);
        
        taskDelay(10000);
        printf("Out of play time.\n");

        irc_disable();
        taskDelete(sendData);
}
