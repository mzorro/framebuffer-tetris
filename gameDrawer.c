#include "fbDrawer.h"
#include "gameDrawer.h"
void drawFrame() {
	drawline(W*CELL+2,0,W*CELL+2,H*CELL,0xFFFFFFFF);
	drawline(0,H*CELL+2,W*CELL,H*CELL+2,0xFFFFFFFF);
}

void drawCell(int x,int y)
{
	x *= CELL;   y *= CELL;
	rectangle(x+1,y+1,x+CELL,y+CELL,0xFFFFFFFF);
}


void coverCell(int x,int y)
{
	x *= CELL;   y *= CELL;
	rectangle(x+1,y+1,x+CELL,y+CELL,0);
}

