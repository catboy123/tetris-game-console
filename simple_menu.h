#ifndef SIMPLE_MENU_H_INCLUDED
#define SIMPLE_MENU_H_INCLUDED
#pragma once
#include<iostream>
#include<stdio.h>
#include <windows.h>
#include <vector>
#include <string>
#include "console.h"

struct simple_menu{
    COORD top_left;
    COORD bot_right;
    int color;
    int now_option = 1;
    std::vector<string> menu_option;
    simple_menu(COORD tl, int c){
        top_left = tl;
        bot_right = tl;
        color = c;
    };
    void add_option(string name_option);
    void print_option(int option, bool visible);
    void print();
    void change_option(char c, bool visible);
    void clr();
};

#endif // SIMPLE_MENU_H_INCLUDED
