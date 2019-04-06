#include "console.h"
#include <conio.h>
#include <sstream>
#include "simple_menu.h"
#include "menu.h"
using namespace std;

#define color_block 12<<4
#define color_empty 15<<4
#define default_step 350

bool game_exit = false;
simple_menu choice_menu({30,15},12);
menu setting_menu({25,10},12<<4,15<<4);
int screen[50][15];
COORD screen_game = {2,2};
char list_block[7] = {'s','z','t','o','j','l','i'};
string color_array[16] = {"Black","Blue","Green","Cyan","Red","Magenta","Yellow","White","Gray","Light Blue","Light Green"
                         ,"Light Cyan","Light Red","Light Magenta","Light Yellow","Bright White"};

long int get_score(int line);
string IntToStr(long int a);

char block_name;
COORD block_top_left;
COORD block_bot_right;

void create_block(char c);
void write_screen();
bool down_block(COORD& top_left, COORD& bot_right);
bool left_block(COORD& top_left, COORD& bot_right);
bool right_block(COORD& top_left, COORD& bot_right);
bool rotate_block(COORD& top_left,COORD& bot_right,char name);
int clear_line();

int random_array[30];
char random_block();

void init_game(){
    // main menu game
    choice_menu.add_option("New Game");
    choice_menu.add_option("Exit");
    //init setting menu
    setting_menu.add_option("Dificult");
    setting_menu.add_option("Map Color");
    setting_menu.add_option("Block Color");
    setting_menu.add_option_value("Super Easy",1);//500
    setting_menu.add_option_value("Easy",1);//300
    setting_menu.add_option_value("Medium",1);//200
    setting_menu.add_option_value("Hard",1);//100
    setting_menu.add_option_value("Extreme",1);//50
    for (int i=0;i<16;i++){
        setting_menu.add_option_value(color_array[i],2);
        setting_menu.add_option_value(color_array[i],3);
    }
    setting_menu.value_select[0] = 3;//medium
    setting_menu.value_select[1] = 15+1;//white
    setting_menu.value_select[2] = 12+1;//red
    // init random array
    for (int i=1;i<=4;i++){
        random_array[i] = 0;
    }
    for (int i=5;i<=8;i++){
        random_array[i] = 1;
    }
    for (int i=9;i<=12;i++){
        random_array[i] = 2;
    }
    for (int i=13;i<=16;i++){
        random_array[i] = 3;
    }
    for (int i=17;i<=18;i++){
        random_array[i] = 4;
    }
    for (int i=19;i<=20;i++){
        random_array[i] = 5;
    }
    random_array[21] = 6;
    char c = random_block();
}

int get_choice(){
    _console_resize(80,25);
    _console_clr();
    _console_draw(30,19,"Press W,S to move",7);
    _console_draw(30,20,"Press X to exit game",7);
    _console_draw(30,21,"Press ENTER to choice",7);
    choice_menu.print();
    bool repeat = true;
    while (!game_exit&&repeat){
        _console_set_cursor(false,0);
        if (_kbhit()){
            char c = _getch();
            switch (c){
                case 's':
                case 'S':{//down
                    choice_menu.change_option('n',true);
                    break;
                }
                case 'w':
                case 'W':{//up
                    choice_menu.change_option('p',true);
                    break;
                }
                case 13:{
                    repeat = false;
                    break;
                }
                case 'x':
                case 'X':{
                    game_exit = true;
                    break;
                }
            }
        }
    }
    choice_menu.clr();
    return choice_menu.now_option;
}

void setting_choice(){
    if (game_exit) return;
    _console_resize(80,25);
    _console_clr();
    _console_draw(25,14,"Press A,S,D,W to move",7);
    _console_draw(25,15,"Press X to exit game",7);
    _console_draw(25,16,"Press ENTER to start game...",7);
    setting_menu.print();
    bool repeat = true;
    while (!game_exit && repeat){

        if (_kbhit()){
            char c = _getch();
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));//clear buffer
            switch(c){
                case 'a':
                case 'A':{// left
                    setting_menu.change_option_value(setting_menu.select_option,'p',true);
                    break;
                }
                case 'd':
                case 'D':{//right
                    setting_menu.change_option_value(setting_menu.select_option,'n',true);
                    break;
                }
                case 's':
                case 'S':{//down
                    setting_menu.change_option('n',true);
                    break;
                }
                case 'w':
                case 'W':{//up
                    setting_menu.change_option('p',true);
                    break;
                }
                case 'x':
                case 'X':{
                    game_exit = true;
                    break;
                }
                case 13:{
                    repeat = false;
                    break;
                }
            }
        }
    }
}

