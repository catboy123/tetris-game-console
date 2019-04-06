//console.h ver 1
#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#pragma once
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

#define console_black			0
#define console_darkblue		1
#define console_darkgreen		2
#define console_darkcyan		3
#define console_darkred		    4
#define console_darkpink		5
#define console_darkyellow	    6
#define console_darkwhite		7
#define console_grey			8
#define console_blue			9
#define console_green			10
#define console_cyan			11
#define console_red			    12
#define console_pink			13
#define console_yellow		    14
#define console_white			15

#define console_default		    7
struct _console{
    int column;
    int row;
    int max_column, max_row;
};
//_console console;

struct my_time{
    int time_array[10];
    void time_save();
};

const int time_val[4] = {3600000,60000,1000,1};
//console function
void _console_init(void);
void _console_pixel_resize(int width, int height);
void _console_no_scroll();
void _console_resize(int column, int row);
void _console_clr();
void _console_gotoxy(int x, int y);
int  _console_where_x();
int  _console_where_y();
void _console_get_max_window(COORD &max_size);
void _console_text_color(int color);
void _console_set_cursor(bool visible, int cursor_size);
void _console_draw(int x, int y,string str,int color);
void _console_write(string str);
void _console_draw_background(int x,int y,int bg[]);
int colorToInt(int background, int text);
void _skip_cursor(COORD& coord,int n);
//time function
long long diff_time(my_time a, my_time b);
void sleep(int milis);
//menu function
int max(int a, int b);
//random function
int random(int a, int b);
#endif // CONSOLE_H_INCLUDED
