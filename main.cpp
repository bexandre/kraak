#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>


// currently, the code is quite frankly garbage
// i'm mainly trying to do something that works here
// don't worry, i'll get it cleaner and more readable
// ... eventually x)


// <minorFunctions>
void move_cursor (int x, int y) { printf("\033[%d;%dH", y, x); }
bool not_equal (char ch1, char ch2, char ch3 = '?', char ch4 = '?')
{ return (ch1 != ch2)*(ch1 != ch3)*(ch1 != ch4); }
// i'll probably make it a va_list thingy
// but i'm a really lazy fuck
void clear() {
	move_cursor(0,0);
	for (int i = 0; i < 120*30; i++) {
		printf(" ");
	}
}
// </minorFunctions>


// i might use classes later but for now i'll keep it simple
// i'm still not used to c++ that much so imma use struct as
// long as i ain't confident enough to manipulate classes ^^
struct map {
	char tab[509][907+1];
	int pos[2];
};

struct zoom {
	map Map;
	char tab[10][10+1];
	int old_pos[2];
	bool after_2nd_frame = false;
};

struct controls {
	char up,down,left,right;
};


// it's a very lemgthy boi so i decided i'd just put it in
// another file instead lmfao
#include "init_map.h"

zoom init_zoom (map M) {
	int q, r;
	zoom ret;
	for (int i = 0; i < 10*10; i++) {
		q = i/10;
		r = i - 10*q;
		ret.tab[q][r] = M.tab[M.pos[0]+r-5][M.pos[1]+q-5];
	}
	ret.Map = M;
	ret.after_2nd_frame = 0;
	ret.old_pos[0] = M.pos[0];
	ret.old_pos[1] = M.pos[1];
	return ret;
}

controls init_controls () {
	controls ret;
	clear();
	move_cursor(10,5); printf("Up    : [X]");
	move_cursor(10,6); printf("Down  : [X]");
	move_cursor(10,7); printf("Left  : [X]");
	move_cursor(10,8); printf("Right : [X]");
}


void update (zoom& displayed, int ch, controls user_controls) {
	int q, r, disp_tmp;
	for (int i = 0; i < 10*10; i++) {
		q = i/10;
		r = i - 10*q;
		disp_tmp = displayed.Map.tab[displayed.Map.pos[0]+r-5][displayed.Map.pos[1]+q-5];
		displayed.tab[q][r] = disp_tmp;
	}
}

void show (zoom displayed) {
	int q, r, disp_tmp, prev_tmp;
	for (int i = 0; i < 10*10; i++) {
		q = i/10;
		r = i - 10*q;
		disp_tmp = displayed.Map.tab[displayed.Map.pos[0]+q-5][displayed.Map.pos[1]+r-5];
		prev_tmp = displayed.Map.tab[displayed.old_pos[0]+q-5][displayed.old_pos[1]+r-5];
		if ((displayed.after_2nd_frame and disp_tmp != prev_tmp) or !displayed.after_2nd_frame)
									{ move_cursor(2*r+10,q+5); printf("%c%c",disp_tmp,disp_tmp); }
	}
	move_cursor(10+10,5+5); printf("[]");
}


int main () {
	int ch;
	system("title Kraak (pre-alpha)");
	map M = init_map();
	controls user_controls = init_controls();
	move_cursor(19,5); user_controls.up = getch(); if (user_controls.up < 0) user_controls.up = getch(); printf("V");
	move_cursor(19,6); user_controls.down = getch(); if (user_controls.down < 0) user_controls.down = getch(); printf("V");
	move_cursor(19,7); user_controls.left = getch(); if (user_controls.left < 0) user_controls.left = getch(); printf("V");
	move_cursor(19,8); user_controls.right = getch(); if (user_controls.right < 0) user_controls.right = getch(); printf("V");
	clear();
	zoom displayed = init_zoom(M);
	show(displayed);
	displayed.after_2nd_frame = true;
	ch = getch();
	for (;;) {
		displayed.old_pos[0] = displayed.Map.pos[0];
		displayed.old_pos[1] = displayed.Map.pos[1];
		if (ch == user_controls.up and not_equal(displayed.Map.tab[displayed.Map.pos[0]-1][displayed.Map.pos[1]], '\xB2', '\xB1')) displayed.Map.pos[0]--;
		if (ch == user_controls.down and not_equal(displayed.Map.tab[displayed.Map.pos[0]+1][displayed.Map.pos[1]], '\xB2', '\xB1')) displayed.Map.pos[0]++;
		if (ch == user_controls.left and not_equal(displayed.Map.tab[displayed.Map.pos[0]][displayed.Map.pos[1]-1], '\xB2', '\xB1')) displayed.Map.pos[1]--;
		if (ch == user_controls.right and not_equal(displayed.Map.tab[displayed.Map.pos[0]][displayed.Map.pos[1]+1], '\xB2', '\xB1')) displayed.Map.pos[1]++;
		// ^^^ this has to be the most terrific shit i've ever written ^^^
		update(displayed,ch,user_controls);
		show(displayed);
		usleep(30000);
		ch = getch();
	}
}