void play_game(){
    if (game_exit) return;
    _console_clr();
    //draw window game
    _console_resize(60,42);
    for (int r=1;r<=60;r++){
        _console_draw(r,1,"#",7<<4);
        _console_draw(r,42,"#",7<<4);
    }
    for (int c=1;c<=42;c++){
        _console_draw(1,c,"#",7<<4);
        _console_draw(60,c,"#",7<<4);
        _console_draw(32,c,"#",7<<4);
    }
    //main process
    long int score = 0;
    char last_button = ' ';
    bool repeat = true;
    bool down_one_time = false;
    int step = default_step;
    my_time last_time, now;
    last_time.time_save();

    for (int i=0;i<=25;i++){
        for (int j=0;j<=10;j++){
            screen[i][j] = 0;
        }
    }
//    for (int j=1;j<=10;j++){
//        screen[25][j] = -1;
//    }
//    block_name = random_block();
    block_name = 'l';
    create_block(block_name);
    _console_draw(35,5,"Your score : 0",7);

    while (!game_exit && repeat){
        _console_set_cursor(false,0);
        now.time_save();
        if (down_one_time || diff_time(last_time,now)>step){// 1 step of game
            last_time.time_save();
            score =score +10;
            if (down_one_time){// just 1 time
                down_one_time = false;
            }
            if (down_block(block_top_left,block_bot_right) == true){
            }else{
                for (int i=block_top_left.Y;i<=block_bot_right.Y;i++){
                    for (int j=block_top_left.X;j<=block_bot_right.X;j++){
                        if (screen[i][j]==1){
                            screen[i][j] = -1;
                        }
                    }
                }
                score =score + get_score(clear_line());
                for (int j=1;j<=10;j++){// CHECK LOSE GAME
                    if (screen[5][j]!=0){
                        repeat = false;
                        break;
                    }
                }
                block_name = random_block();
                create_block(block_name);
            }
            write_screen();
            string tmp = "Your score : " + IntToStr(score);
            _console_draw(35,5,tmp,7);
        }
        if (_kbhit()){
            char c = _getch();
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));//clear buffer
            switch(c){
                case 'a':
                case 'A':{// left
                    if (last_button=='a'||last_button=='A')
                        sleep(30);
                    last_button = c;
                    if (left_block(block_top_left,block_bot_right)){
                        write_screen();
                    }
                    break;
                }
                case 'd':
                case 'D':{//right
                    if (last_button=='d'||last_button=='D')
                        sleep(30);
                    last_button = c;
                    if (right_block(block_top_left,block_bot_right)){
                        write_screen();
                    }
                    break;
                }
                case 's':
                case 'S':{//down
//                    if (last_button=='s'||last_button=='S')
//                        sleep(30);
                    last_button = c;
                    down_one_time = true;
                    break;
                }
                case 'w':
                case 'W':{//up
                    if (last_button=='w'||last_button=='W')
                        sleep(30);
                    last_button = c;
                    if (rotate_block(block_top_left,block_bot_right,block_name)){
                        write_screen();
                    }
                    break;
                }
                case 'x':
                case 'X':{
                    game_exit = true;
                    break;
                }
            }
        }
    }
    _console_clr();
