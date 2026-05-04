#include <ncurses.h> //run program with -lncurses
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//enable non canonical mode

#define WIDTH 80
#define HEIGHT 20

int collision(int xPosition,int yPosition, int snakeLength);
void slither(int xPosition,int yPosition, int snakeLength);
void generateFruit(int xPosition,int yPosition, int *snakeLength);

//global variables !!
int fruitX,fruitY; 
bool checkFruit = false;
int score = 0; // start score
int snakeSegmentX[(WIDTH*4)/2];
int snakeSegmentY[(WIDTH*4)/2];




int main(){
    struct termios tty; //struct to access termios data
    tcgetattr(STDIN_FILENO, &tty); //get attributes for the terminal
    tty.c_lflag &= ~ICANON; //set terminal to non canonical mode
    tty.c_lflag &= ~ECHO; //disable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &tty); //apply changes
    initscr(); //start game
    curs_set(0); //hide cursor
    //score was here
    keypad(stdscr, TRUE); //accept arrow keys as inputs without printing to the terminal
    timeout(150); // keeps moving waitng for new input 
    
    int xPosition, yPosition;
    xPosition = WIDTH/2; 
    yPosition = HEIGHT/2 + 3;   //place to spawn snake
    int snakeLength = 1;
    int fruitx = rand() % (WIDTH - 2) + 1;
    int fruity = rand() % (HEIGHT - 2) + 1;
    snakeSegmentX[0] = xPosition;
    snakeSegmentY[0] = yPosition;



    int moveX = 1; //default direction
    int moveY = 0;
    while(true){
        int keyPressed = getch(); //get input
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
        
        if(collision(xPosition, yPosition, snakeLength)){
            break;
        }
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
        
        generateFruit(xPosition,yPosition, &snakeLength);
        slither(xPosition,yPosition, snakeLength);
        refresh();
    }
    clear();
    mvprintw(WIDTH/ 2, HEIGHT/ 2, "GAME OVER");
    refresh();
    return 0;
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
        fruitX = (rand() % WIDTH)+ 3; //3, height+4
        fruitY = (rand() % HEIGHT+3)+ 1; //1, width+1
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
        score+= 10; //increase score
    }
}



int collision(int xPosition, int yPosition, int snakeLength){ //function for if snake hits a wall
    if(xPosition <= 0 || xPosition >= WIDTH +1){
        return 1;
    }else if(yPosition <= 3 || yPosition >= HEIGHT + 4){
        return 1;
    }
    for(int i = 1; i<snakeLength; i++){ //loop through each snake segment
        if(xPosition == snakeSegmentX[i] && yPosition == snakeSegmentY[i]){ //check if the head is at the same location as a part of its body
            return 1; //if it is, die
        }
    }
    return 0;
}