#include"menu.h"
void menu::change_menu_position(COORD position){
    int x,y;
    x = position.X-top_left.X;
    y = position.Y-top_left.Y;
    top_left = position;
    bot_right.X += x;
    bot_right.Y += y;
}//menu::change_menu_position

void menu::add_option(string name_option){
    menu_option.push_back(name_option);
    this->bot_right.X = max(this->bot_right.X,name_option.length()-1+(this->top_left.X));
    this->bot_right.Y ++;
    value_select.push_back(-1);
    n_value_select.push_back(0);
}//menu::add_option

int real_length_option(string str){
    for (int i=0;i<str.length();i++){
        if (str[i]=='>'){
            return i;//(i-1)+1
        }
    }
}//real_length_option

void menu::add_option_value(string name_value,int option){
    menu_option[option-1] = menu_option[option-1]+">"+name_value;
    int real_length = real_length_option(menu_option[option-1]);
    this->bot_right.X = max(bot_right.X,top_left.X+real_length-1+name_value.length()+2);//name_option<name_value>
    if (value_select[option-1]<0){
        value_select[option-1] = 1;
    }
    n_value_select[option-1]++;
}//menu::add_option_value

string menu::option_str(int option){
    int len = menu_option[option-1].length();
    string option_tmp = "";
    string value_option = "";
    for (int i=0;i<len;i++){
        if (menu_option[option-1][i]=='>'){
            int n=0;
            for (int j=i;j<len;j++){
                if (menu_option[option-1][j]=='>'){
                    n++;
                }
                if (n>=value_select[option-1]){
                    for (int k = j+1;k<len;k++){
                        if (menu_option[option-1][k]=='>'){
                            break;
                        }
                        value_option+=menu_option[option-1][k];
                    }
                    break;
                }
            }
            break;
        }
        option_tmp += menu_option[option-1][i];
    }
    int menu_width = this->bot_right.X-this->top_left.X+1;
    if (value_option!=""){
        int n = (menu_width-(option_tmp.length()+value_option.length()+2)) ;
        for (int i=1;i<=n;i++){
            option_tmp=option_tmp+" ";
        }
//        cout << value_option << "\n";
        option_tmp= option_tmp+"<"+value_option+">";
    }
    while (option_tmp.length()<menu_width) option_tmp+=" ";
    return option_tmp;
}//menu::option_str

void menu::print_select_option(int option){
    int x,y;
    x = top_left.X;
    y = top_left.Y+option-1;
    _console_gotoxy(x,y);
    _console_text_color(color_select);
    _console_write(option_str(option));
}//menu::print_select_option

void menu::print_unselect_option(int option){
    int x,y;
    x = top_left.X;
    y = top_left.Y+option-1;
    _console_gotoxy(x,y);
    _console_text_color(color_unselect);
    _console_write(option_str(option));
}//menu::print_unselect_option

void menu::print(){
    // i+1 because option = 1.. size
    for (int i=0;i<menu_option.size();i++){
        if (i+1==select_option){
            print_select_option(i+1);
        }else{
            print_unselect_option(i+1);
        }
    }
}//menu::print

void menu::clr(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD charWriten;
    DWORD dwSize = bot_right.X-top_left.X+1;
    COORD dwWriteCoord;
    for (int i=1;i<=menu_option.size();i++){
        dwWriteCoord = {top_left.X-1,top_left.Y+i-2};
        if (!FillConsoleOutputCharacter(hOut,(TCHAR) ' '    ,dwSize,dwWriteCoord,&charWriten)) return;
        if (!FillConsoleOutputAttribute(hOut,console_default,dwSize,dwWriteCoord,&charWriten)) return;
    }
}//menu::clr

void menu::change_option(char c,bool print){
    int new_option;
    if (c=='n'){//next
        new_option = (select_option+1)% menu_option.size();
        if (new_option==0)
            new_option = menu_option.size();
    }
    if (c=='p'){//previous
        new_option = (select_option+menu_option.size()-1) % menu_option.size();
        if (new_option==0){
            new_option = menu_option.size();
        }
    }
    if (print){
        print_select_option(new_option);
        print_unselect_option(select_option);
    }
    select_option = new_option;
}//menu::change_option

void menu::change_option_value(int option, char c, bool print){
    int tmp = value_select[option-1];
    if (tmp<0){
        return;
    }
    int new_value_option;
    if (c=='n'){
        new_value_option = (tmp+1)%n_value_select[option-1];
        if (new_value_option == 0){
            new_value_option = n_value_select[option-1];
        }
    }
    if (c=='p'){
        new_value_option = (tmp+n_value_select[option-1]-1)%n_value_select[option-1];
        if (new_value_option == 0){
            new_value_option = n_value_select[option-1];
        }
    }
    value_select[option-1] = new_value_option;
    if (print){
        if (select_option==option){
            print_select_option(option);
        }else{
            print_unselect_option(option);
        }
    }
}//menu::change_option_value