//    _console_draw(10,10," __   __  _______  __   __  ______      _______  _______  _______  ______    _______ ",14);
//    _console_draw(10,11,"|  | |  ||       ||  | |  ||    _ |    |       ||       ||       ||    _ |  |       |",14);
//    _console_draw(10,12,"|  |_|  ||   _   ||  | |  ||   | ||    |  _____||       ||   _   ||   | ||  |    ___|",14);
//    _console_draw(10,13,"|       ||  | |  ||  |_|  ||   |_||_   | |_____ |       ||  | |  ||   |_||_ |   |___ ",14);
//    _console_draw(10,14,"|_     _||  |_|  ||       ||    __  |  |_____  ||      _||  |_|  ||    __  ||    ___|",14);
//    _console_draw(10,15,"  |   |  |       ||       ||   |  | |   _____| ||     |_ |       ||   |  | ||   |___ ",14);
//    _console_draw(10,16,"  |___|  |_______||_______||___|  |_|  |_______||_______||_______||___|  |_||_______|",14);
    _console_draw(20,20,"Your score is: "+IntToStr(score),14);
    _console_gotoxy(20,30);
    system("pause");
}

void intro_game(){
    _console_clr();
    _console_resize(80,25);
    _console_draw(10,1,"  _______   ______   _______   _____    _____    _____ ",14);
    _console_draw(10,2," |__   __| |  ____| |__   __| |  __ \\  |_   _|  / ____|",14);
    _console_draw(10,3,"    | |    | |__       | |    | |__) |   | |   | (___  ",14);
    _console_draw(10,4,"    | |    |  __|      | |    |  _  /    | |    \\___ \\ ",14);
    _console_draw(10,5,"    | |    | |____     | |    | | \\ \\   _| |_   ____) |",14);
    _console_draw(10,6,"    |_|    |______|    |_|    |_|  \\_\\ |_____| |_____/ ",14);
    _console_draw(10,7,"                                                       ",14);
    _console_draw(10,8,"                                                       ",14);
    _console_draw(30,15,"Author : Quan Luong",14);
}
//========================================================================================================================
int main(){
    _console_init();
    init_game();
    intro_game();
    _console_gotoxy(10,25);
    system("pause");
    while (!game_exit){
//        intro_menu();
        int choice = get_choice();
        switch(choice){
            case 1:{// play game
                setting_choice();
                play_game();
                break;
            }
            case 2:{// exit
                game_exit = true;
                break;
            }
        }
    }
    intro_game();
    _console_write("Press ENTER to exit.....");
    do{
    }while (!_kbhit());
}
//===================================================================================================================
void create_block(char c){
    switch(c){
        case 'i':case 'I':{
            for (int i=2;i<=5;i++){
                screen[i][6] = 1;
            }
            block_top_left = {6,2};
            block_bot_right = {6,5};
            break;
        }
        case 's': case 'S':{
            block_top_left = {5,4};
            block_bot_right = {7,6};
            screen[4][5] = 0;screen[4][6] = 1;screen[4][7] = 1;
            screen[5][5] = 1;screen[5][6] = 1;screen[5][7] = 0;
            break;
        }
        case 'z':case 'Z':{
            block_top_left = {5,4};
            block_bot_right = {7,6};
            screen[4][5] = 1;screen[4][6] = 1;screen[4][7] = 0;
            screen[5][5] = 0;screen[5][6] = 1;screen[5][7] = 1;
            break;
        }
        case 't':case 'T':{
            block_top_left = {5,4};
            block_bot_right = {7,6};
            screen[4][5] = 0;screen[4][6] = 1;screen[4][7] = 0;
            screen[5][5] = 1;screen[5][6] = 1;screen[5][7] = 1;
            break;
        }
        case 'o':case 'O':{
            block_top_left = {6,4};
            block_bot_right = {7,5};
            screen[4][6] = 1;screen[4][7] = 1;
            screen[5][6] = 1;screen[5][7] = 1;
            break;
        }
        case 'j': case 'J':{
            block_top_left = {5,3};
            block_bot_right = {7,5};
            screen[3][5] = 0;screen[3][6] = 1;screen[3][7] = 0;
            screen[4][5] = 0;screen[4][6] = 1;screen[4][7] = 0;
            screen[5][5] = 1;screen[5][6] = 1;screen[5][7] = 0;
            break;
        }
        case 'l': case 'L':{
            block_top_left = {5,3};
            block_bot_right = {7,5};
            screen[3][5] = 0;screen[3][6] = 1;screen[3][7] = 0;
            screen[4][5] = 0;screen[4][6] = 1;screen[4][7] = 0;
            screen[5][5] = 0;screen[5][6] = 1;screen[5][7] = 1;
            break;
        }
    }
}

