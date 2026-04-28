#include <ncurses.h> //run program with -lncurses
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//enable non canonical mode

int main(){
    struct termios tty; //struct to access termios data
    tcgetattr(STDIN_FILENO, &tty); //get attributes for the terminal
    tty.c_lflag &= ~ICANON; //set terminal to non canonical mode
    tty.c_lflag &= ~ECHO; //disable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &tty); //apply changes
    initscr(); //start game
    curs_set(0); //hide cursor
    keypad(stdscr, TRUE); //accept arrow keys as inputs without printing to the terminal


    int xPosition, yPosition;
    xPosition = COLS/2;
    yPosition = LINES/2;
    int moveX = 0;
    int moveY = 0;
    while(true){
        int keyPressed = getch();
        if(keyPressed == KEY_UP){
            moveX = 0;
            moveY = -1;
        }
        if(keyPressed == KEY_DOWN){
            moveX = 0;
            moveY = 1;
        }
        if(keyPressed == KEY_LEFT){
            moveX = -1;
            moveY = 0;
        }
        if(keyPressed == KEY_RIGHT){
            moveX = 1;
            moveY = 0;
        }
        xPosition += moveX;
        yPosition += moveY;
        clear();
        move(yPosition,xPosition);
        addch('@');
        refresh();
    }

    endwin();
    return 0;
}
