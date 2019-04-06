//console.h ver 1
#include"console.h"
//console function

_console console;
void _console_get_max_window(COORD &max_size){
    max_size.X = console.max_column;
    max_size.Y = console.max_row;
}//_console_get_max_window

void _console_init(void){
    console.column = 80;
    console.row = 25;
    system("mode 650");
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut,&csbi);
    console.max_column = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    console.max_row    = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    _console_resize(80,25);
}//_console_init

void _console_pixel_resize(int width, int height){
    //resize window
    HWND conso = GetConsoleWindow();
    RECT re;
    GetWindowRect(conso, &re);
    MoveWindow(conso,re.left,re.top,width,height,TRUE);
    //  get window size
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut,&csbi);
    console.column = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    console.row    = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}//_console_pixel_resize
void _console_no_scroll(){
    int column, row;
    //  get window size
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut,&csbi);
    column = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    row = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    //    no scroll
    COORD new_size = {column,row};
    SetConsoleScreenBufferSize(hOut,new_size);
}//_console_no_scroll

void _console_resize(int column, int row){
    char tmp_string[256];
    sprintf(tmp_string,"mode %d,%d",column,row);
    system(tmp_string);
    console.column = column;
    console.row    = row;
}//_console_resize

void _console_clr(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwWriteCoord = {0,0};
    DWORD dwSize = console.column*console.row;
    DWORD charWriten;
    if (!FillConsoleOutputCharacter(hOut,(TCHAR) ' '    ,dwSize,dwWriteCoord,&charWriten)){
        return;
    }
    if (!FillConsoleOutputAttribute(hOut,console_default,dwSize,dwWriteCoord,&charWriten)){
        return;
    }
}//_console_clr

void _console_gotoxy(int x, int y){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = {x-1,y-1};
    SetConsoleCursorPosition(hOut,position);
}//_console_gotoxy

int _console_where_x(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hOut,&csbi)){
        return csbi.dwCursorPosition.X;
    }
    return -1;
}//_console_where_x

int _console_where_y(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hOut,&csbi)){
        return csbi.dwCursorPosition.Y;
    }
    return -1;
}//_console_where_y

void _console_text_color(int color){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut,color);
}//_console_text_color

void _console_set_cursor(bool visible, int cursor_size){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (cursor_size<=0){
        cursor_size = 20; // default size of cursor
    }
    CONSOLE_CURSOR_INFO cci;
    cci.bVisible = visible;
    cci.dwSize = cursor_size;
    SetConsoleCursorInfo(hOut, &cci);
}//_console_set_cursor

void _console_draw(int x, int y,string str,int color){
    int background_color, text_color;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwWriteCoord = {x-1,y-1};
    COORD coord_char=dwWriteCoord;
    DWORD charWriten;
    char lpCharacter;
    //print text
    _console_gotoxy(x,y);
    int Count = 0;
    for (int i=0;i<str.length();i++){
        Count ++;
        if (Count==1){
            ReadConsoleOutputCharacter(hOut,&lpCharacter,1,coord_char,&charWriten);
            if (lpCharacter==str[i]){// not write this character
                _skip_cursor(coord_char,1);
                Count=0;
                continue;
            }
        }
        if (i==str.length()-1||str[i]!=str[i+1]){
            FillConsoleOutputCharacter(hOut,str[i],Count,coord_char,&charWriten);
            _skip_cursor(coord_char,Count);
            Count = 0;
        }
    }

    //fill background
    if (color>=0){
        if (!FillConsoleOutputAttribute(hOut,(DWORD)color,str.length(),dwWriteCoord,&charWriten)){
                return;
        }
    }

    //reset to default
    _console_text_color(console_default);
}//_console_draw

void _console_draw_background(int x,int y,int bg[]){//bg[0] = length, bg[1...length] = value of background
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);;
    WORD lpAttribute;
    int nLength=0;
    COORD dwCoord = {x-1,y-1};
    DWORD charWriten;
    for (int i=1;i<=bg[0];i++){
        nLength++;
        ReadConsoleOutputAttribute(hOut,&lpAttribute,1,dwCoord,&charWriten);
        if (nLength==1&&bg[i]==lpAttribute){
            _skip_cursor(dwCoord,1);
            nLength = 0;
            continue;
        }
        if (i==bg[0]||bg[i]!=bg[i+1]){
            FillConsoleOutputAttribute(hOut,bg[i],nLength,dwCoord,&charWriten);
            _skip_cursor(dwCoord,nLength);
            nLength = 0;
        }
    }
}//_console_draw_background

void _console_write(string str){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut,&csbi);
    _console_draw(_console_where_x()+1,_console_where_y()+1,str,csbi.wAttributes);
}//_console_write

int colorToInt(int background, int text){
    return (background<<4)|text;// bg*16+text
}//colorToInt

void _skip_cursor(COORD& coord, int n){
    if (coord.X+n>=console.column){
        coord.Y = coord.Y+((coord.X+n)/console.column);
        coord.X = ((coord.X+n))%console.column;
    }else{
        coord.X = coord.X+n;
        coord.Y = coord.Y;
    }
}//_skip_cursor
//-----------------------------------------------------------------------

//time function

void my_time::time_save(){
    SYSTEMTIME ST;
    GetSystemTime(&ST);
    time_array[0] = ST.wHour;
    time_array[1] = ST.wMinute;
    time_array[2] = ST.wSecond;
    time_array[3] = ST.wMilliseconds;
}//my_time::time_save()
long long diff_time(my_time a, my_time b){
    int tmp = 0;
    for (int i=0;i<=3;i++){
        tmp += (b.time_array[i]-a.time_array[i])*time_val[i];
    }
    if (tmp < 0)
        tmp = time_val[0] + tmp;
    return tmp;
}//diff_time
void sleep(int milis){
    my_time last, now;
    last.time_save();
    while (1){
        now.time_save();
        if (diff_time(last,now)>=milis){
            return;
        }
    }
}//sleep
//--------------------------------------------------------------------------

int max(int a, int b){
    if (a>b) return a;
    return b;
}//max

//random function
int random(int a, int b){
    srand(time(NULL));
    return rand()%(b-a+1)+a;
}//random