void write_screen(){
    string tmp;
    int bg_tmp[50]; bg_tmp[0] = 30;
    for (int i=6;i<=25;i++){
        tmp = "";
        for (int j=1;j<=10;j++){
            if (screen[i][j]!=0){
                bg_tmp[3*j-2] = color_block;
                bg_tmp[3*j-1] = color_block;
                bg_tmp[3*j] = color_block;
                tmp =tmp + "   ";
            }else{
                bg_tmp[3*j-2] = color_empty;
                bg_tmp[3*j-1] = color_empty;
                bg_tmp[3*j] = color_empty;
                tmp = tmp + "   ";
            }
        }
        _console_draw(screen_game.X,screen_game.Y+2*(i-6),tmp,-1);
        _console_draw(screen_game.X,screen_game.Y+2*(i-6)+1,tmp,-1);
        _console_draw_background(screen_game.X,screen_game.Y+2*(i-6),bg_tmp);
        _console_draw_background(screen_game.X,screen_game.Y+2*(i-6)+1,bg_tmp);
    }
}

bool down_block(COORD& top_left, COORD& bot_right){
    for (int i=bot_right.Y;i>=top_left.Y;i--){
        for (int j=bot_right.X;j>=top_left.X;j--){
            if (screen[i][j]==1){
                if (i+1>25|| screen[i+1][j]==-1)
                    return false;
            }
        }
    }
    for (int i=bot_right.Y;i>=top_left.Y;i--){
        for (int j=bot_right.X;j>=top_left.X;j--){
            if (screen[i][j]==1){
                screen[i+1][j] = screen[i][j];
                screen[i][j] = 0;
            }
        }
    }
    top_left.Y++;
    bot_right.Y++;

    return true;
}

