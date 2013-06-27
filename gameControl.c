#include "gameControl.h"
#include "gameDrawer.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int blocks[W+1][H+1];    //表示整个游戏区域。-1表示没有方块，0~6表示各种方块,7表示边界
int curx[5],cury[5];     //[0]到[3]表示当前四个方块的位置
                         //curx[4]表示当前方块所占4*4矩形到游戏左边界距离
                         //cury[4]表示当前方块所占4*4矩形到游戏上边界距离
int kind,score;          //当前方块的种类和游戏的分数

int getScore() {
	return score;
}

void initGame() {
	int i,j;
	for(i=0;i<W;i++)
		for(j=0;j<H;j++)
			blocks[i][j] = -1;
	for(i=0;i<W+1;i++)
		blocks[i][H] = 7;
	for(i=0;i<H+1;i++)
		blocks[W][i] = 7;
	for(i=0;i<5;i++)
		curx[i] = cury[i] = 0;
	score = 0;
	newBlock();
	drawFrame();
}

void newBlock() {
	srand((unsigned)time(0));
	int i,j,k;
	kind = rand()%7;
	switch(kind) {
	case 0:
		//▓▓▓▓▓
		curx[4] = W/2 - 2 , cury[4] = -2;
		for(i=0;i<4;i++)
			curx[i] = curx[4] + i , cury[i] = -1;
		break;
	case 1:
		//▓
		//▓▓▓
		curx[4] = W/2 - 2 , cury[4] = -2;
		curx[0] = curx[4] , cury[0] = -1;
		for(i=1;i<4;i++)
			curx[i] = curx[4]-1+i , cury[i] = 0;
		break;
	case 2:
		//  ▓
		//▓▓▓
		curx[4] = W/2 - 2 , cury[4] = -2;
		curx[0] = curx[4] + 2 , cury[0] = -1;
		for(i=1;i<4;i++)
			curx[i] = curx[4]-1+i , cury[i] = 0;
		break;
	case 3:
		// ▓
		//▓▓▓
		curx[4] = W/2 - 2 , cury[4] = -2;
		curx[0] = curx[4] + 1 , cury[0] = -1;
		for(i=1;i<4;i++)
			curx[i] = curx[4]-1+i , cury[i] = 0;
		break;
	case 4:
		//▓▓
		// ▓▓
		curx[4] = W/2 - 2 , cury[4] = -2;
		for(i=0;i<2;i++)
			curx[i] = curx[4] + i , cury[i] = -1;
		for(;i<4;i++)
			curx[i] = curx[4]-1+i , cury[i] = 0;
		break;
	case 5:
		// ▓▓
		//▓▓
		curx[4] = W/2 - 2 , cury[4] = -2;
		for(i=0;i<2;i++)
			curx[i] = curx[4]+1+i , cury[i] = -1;
		for(;i<4;i++)
			curx[i] = curx[4]-2+i , cury[i] = 0;
		break;
	case 6:
		// ▓▓
		// ▓▓
		curx[4] = W/2 - 2 , cury[4] = -2;
		for(i=0;i<2;i++)
			curx[i] = curx[4]+1+i , cury[i] = -1;
		for(;i<4;i++)
			curx[i] = curx[4]-1+i , cury[i] = 0;
		break;
	default:
		break;
	}
	int move;//临时变量---判断是否可以移动或者旋转
	for(i=H-1;i>=0;i--) {
		for(j=0,move=TRUE;j<W;j++)
			if(blocks[j][i]<0) {
				move = FALSE;
				break;
			}
		if(move==TRUE) {
			score += 100;
			for(k=i++;k>=0;k--)
				for(j=0;j<W;j++)
					if(k>0&&(blocks[j][k]=blocks[j][k-1])>=0)
						drawCell(j,k);
					else coverCell(j,k);
		}
	}
}

void moveDown() {
	int i;
    for(i=0;i<4;i++)
		if(blocks[curx[i]][cury[i]+1]>=0) {
			if(cury[4]<0) {
				printf("Game Over!!!\n");
				printf("Your score:%d\n",score);
				exit(0);
			}
			for(i=0;i<4;i++)
				blocks[curx[i]][cury[i]] = kind;
			newBlock();
			return;
		}
	for(i=0;i<4;i++)
	    coverCell(curx[i],cury[i]++);
	for(i=0;i<4;i++)
		drawCell(curx[i],cury[i]);
	cury[4]++;
}

void moveLeft() {
	int i;
	for(i=0;i<4;i++)
		if(curx[i]<=0 || blocks[curx[i]-1][cury[i]]>=0)
			return;
	for(i=0;i<4;i++)
		coverCell(curx[i]--,cury[i]);
	for(i=0;i<4;i++)
		drawCell(curx[i],cury[i]);
	curx[4]--;	    
}

void moveRight() {
	int i;
	for(i=0;i<4;i++)
		if(blocks[curx[i]+1][cury[i]]>=0)
			return;
	for(i=0;i<4;i++)
		coverCell(curx[i]++,cury[i]);
	for(i=0;i<4;i++)
		drawCell(curx[i],cury[i]);
	curx[4]++;
}

void rotate() {
	int i,cgdx[4],cgdy[4];//临时变量---旋转后四个方块的位置和循环变量
	if(kind==6) return;
	for(i=0;i<4;i++) {
		cgdx[i] = 3 - (cury[i]-cury[4]) + curx[4];
		cgdy[i] = curx[i] - curx[4] + cury[4];
		if(cgdx[i]<=0 || blocks[cgdx[i]][cgdy[i]]>=0)
			return;
	}
	for(i=0;i<4;i++) {
		coverCell(curx[i],cury[i]);
		curx[i] = cgdx[i] , cury[i] = cgdy[i];
	}
	for(i=0;i<4;i++)
		drawCell(curx[i],cury[i]);
}
