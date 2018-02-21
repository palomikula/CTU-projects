#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"


typedef struct{
    unsigned char R,G,B;
}pixelRGB;

typedef struct{
    unsigned char RK,GK,BK,RB,GB,BB;
}RGBknobs;

typedef struct{
    long double re;
    long double im;
}cmplx;

cmplx step(cmplx z, cmplx c){
    cmplx result;
    result.re = z.re*z.re - z.im*z.im + c.re;
    result.im = 2*z.re*z.im + c.im;
    return result;
}

RGBknobs unpack(uint32_t rgb_knobs_value){

	RGBknobs knobs;

	knobs.BK = rgb_knobs_value  & 0xFF;
	knobs.GK = (rgb_knobs_value>>8)  & 0xFF;
	knobs.RK = (rgb_knobs_value>>16)  & 0xFF;

	knobs.BB = (rgb_knobs_value>>24) & 1;
	knobs.GB = (rgb_knobs_value>>25) & 1;
	knobs.RB = (rgb_knobs_value>>26) & 1;

	return knobs;
}

RGBknobs diffs(RGBknobs now ,RGBknobs before){
	RGBknobs diff;
	diff.BK = now.BK - before.BK;
	diff.GK = now.GK - before.GK;
	diff.RK = now.RK - before.RK;

	//printf("diff R %d\n", before.RK);

	return diff;
}

uint16_t color_16bit(pixelRGB RGB){
    uint16_t color;
    color = ((RGB.R >> 3) << 11) | ((RGB.G >> 2) << 5) | (RGB.B >> 3);
    return color;
}



int height = 320;
int width = 480;
int itr = 100;
int zoom = 1;
int shift_ver = 0, shift_hor = 0;
uint16_t pixels[320*480];
//pixels = malloc(320*480*sizeof(uint16_t));
RGBknobs knobs;
RGBknobs diff;
cmplx z,c;
uint32_t rgb_knobs_value;
pthread_t tid[2];
int mode = 1;
int b_prev = 1;
int i;


unsigned char *parlcd_mem_base;
unsigned char *mem_base;

void *output(void *arg){
	uint16_t *pixels = (uint16_t*)arg;
	/*parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
	mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);*/
	parlcd_write_cmd(parlcd_mem_base, 0x2c);
	for( i= 0; i < width*height; ++i){
		parlcd_write_data(parlcd_mem_base,pixels[i]);
	}
	return NULL;
}
int main(int argc, char** argv){
	RGBknobs knobs_before_m1 = unpack(0);
	RGBknobs knobs_before_m2 = unpack(0);
	parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
	mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
	if (parlcd_mem_base == NULL)
    	exit(1);

  	parlcd_hx8357_init(parlcd_mem_base);

    pixelRGB pixel;
		//zoom = 1;
		c.re = -0.4;
		c.im = 0.6;
	rgb_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	knobs = unpack(rgb_knobs_value);
	while(1){
		int h,w,i;
		double t;
		//zoom = 1;
		/*rgb_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
		knobs = unpack(rgb_knobs_value);*/
		//mode = (knobs.RB == 1 && b_prev == 0) ? ((mode == 1) ? 2 : 1) : mode;
		switch(mode){
			case 1:

				c.re = ((double)((unsigned char)(knobs.RK-diff.RK))/127.0)-1.0;
				c.im = ((double)((unsigned char)(knobs.GK-diff.GK))/127.0)-1.0;
				itr = (unsigned char)(knobs.BK-diff.BK);
				printf("c.re %f\n", c.re);
				printf("c.im %f\n", c.im);
				printf("itr %d\n", itr);
				printf("value %d\n", rgb_knobs_value);
				//knobs_before_m1 = knobs;
				break;

			case 2:
				//diff = diffs(knobs,knobs_before_m2);

				shift_ver = ((unsigned char)(knobs.RK- diff.RK)) - 127;
				shift_hor = ((unsigned char)(knobs.GK - diff.GK)) - 127;
				//zoom = ((unsigned char)(knobs.BK - diff.GB));
				zoom = 1;
				int k;
				for(k=1; k<=((unsigned char)(knobs.BK - diff.BK)); ++k){
					zoom += 0.12*k + 1;
				}
				printf("shift_ver %d\n", shift_ver);
				printf("shift_hor %d\n", shift_hor);
				printf("zoom %d\n", zoom);

				printf("value %d\n", rgb_knobs_value);

				//knobs_before_m2 = knobs;
				break;
		}
		printf("Blue %d\n", knobs.BK);
		printf("Green %d\n", knobs.GK);
		printf("Red %d\n", knobs.RK);

		printf("MODE %d\n", mode);

		printf("fontmaxwidth  %d\n", font_winFreeSystem14x16);
		for(h = 0; h<height; ++h){
		    for(w = 0; w<width; ++w){

		        z.re = (((w)*4+shift_hor*zoom)/((double)width) - 2)/zoom;
		        z.im = (((h)*4+shift_ver*zoom)/((double)height) - 2)/zoom;

				rgb_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
				knobs = unpack(rgb_knobs_value);
				if(knobs.RB == 1 && b_prev == 0){
					if(mode == 1){
						mode = 2;
						knobs_before_m1 = knobs;
						diff = diffs(knobs,knobs_before_m2);
					}else{
						mode = 1;
						knobs_before_m2 = knobs;
						diff = diffs(knobs,knobs_before_m1);
					}
				}

				b_prev = knobs.RB;

		        for(i = 0; i<itr && z.re*z.re + z.im*z.im < 4; ++i){
		            z = step(z,c);
		        }

		        t = i/(double)itr;

		        pixel.R = 9*(1-t)*t*t*t*255;
		        pixel.G = 15*(1-t)*(1-t)*t*t*255;
		        pixel.B = 8.5*(1-t)*(1-t)*(1-t)*t*255;

				pixels[width*h+w] = color_16bit(pixel);

		    }
		}
		pthread_create(&(tid[0]), NULL, output, pixels);
		/*for(int i = 0; i < width*height; ++i){
			parlcd_write_data(parlcd_mem_base,pixels[i]);
		}*/

	}

  return 0;
}
