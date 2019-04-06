#include"simple_menu.h"

void simple_menu::add_option(string name_option){
    menu_option.push_back(name_option);
    bot_right.X = max(bot_right.X,top_left.X + name_option.length()+8 - 1);
    bot_right.Y++;
}

void simple_menu::print_option(int option, bool visible){
    string tmp_str = "";
    option --;
    int len = bot_right.X-top_left.X+1;
    int x = top_left.X;
    int y = top_left.Y+option;
    if (visible){
        tmp_str = "->  "+menu_option[option]+"  <-";
    }else{
        tmp_str = "    "+menu_option[option]+"    ";
    }
//    cout<< tmp_str << " " << tmp_str.length();
    int left_space = (len-tmp_str.length())/2;
    for (int i=1;i<=left_space;i++){
        tmp_str = " "+tmp_str;
    }
    while (tmp_str.length()<len) tmp_str=tmp_str+" ";
//    cout << tmp_str;
    _console_draw(x,y,tmp_str,color);
}

void simple_menu::print(){
    for (int i=1;i<=menu_option.size();i++){
        if (i==now_option){
            print_option(i,true);
        }else{
            print_option(i,false);
        }
    }
}

void simple_menu::change_option(char c, bool visible){
    if (visible){
        print_option(now_option,false); // xoa lua chon cu
    }
    if (c=='n'){
        now_option = (now_option+1)%menu_option.size();
        if (now_option == 0){
            now_option = menu_option.size();
        }
    }else{
        now_option = (now_option-1)%menu_option.size();
        if (now_option == 0){
            now_option = menu_option.size();
        }
    }
    if (visible){
        print_option(now_option,true);
    }
}

void simple_menu::clr(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwSize = bot_right.X - top_left.X + 1;
    COORD dwCoord;
    DWORD charWriten;
    for (int y=top_left.Y;y<=bot_right.Y;y++){
        dwCoord = {top_left.X-1,y-1};
        FillConsoleOutputCharacter(hOut,(TCHAR)' ',dwSize,dwCoord,&charWriten);
        FillConsoleOutputAttribute(hOut,console_default,dwSize,dwCoord,&charWriten);
    }
}
