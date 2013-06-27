#include "fbDrawer.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/fb.h>
#include <string.h>
#include <unistd.h>

struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
long int screensize = 0;
char *fbp = NULL;
int fbfd = 0;

void openfb(char* bfname) {
	//open framebuffer device
    fbfd = open(bfname,O_RDWR);
    if(!fbfd) {
		printf("Error:cannot open the framebuffer device!\n");
		exit(1);
	}
    
	//get the fixed screen information
    if(ioctl(fbfd,FBIOGET_FSCREENINFO,&finfo)) {
		printf("Error reading fixed information\n");
		exit(2);
	}
	
	//get the variable screen information
    if(ioctl(fbfd,FBIOGET_VSCREENINFO,&vinfo)) {
		printf("Error reading variable information\n");
		exit(3);
	}

    screensize = vinfo.xres*vinfo.yres*vinfo.bits_per_pixel/8;

	//map the device to memory 
	fbp = (char *)mmap(0,screensize,PROT_READ|PROT_WRITE,MAP_SHARED,fbfd,0); 
    if(*(int*)fbp == -1) {
        printf("Error:failed to map framebuffer device to memory\n"); 
        exit(4); 
    }
	memset(fbp,0,screensize);
}

void closefb() {
	//release the memory
    munmap(fbp,screensize);
    close(fbfd);
}

void drawline(int x1, int y1, int x2, int y2,int color) {
	if(x1 > x2) {
	    int tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	if(y1 > y2) {
		int tmp = y1;
		y1 = y2;
		y2 = tmp;
	}
	if(y1<0 || x1<0) return;
	int x,y;
	if(x2-x1 > y2-y1) {
        for(x=x1;x<=x2;x++) {
			double k = (double)(y2-y1)/(x2-x1);
			y = (int)(k*(x-x1) + y1);
			int location = (x+vinfo.xoffset)*(vinfo.bits_per_pixel/8)
				+ (y+vinfo.yoffset)*finfo.line_length;
			memcpy(fbp+location, &color, vinfo.bits_per_pixel/8);
		}
	}
	else {
        for(y=y1;y<=y2;y++) {
			double k = (double)(x2-x1)/(y2-y1);
			x = (int)(k*(y-y1) + x1);
			int location = (x+vinfo.xoffset)*(vinfo.bits_per_pixel/8)
				+ (y+vinfo.yoffset)*finfo.line_length;
			memcpy(fbp+location, &color, vinfo.bits_per_pixel/8);
		}
	}
}

void rectangle(int x1, int y1, int x2, int y2,int color) {
	if(color==0) {
		int x;
		for(x=x1;x<=x2;x++)
			drawline(x,y1,x,y2,color);
	}
	else {
		drawline(x1,y1,x2,y1,color);
		drawline(x1,y1,x1,y2,color);
		drawline(x2,y1,x2,y2,color);
		drawline(x1,y2,x2,y2,color);
		int x;
		for(x=x1+2;x<=x2-2;x++)
			drawline(x,y1+2,x,y2-2,color);
	}
}
