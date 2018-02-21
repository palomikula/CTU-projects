#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

FILE* f_in,*f_out;
int histogram[] = {0,0,0,0,0};
int size,tmp,value;
short int index;
int clrRange, height, width;
unsigned char **data, **new_data;
char f_type[2];

int main(int argc, char** argv)
{
    f_in = fopen(argv[1],"rb");
    if(fscanf(f_in,"%s %d %d %d",f_type,&width,&height,&clrRange)!=4){
        return -1;
    }
    fgetc(f_in);
    size = 3*width * height;
    data = malloc(height*sizeof(char*));
    new_data = malloc(height*sizeof(char*));
    for (int i=0;i<height;++i){
        *(data+i)=malloc(3*width);
        *(new_data+i)=malloc(3*width);
    }
    if(fread(data[0], 3*width, 1, f_in)!=1){
        return -1;
    }
    if(fread(data[1], 3*width, 1, f_in)!=1){
        return -1;
    }
    f_out = fopen("output.ppm","w");
    fprintf(f_out,"%s\n%d\n%d\n%d\n",f_type,width,height,clrRange);
    fwrite(data[0], 3*width, 1, f_out);
    for(int j = 1; j<height-1; ++j){
        if(fread(data[j+1], 3*width, 1, f_in)!=1){
        return -1;
        }
        new_data[j][0]=data[j][0];
        new_data[j][1]=data[j][1];
        new_data[j][2]=data[j][2];
		value = round(0.2126*new_data[j][0]+ 0.7152*new_data[j][1] + 0.0722*new_data[j][2]);
        index = (value==255) ? 4 : floor(value/51);
        histogram[index]++;
        new_data[j][3*width-3]=data[j][3*width-3];
        new_data[j][3*width-2]=data[j][3*width-2];
        new_data[j][3*width-1]=data[j][3*width-1];
		value = round(0.2126*new_data[j][3*width-3]+ 0.7152*new_data[j][3*width-2] + 0.0722*new_data[j][3*width-1]);
        index = (value==255) ? 4 : floor(value/51);
        histogram[index]++;
        for(int i=3; i<3*(width-1); i+=3){
            tmp=5*(data[j][i])-(data[j][i+3] + data[j][i-3] + data[j+1][i] + data[j-1][i]);
            new_data[j][i] = (tmp <= 255) ? ((tmp >0) ? tmp : 0) : 255;
			tmp=5*(data[j][i+1])-(data[j][i+4] + data[j][i-2] + data[j+1][i+1] + data[j-1][i+1]);
            new_data[j][i+1] = (tmp <= 255) ? ((tmp >0) ? tmp : 0) : 255;
			tmp=5*(data[j][i+2])-(data[j][i+5] + data[j][i-1] + data[j+1][i+2] + data[j-1][i+2]);
            new_data[j][i+2] = (tmp <= 255) ? ((tmp >0) ? tmp : 0) : 255;
            value = round(0.2126*new_data[j][i]+ 0.7152*new_data[j][i+1] + 0.0722*new_data[j][i+2]);
            index = (value==255) ? 4 : floor(value/51);
            histogram[index]++;
            
        }


        fwrite(new_data[j], 3*width, 1, f_out);
    }
	for(int i = 0; i<3*width; i+=3){
		value = round(0.2126*data[0][i]+ 0.7152*data[0][i+1] + 0.0722*data[0][i+2]);
        index = (value==255) ? 4 : floor(value/51);
        histogram[index]++;
		value = round(0.2126*data[height-1][i]+ 0.7152*data[height-1][i+1] + 0.0722*data[height-1][i+2]);
        index = (value==255) ? 4 : floor(value/51);
        histogram[index]++;
	}
    fwrite(data[height-1], 3*width, 1, f_out);
    fclose(f_in);
    fclose(f_out);
    for (int i=0;i<height;++i){
        free(data[i]);
        free(new_data[i]);
    }
    free(data);
    free(new_data);
	f_out = fopen("output.txt","w");
    fprintf(f_out,"%d %d %d %d %d",histogram[0],histogram[1],histogram[2],histogram[3],histogram[4]);
    fclose(f_out);
    }
