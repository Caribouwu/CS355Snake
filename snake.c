#include <ncurses.h> //run program with -lncurses
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WIDTH 80 //reusable constants 
#define HEIGHT 20

//prototypes
int collision(int xPosition,int yPosition, int snakeLength);
void slither(int xPosition,int yPosition, int snakeLength);
void generateFruit(int xPosition,int yPosition, int *snakeLength);

//global variables !!
int fruitX,fruitY; 
bool checkFruit = false;
bool win = false;
bool lose = false;
int score = 0; // start score
int snakeSegmentX[WIDTH*2];
int snakeSegmentY[WIDTH*2];

int main(){
    struct termios tty; //struct to access termios data
    tcgetattr(STDIN_FILENO, &tty); //get attributes for the terminal
    tty.c_lflag &= ~ICANON; //set terminal to non canonical mode
    tty.c_lflag &= ~ECHO; //disable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &tty); //apply changes
    initscr(); //start game
    curs_set(0); //hide cursor
    keypad(stdscr, TRUE); //accept arrow keys as inputs without printing to the terminal  
    timeout(250); // keeps moving waitng for new input 
    int xPosition, yPosition;
    xPosition = WIDTH/2; 
    yPosition = HEIGHT/2 + 3;   //place to spawn snake
    int snakeLength = 3;
    //coordinates to spawn fruit with arena bounds
    fruitX = (rand() % WIDTH) + 1;
    fruitY = (rand() % HEIGHT) + 4;
    
    //snake head
    snakeSegmentX[0] = xPosition;
    snakeSegmentY[0] = yPosition;
    
    
    
    int moveX = 1; //default direction
    int moveY = 0;
    
    //all directional arrow keys are eligible to be pressed
    bool kdown = true;
    bool kup = true;
    bool kleft = false; //starting direction is moving right
    bool kright = true;
    
    
    while(true){
        int keyPressed = getch(); //get input
        if(keyPressed == KEY_UP && (kup == true)){
            kdown = false;
            kleft = true;
            kright = true;
            moveX = 0;
            moveY = -1;
        }
        if(keyPressed == KEY_DOWN && (kdown == true)){
            kup = false;
            kleft = true;
            kright = true;
            moveX = 0;
            moveY = 1;
        }
        if(keyPressed == KEY_LEFT && (kleft == true)){
            kright = false;
            kup = true;
            kdown = true;
            moveX = -1;
            moveY = 0;
        }
        if(keyPressed == KEY_RIGHT && (kright == true)){
            kleft = false;
            kup = true;
            kdown = true;
            moveX = 1;
            moveY = 0;
        }
        xPosition += moveX;
        yPosition += moveY;
        
        if(collision(xPosition, yPosition, snakeLength)){
            lose = true; // Game over screen 
            break;
        }if(snakeLength >= 100){
            win = true; // Winner winner!
            break;
        }
        clear();
        
        char title[] = "SNAKE GAME";
        mvprintw(1, 2, "Score: %d ", score); //displays score in the corner left
        mvprintw(1,(WIDTH+2 - strlen(title))/2, "%s", title); // displays title in the middle
        
        for(int x = 0; x <= WIDTH +1; x++){
            mvaddch(3, x, '*');// top wall
            mvaddch(HEIGHT + 4, x, '*');// bottom wall
        }
        for(int y = 3; y<= HEIGHT+4; y++){
            mvaddch(y, 0, '*');// left wall
            mvaddch(y, WIDTH + 1, '*');// right wall
        }

        generateFruit(xPosition,yPosition, &snakeLength);
        slither(xPosition,yPosition, snakeLength);
        refresh();

        if(snakeLength >= 8 && snakeLength < 18){ //snake movement speedup at size 8
            timeout(175);
        }
        if(snakeLength >= 18){
            timeout(100);
        }

    }
    clear();
    if(lose){
        mvprintw(LINES/2, COLS/2 - 5, "GAME OVER");// game over message in the center 
    }if(win){
        mvprintw(LINES/2, COLS/2 - 4, "YOU WIN!");// win message in the center
    }
    refresh();
    timeout(-1);
    getch();
    endwin();
    return 1;
}


void slither(int xPosition, int yPosition, int snakeLength){
    int prevX = snakeSegmentX[0];
    int prevY = snakeSegmentY[0];

    int tempX, tempY;

    // move head
    snakeSegmentX[0] = xPosition;
    snakeSegmentY[0] = yPosition;

    // move snake in array
    for(int i = 1; i < snakeLength; i++){
        //standard temp swapping loop
        //get coordinates for a snake segment behind head and store in temp
        tempX = snakeSegmentX[i];
        tempY = snakeSegmentY[i];

        //move snake segment to coordinates of the segment it follows
        snakeSegmentX[i] = prevX;
        snakeSegmentY[i] = prevY;

        //look at the next snake segment
        prevX = tempX;
        prevY = tempY;

    }

    // print snake to screen
    for(int i = 0; i < snakeLength; i++){ 
        if(i == 0){ //if starting the loop
            mvaddch(snakeSegmentY[i], snakeSegmentX[i], '@'); // print head
        } else {
            mvaddch(snakeSegmentY[i], snakeSegmentX[i], 'o'); // print body
        }
    }
}

void generateFruit(int xPosition, int yPosition, int *snakeLength){
    //spawns in fruit
    while(!checkFruit){ //loop used to bruteforce a random fruit placement
        //find a random x/y coordinates in game field
        fruitX = (rand() % WIDTH) + 1;  
        fruitY = (rand() % HEIGHT) + 4;
        //get the character from a location on screen, to see if the fruit will spawn inside the snake
        chtype ch = mvinch(fruitY, fruitX);
        char charAt = ch & A_CHARTEXT;
        if(charAt != '@' && charAt != 'o'){ //if the fruit is in a blank space, place it there
            checkFruit = true; // prevent any more fruit from spawning until this one is eaten
        }
    }
    mvaddch(fruitY,fruitX,'$');
    
    if(xPosition == fruitX && yPosition == fruitY){ //if the head is on the same square as the fruit
        //snake eats fruit
        (*snakeLength)++; //increase snake length
        checkFruit = false; //another fruit can now be generated
        score+= 1; //increase score
    }
}

int collision(int xPosition, int yPosition, int snakeLength){ //function for if snake hits a wall
    if(xPosition <= 0 || xPosition >= WIDTH +1){//if snake hits left or right wall then end game
        return 1;
    }else if(yPosition <= 3 || yPosition >= HEIGHT + 4){// if snake hits top or bottom wall end game
        return 1;
    }
    for(int i = 1; i<snakeLength; i++){ //loop through each snake segment
        if(xPosition == snakeSegmentX[i] && yPosition == snakeSegmentY[i]){ //check if the head is at the same location as a part of its body
            return 1; //if it is, die
        }
    }
    return 0;
}