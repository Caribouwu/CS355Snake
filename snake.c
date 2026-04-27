#include <ncurses.h> //run program with -lncurses
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
//enable non canonical mode

int main(){
    struct termios tty; //struct to access termios data
    tcgetattr(STDIN_FILENO, &tty); //get attributes for the terminal
    tty.c_lflag &= ~ICANON; //set terminal to non canonical mode
    tty.c_lflag &= ~ECHO; //disable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &tty); //apply changes
    
    initscr();
    keypad(stdscr, TRUE); //accept arrow keys as inputs without printing to the terminal

    int snake_length = 1;
    int win_condition = LINES * COLS; //the full terminal size, massive board right now -> will make 10x10 later on probably
    char ch = getch();
    
    while(snake_length < win_condition){
        addch('@');
        refresh();
        getch();
    }



    endwin();
    return 0;
}