bool left_block(COORD& top_left,COORD& bot_right){
    for (int i=top_left.Y;i<=bot_right.Y;i++){
        for (int j=top_left.X;j<=bot_right.X;j++){
            if (screen[i][j]==1){
                if (j==1 || screen[i][j-1]==-1){
                    return false;
                }
            }
        }
    }
    for (int i=top_left.Y;i<=bot_right.Y;i++){
        for (int j=top_left.X;j<=bot_right.X;j++){
            if (screen[i][j]==1){
                screen[i][j-1] = screen[i][j];
                screen[i][j] = 0;
            }
        }
    }
    top_left.X--;
    bot_right.X--;
    return true;
}
bool right_block(COORD& top_left,COORD& bot_right){
    for (int i=top_left.Y;i<=bot_right.Y;i++){
        for (int j=bot_right.X;j>=top_left.X;j--){
            if (screen[i][j]==1){
                if (j==10 || screen[i][j+1]==-1){
                    return false;
                }
            }
        }
    }
    for (int i=top_left.Y;i<=bot_right.Y;i++){
        for (int j=bot_right.X;j>=top_left.X;j--){
            if (screen[i][j]==1){
                screen[i][j+1] = screen[i][j];
                screen[i][j] = 0;
            }
        }
    }
    top_left.X++;
    bot_right.X++;
    return true;
}
int clear_line(){
    int Count = 0;
    for (int i=25;i>=6;i--){
        bool full_line = true;
        for (int j=1;j<=10;j++){
            if (screen[i][j]!=-1){
                full_line = false;
            }
        }
        if (full_line){
            for (int k=i-1;k>=6;k--){
                for (int j=1;j<=10;j++){
                    screen[k+1][j] = screen[k][j];
                    screen[k][j] = 0;
                }
            }
            i++;//check line i again because the tetris fall down
            Count ++;
        }
    }
    return Count;
}
bool rotate_block(COORD& top_left,COORD& bot_right,char name){
    switch(name){
        case 'o':case 'O': return false;
        case 'i': case 'I':{
            if (top_left.Y==bot_right.Y){//nam ngang
                // check if can rotate
                int i,j;
                j=top_left.X+1;
                for (i=top_left.Y+1;i<=top_left.Y+3;i++){
                    if (i>25||screen[i][j]!=0)
                        return false;
                }
                //clear block
                i = top_left.Y;
                for (j=top_left.X;j<=bot_right.X;j++){
                    screen[i][j] = 0;
                }
                //create new block
                top_left.X = top_left.X+1;
                bot_right.X = top_left.X;
                bot_right.Y = top_left.Y+3;
                j = top_left.X;
                for (i=top_left.Y;i<=bot_right.Y;i++){
                    screen[i][j] = 1;
                }
            }else{// nam doc
                //check if rotate
                int i,j;
                i = top_left.Y;
                for (j=top_left.X-1;j<=top_left.X+2;j++){
                    if (j<1 || j>10 || screen[i][j]==-1){
                        return false;
                    }
                }
                //clear block
                j = top_left.X;
                for (i=top_left.Y;i<=bot_right.Y;i++){
                    screen[i][j] = 0;
                }
                //create block
                bot_right.X = top_left.X+2;
                bot_right.Y = top_left.Y;
                top_left.X = top_left.X-1;
                i = top_left.Y;
                for (j=top_left.X;j<=bot_right.X;j++){
                    screen[i][j] = 1;
                }
            }
            return true;
        }
        default:{
            //rotate and give it to tmp[3][3]
            int tmp[5][5];
            for (int i=top_left.Y;i<=bot_right.Y;i++){
                for (int j=top_left.X;j<=bot_right.X;j++){
                    int x = i-top_left.Y+1;
                    int y = j-top_left.X+1;
                    if (screen[i][j]!=-1)
                        tmp[y][3-x+1] = screen[i][j];
                    else tmp[y][3-x+1] = 0;
                }
            }
            //check if can rotate
            for (int i=top_left.Y;i<=bot_right.Y;i++){
                for (int j=top_left.X;j<=bot_right.X;j++){
                    int x = i-top_left.Y+1;
                    int y = j-top_left.X+1;
                    if (tmp[x][y]==1){
                        if (i>25 || j>10 || j<1 || screen[i][j]==-1)
                            return false;
                    }
                }
            }
            // clear block
            for (int i=top_left.Y;i<=bot_right.Y;i++){
                for (int j=top_left.X;j<=bot_right.X;j++){
                    if (screen[i][j]==1){
                        screen[i][j] = 0;
                    }
                }
            }
            //create new block
            for (int i=top_left.Y;i<=bot_right.Y;i++){
                for (int j=top_left.X;j<=bot_right.X;j++){
                    int x = i-top_left.Y+1;
                    int y = j-top_left.X+1;
                    if (tmp[x][y]==1)
                        screen[i][j] = tmp[x][y];
                }
            }
            break;
        }
    }
    if (name == 'l' || name == 'L' || name == 'j' || name == 'J'){
        int i= top_left.Y;
        bool full_empty = true;
        for (int j=top_left.X;j<=bot_right.X;j++){
            if (screen[i][j]!=0){
                full_empty = false;
                break;
            }
        }
        if (full_empty){
            for (i=top_left.Y;i<=bot_right.Y-1;i++){
                for (int j = top_left.X;j<=bot_right.X;j++){
                    screen[i][j] = screen[i+1][j];
                    if (screen[i+1][j]==1)
                        screen[i+1][j] = 0;
                }
            }
        }
    }

}
char random_block(){
    int seek = time(NULL)%21+1;
    for (int i=seek;i<=21;i++){
        int j = random(1,21);
        swap(random_array[i],random_array[j]);
    }
    for (int i=1;i<seek;i++){
        int j = random(1,21);
        swap(random_array[i],random_array[j]);
    }
    int r = random_array[random(1,21)];
    return list_block[r];
}
long int get_score(int line){
    switch (line){
        case 1:
            return 2000;
        case 2:
            return 5000;
        case 3:
            return 10000;
        case 4:
            return 50000;
    }
}
string IntToStr(long int a){
    stringstream ss;
    if (a>0)
        ss<<a;
    else
        ss<<"0";
    return ss.str();
}
