#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include "fbDrawer.h"
#include "gameControl.h"

void SetTimer(int sec, void(*proc)(int)) {
	struct itimerval timer;
	timer.it_interval.tv_sec = 1;
	timer.it_interval.tv_usec = 0;
	timer.it_value.tv_sec = 1;
	timer.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &timer, NULL);
	signal(SIGALRM, proc);
}


void alarm_handler(int signum) {
	moveDown();
}

static struct termios newtermset, oldtermset;

void noecho(void) {
	tcgetattr(STDIN_FILENO, &oldtermset);
	newtermset = oldtermset;
	newtermset.c_lflag &= ~ICANON;
	newtermset.c_lflag &= ~ECHO;
	newtermset.c_cc[VMIN] = 1;
	tcsetattr(STDIN_FILENO, TCSANOW, &newtermset);
}

void reset_term(void) {
	tcsetattr(STDIN_FILENO, TCSANOW, &oldtermset);
}

int getch(void) {
	int ch;
	noecho();
	ch = getchar();
	reset_term();
	return ch;
}

int main(void) {
	openfb("/dev/fb0");
	initGame();
	SetTimer(1,alarm_handler);
	while(1) {
		char c;
		c = getch();
		fflush(NULL);
		switch(c) {
		case 'w':
			rotate();
			break;
		case 'a':
			moveLeft();
			break;
		case 's':
			moveDown();
			break;
		case 'd':
			moveRight();
			break;
		case 'q':
			printf("Game Aborted!!!\n");
			printf("Your score:%d\n",getScore());
			exit(0);
		default:
			break;			
		}
	}
	closefb();
	return 0;
}
