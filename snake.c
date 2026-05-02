#include <ncurses.h> //run program with -lncurses
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
//enable non canonical mode

#define WIDTH 80
#define HEIGHT 20

int main(){
    struct termios tty; //struct to access termios data
    tcgetattr(STDIN_FILENO, &tty); //get attributes for the terminal
    tty.c_lflag &= ~ICANON; //set terminal to non canonical mode
    tty.c_lflag &= ~ECHO; //disable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &tty); //apply changes
    initscr(); //start game
    curs_set(0); //hide cursor
    keypad(stdscr, TRUE); //accept arrow keys as inputs without printing to the terminal
    int score = 0; // start score
    timeout(150); // keeps moving waitng for new input 


    int xPosition, yPosition;
    xPosition = WIDTH/2; 
    yPosition = HEIGHT/2 + 3;
    int moveX = 1;
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
        
        char title[] = "SNAKE GAME";
        mvprintw(1, 2, "Score: %d ", score); //displays score 
        mvprintw(1,(WIDTH+2 - strlen(title))/2, "%s", title); // displays title

        for(int x = 0; x <= WIDTH +1; x++){
            mvaddch(3, x, '*');
            mvaddch(HEIGHT + 4, x, '*');
        }
        for(int y = 3; y<= HEIGHT+4; y++){
            mvaddch(y, 0, '*');
            mvaddch(y, WIDTH + 1, '*');
        }

        move(yPosition,xPosition);
        addch('@');
        refresh();
    }

    endwin();
    return 0;
}
