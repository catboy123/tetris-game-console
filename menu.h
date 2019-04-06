#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#pragma once
#include"console.h"

struct menu{
    COORD top_left;
    COORD bot_right;
    int color_select;
    int color_unselect;
    int select_option;
    std::vector<string> menu_option;
    std::vector<int>    value_select;
    std::vector<int>    n_value_select;
    menu(COORD tl, int select, int unselect){
        top_left = tl;
        bot_right = tl;
        color_select = select;
        color_unselect = unselect;
        select_option = 1;
    }
    string option_str(int option);
    void print_select_option(int option);
    void print_unselect_option(int option);
    void change_option(char c, bool print);
    void change_option_value(int option, char c, bool print);
    void add_option_value(string name_value,int option);
    void add_option(string name_option);
    void print();
    void clr();
    void change_menu_position(COORD position);
};
int real_length_option(string str);


#endif // MENU_H_INCLUDED
