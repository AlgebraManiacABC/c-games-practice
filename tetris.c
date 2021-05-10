/*********************************************
 *                                           *
 *      TETRIS programmed by Luke Dunn       *
 *                                           *
 *  Made for the purpose of learning about   *
 *  unbuffered input! Also some fun with     *
 *  formatting prtinf's!                     *
 *                                           *
 *  Level system, high score, and            *
 *  MUSIC have not "yet" been implemented.   *
 *  Whether or not they will ever be is      *
 *  questionable xD                          *
 *                                           *
 * *******************************************/
#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "color.h"
#define FOUR 1200
#define THREE 300
#define TWO 100
#define ONE 40
//definitions for line clearing scores
#define UWAIT 5000
//useconds the game waits before continuing without input
void setTerm(int set);
    //sets canonical/echo/cursor or not
void title();
    //displays the title screen
void startGame();
    //sets up the game screen, sets points/level to 0
int tetRand();
    //returns a random tetromino value
void rotate(int front[][10]);
    //takes in the board array and rotates the currently falling tetromino
void updateBlocks(int front[][10]);
    //takes in the board array and prints it to the window
void updateStats(int lines_i, int next[][4]);
    //takes in stat increments and the next block and updates them
void printBlock(int block);
    //takes in the block value and prints the respective block (even 0)
int step(int front[][10]);
    //takes in the board array and moves the tetromino down a step
int onBlock(int front[][10]);
    //checks the board and tells if the falling tetromino is sitting
void lock(int front[][10]);
    //locks the current tetromino in place
void newBlock(int front[][10], int next[][4]);
    //places the next block on the array and updates the next block values
void move(int front[][10],int direction);
    //moves the tetromino left or right (if possible)
int lineCheck(int front[][10],int lines[]);
    //checks if a full line exists and updates lines; returns line count
void clearLines(int front[][10],int lines[],int count);
    //clears the specified line(s)
int isGameOver(int front[][10]);
    //checks row 1 for value 8, which means Game Over
void process(int f[][10],char ch);
    //takes a char and performs actions if necessary
int kbhit()
{
    int bytesWaiting=0;
    ioctl(STDIN_FILENO,FIONREAD,&bytesWaiting);
    return bytesWaiting;
}
int main()
{
    setTerm(0);
    title();
    /*
    while(!kbhit())
    {
        printf(".");
        fflush(stdout);
        usleep(10000);
    }
    char c = getchar();
    printf("You entered %c\n",c);
    */
    startGame();
    srand(time(NULL));
    int front[22][10]={0};
    int next[4][4]={0};
    newBlock(front,next);
    updateBlocks(front);
    updateStats(0,next);
    fflush(stdout);
    //ready(front);
    char c = 0;
    int lines_i=0;
    newBlock(front,next);
    while(!isGameOver(front))
    {
        usleep(UWAIT*100);
        updateBlocks(front);
        updateStats(lines_i,next);
        lines_i=0;
        do
        {
            updateBlocks(front);
            int i = 0;
            for(i=0;i<100;i++)
            {
                usleep(UWAIT);
                //printf("%d\b",i);
                fflush(stdout);
                if(kbhit())
                {
                    c = getchar();
                    if(c=='s'||c=='S'||c==' ')
                        break;
                    else
                    {
                        process(front,c);
                        fflush(stdout);
                    }
                }
            }
            lines_i=step(front);
        } while(lines_i?(lines_i<0?lines_i++,0:0):1);
        newBlock(front,next);
    }
    printf("\r\f\f");
    setTerm(1);
}
void setTerm(int set)
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    if(set)
    {
        term.c_lflag |= (ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
        printf("\e[?25h");
    }
    else
    {
        term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
        printf("\e[?25l");
    }
}
void title()
{
    printf("\n\n\n\n\n\n\n\n\n\n\n"\
           TEXT_ADMIN"âââââââââ"BACK_YELLOW TEXT_RED"âââââââ"COLOR_RESET TEXT_ADMIN"â"TEXT_YELLOW"ââââââââ"TEXT_PUKE"â"TEXT_LIME"ââââââ"TEXT_GREEN"â"TEXT_AQUA" ââ"TEXT_SKY"â"TEXT_MAGENTA"ââââââââ"COLOR_RESET"\n"\
           TEXT_ADMIN"âââââââââ"BACK_YELLOW TEXT_RED"ââ"COLOR_RESET TEXT_ADMIN"ââââââ"TEXT_PUKE"âââ"TEXT_YELLOW"ââ"TEXT_PUKE"ââââ"TEXT_LIME"ââ"TEXT_GREEN"âââ"TEXT_LIME"ââ"TEXT_LIME"â"TEXT_AQUA"ââ"TEXT_SKY"â"TEXT_MAGENTA"ââââââââ"COLOR_RESET"\n"\
           TEXT_ADMIN"   âââ   "BACK_YELLOW TEXT_RED"âââââ"COLOR_RESET TEXT_ADMIN"â     "TEXT_YELLOW"ââ"TEXT_PUKE"â   "TEXT_LIME"ââââââ"TEXT_GREEN"ââ"TEXT_AQUA"ââ"TEXT_SKY"â"TEXT_MAGENTA"ââââââââ"COLOR_RESET"\n"\
           TEXT_ADMIN"   âââ   "BACK_YELLOW TEXT_RED"ââ"COLOR_RESET TEXT_ADMIN"ââââ     "TEXT_YELLOW"ââ"TEXT_PUKE"â   "TEXT_LIME"ââ"TEXT_GREEN"âââ"TEXT_LIME"ââ"TEXT_GREEN"â"TEXT_AQUA"ââ"TEXT_SKY"â"TEXT_MAGENTA"ââââââââ"COLOR_RESET"\n"\
           TEXT_ADMIN"   âââ   "BACK_YELLOW TEXT_RED"âââââââ"COLOR_RESET TEXT_ADMIN"â   "TEXT_YELLOW"ââ"TEXT_PUKE"â   "TEXT_LIME"ââ"TEXT_GREEN"â  "TEXT_LIME"ââ"TEXT_GREEN"â"TEXT_AQUA"ââ"TEXT_SKY"â"TEXT_MAGENTA"ââââââââ"COLOR_RESET"\n"\
           TEXT_ADMIN"   âââ   ââââââââ   "TEXT_PUKE"âââ   "TEXT_GREEN"âââ  âââ"TEXT_SKY"âââ"TEXT_MAGENTA"ââââââââ"COLOR_RESET"\n\n"\
           "             PRESS ANY KEY TO BEGIN\n\n\n\n\n\n\n\n");
    /*
    printf(BACK_ADMIN TEXT_PUKE"P/Aâââ"COLOR_RESET"\n"\
           BACK_PUKE TEXT_ADMIN"A/Pâââ"COLOR_RESET"\n"\
           BACK_ADMIN TEXT_YELLOW"Y/Aâââ"COLOR_RESET"\n"\
           BACK_YELLOW TEXT_ADMIN"A/Yâââ"COLOR_RESET"\n"\
           BACK_RED TEXT_PUKE"P/Râââ"COLOR_RESET"\n"\
           BACK_PUKE TEXT_RED"R/Pâââ"COLOR_RESET"\n"\
           BACK_RED TEXT_YELLOW"Y/Râââ"COLOR_RESET"\n"\
           BACK_YELLOW TEXT_RED"R/Yâââ"COLOR_RESET"\n");
    */
    getchar();
}
void startGame()
{
    printf("\n\n\n\n\n\n");
    printf(TEXT_GRAY"ââââââââââââââââââââââââââââââââââââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââââââââââââââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââ  "TEXT_WHITE"SCORE:"TEXT_GRAY"  ââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââ          ââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââââââââââââââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââ  "TEXT_WHITE"LEVEL:"TEXT_GRAY"  ââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââ          ââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââââââââââââââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââ  "TEXT_WHITE"LINES:"TEXT_GRAY"  ââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââ          ââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââââââââââââââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââââââââââââââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââ   "TEXT_WHITE"NEXT"TEXT_GRAY"   ââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââ          ââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââ          ââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââ          ââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââ          ââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââ          ââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââ          ââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââââââââââââââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââ                    ââââââââââââââ"COLOR_RESET"\n"\
           TEXT_GRAY"ââââââââââââââââââââââââââââââââââââ"COLOR_RESET);
    printf("\e[A\b\b\b\b\b\b\b\b\b\b\b\b");
}
//  __   ___       __      __
// |__) |__   /\  |  \ \ /  _|
// |  \ |___ /--\ |__/  |   .
//
int tetRand()
{
    return (rand()%7)+1;
}
void updateStats(int lines_i, int next[][4])
{
    static int score = 0;
    static int level = 0;
    static int lines = 0;
    lines += lines_i;
    switch(lines_i)
    {
        case 4:
            score += FOUR - (THREE + TWO + ONE);
        case 3:
            score += THREE - (TWO + ONE);
        case 2:
            score += TWO - ONE;
        case 1:
            score += ONE;
    }
    //level += level_i;
    score<0?score=0:0;
    level<0?level=0:0;
    lines<0?lines=0:0;
    for(int i=0;i<17;i++)
        printf("\e[A");
    printf("  "\
           "%6d",score);
    printf("\f\f\f\b\b\b"\
           "%2d",level);
    printf("\f\f\f\b\b\b\b"\
           "%4d",lines);
    printf("\f\f\f\f\f\b\b\b\b\b\b");
    for(int r=0;r<4;r++)
    {
        for(int c=0;c<4;c++)
            printBlock(next[r][c]);
        printf("\b\b\b\b\b\b\b\b\f");
    }
    printf("\f\f\b\b");
}
void updateBlocks(int f[][10])
{
    for(int i=0;i<20;i++)
        printf("\e[A");
    for(int r=2;r<=21;r++)
    {
        printf("\r\f\e[C\e[C");
        for(int c=0;c<10;c++)
            printBlock(f[r][c]);
    }
    printf("\e[C\e[C");
    fflush(stdout);
}
void printBlock(int block)
{
    switch(block)
    {
        case 0:
            printf("  ");
        break;
        case 1://   I
        case -1:
            printf(TEXT_AQUA"ââ"COLOR_RESET);
        break;
        case 2://   O
        case -2:
            printf(TEXT_YELLOW"ââ"COLOR_RESET);
        break;
        case 3://   T
        case -3:
            printf(TEXT_MAGENTA"ââ"COLOR_RESET);
        break;
        case 4://   J
        case -4:
            printf(TEXT_BLUE"ââ"COLOR_RESET);
        break;
        case 5://   L
        case -5:
            printf(BACK_RED TEXT_YELLOW"ââ"COLOR_RESET);
        break;
        case 6://   S
        case -6:
            printf(TEXT_LIME"ââ"COLOR_RESET);
        break;
        case 7://   Z
        case -7:
            printf(TEXT_ADMIN"ââ"COLOR_RESET);
        break;
    }
}
void newBlock(int f[][10], int next[][4])
{
    switch(next[2][2])
    {
        case 1://   I
            f[2][3]?f[2][3]=8:(f[2][3]=-1);
            f[2][4]?f[2][4]=8:(f[2][4]=-1);
            f[2][5]?f[2][5]=8:(f[2][5]=-1);
            f[2][6]?f[2][6]=8:(f[2][6]=-1);
        break;
        case 2://   O
            f[1][4]=-2;
            f[1][5]=-2;
            f[2][4]?f[2][4]=8:(f[2][4]=-2);
            f[2][5]?f[2][5]=8:(f[2][5]=-2);
        break;
        case 3://   T
            f[1][5]=-3;
            f[2][4]?f[2][4]=8:(f[2][4]=-3);
            f[2][5]?f[2][5]=8:(f[2][5]=-3);
            f[2][6]?f[2][6]=8:(f[2][6]=-3);
        break;
        case 4://   J
            f[1][4]=-4;
            f[2][4]?f[2][4]=8:(f[2][4]=-4);
            f[2][5]?f[2][5]=8:(f[2][5]=-4);
            f[2][6]?f[2][6]=8:(f[2][6]=-4);
        break;
        case 5://   L
            f[1][6]=-5;
            f[2][4]?f[2][4]=8:(f[2][4]=-5);
            f[2][5]?f[2][5]=8:(f[2][5]=-5);
            f[2][6]?f[2][6]=8:(f[2][6]=-5);
        break;
        case 6://   S
            f[1][5]=-6;
            f[1][6]=-6;
            f[2][4]?f[2][4]=8:(f[2][4]=-6);
            f[2][5]?f[2][5]=8:(f[2][5]=-6);
        break;
        case 7://   Z
            f[1][4]=-7;
            f[1][5]=-7;
            f[2][5]?f[2][5]=8:(f[2][5]=-7);
            f[2][6]?f[2][6]=8:(f[2][6]=-7);
        break;
    }
    int piece=tetRand();
    int r,c;
    for(r=0;r<4;r++)
    {
        for(c=0;c<4;c++)
            next[r][c]=0;
    }
    switch(piece)
    {
        case 1://   I
            next[2][0]=piece;
            next[2][1]=piece;
            next[2][2]=piece;
            next[2][3]=piece;
        break;
        case 2://   O
            next[1][1]=piece;
            next[1][2]=piece;
            next[2][1]=piece;
            next[2][2]=piece;
        break;
        case 3://   T
            next[1][2]=piece;
            next[2][1]=piece;
            next[2][2]=piece;
            next[2][3]=piece;
        break;
        case 4://   J
            next[1][1]=piece;
            next[2][1]=piece;
            next[2][2]=piece;
            next[2][3]=piece;
        break;
        case 5://   L
            next[1][3]=piece;
            next[2][1]=piece;
            next[2][2]=piece;
            next[2][3]=piece;
        break;
        case 6://   S
            next[1][2]=piece;
            next[1][3]=piece;
            next[2][1]=piece;
            next[2][2]=piece;
        break;
        case 7://   Z
            next[1][1]=piece;
            next[1][2]=piece;
            next[2][2]=piece;
            next[2][3]=piece;
        break;
    }
}
int step(int f[][10])
{
    if (onBlock(f))
    {
        lock(f);
        int lines[5]={0};
        int x = lineCheck(f,lines);
        if(x)
            clearLines(f,lines,x);
        else
            x--;
        return x;
    }
    else
    {
        for(int r=20;r!=-1;r--)
        {
            for(int c=0;c<10;c++)
            {
                if(f[r][c]<0)
                {
                    f[r+1][c]=f[r][c];
                    f[r][c]=0;
                }
            }
        }
        return 0;
    }
}
int onBlock(int f[][10])
{
    int blocks=0;
    for(int r=21;r>=0;r--)
    {
        for(int c=0;c<10;c++)
        {
            if(f[r][c]<0 && (f[r+1][c]>0 || r==21))
                blocks++;
        }
    }
    return blocks;
}
void lock(int f[][10])
{
    for(int r=21;r>=0;r--)
    {
        for(int c=0;c<10;c++)
        {
            if(f[r][c]<0)
                f[r][c]*=-1;
        }
    }
    printf("\a");
    fflush(stdout);
}
int isGameOver(int f[][10])
{
    int GameOver=0;
    for(int c=0;c<10;c++)
    {
        if(f[2][c]==8)
            GameOver++;
    }
    return GameOver;
}
void process(int f[][10],char c)
{
    switch(c)
    {
        case 'a':
        case 'A':
            move(f,-1);
            updateBlocks(f);
        break;
        case 'd':
        case 'D':
            move(f,1);
            updateBlocks(f);
        break;
        case 'w':
        case 'W':
            rotate(f);
            updateBlocks(f);
        break;
    }
}
void move(int f[][10],int d)
{
    for(int c=(d<0?0:9);c>=0 && c<10;c-=d)
    {
        for(int r=21;r>=0;r--)
        {
            if(f[r][c]<0)
            {
                if(c==(d<0?0:9))
                    return;
                if(f[r][c+d]>0)
                    return;
            }
        }
    }
    for(int c=(d<0?0:9);c>=0 && c<10;c-=d)
    {
        for(int r=21;r>=0;r--)
        {
            if(f[r][c]<0)
            {
                f[r][c+d]=f[r][c];
                f[r][c]=0;
            }
        }
    }
}
void rotate(int f[][10])
{
    int n[4][2]={0};
    int i=0;
    for(int c=0;c<10;c++)
    {
        for(int r=21;r>=0;r--)
        {
            if(f[r][c]<0)
            {
                n[i][0]=r;
                n[i++][1]=c;
            }
        }
    }
    switch(f[n[0][0]][n[0][1]])
    {
        case -1://  I
            if(n[0][0]==n[3][0])//horizontal
            {
                if(n[0][0]<2 || n[0][0]==21)
                    return;
                if(! (f[n[2][0]-2][n[2][1]] || f[n[2][0]-1][n[2][1]] || f[n[2][0]+1][n[2][1]]))
                {
                    f[n[2][0]-2][n[2][1]]=-1;
                    f[n[2][0]-1][n[2][1]]=-1;
                    f[n[2][0]+1][n[2][1]]=-1;
                    f[n[0][0]][n[0][1]]=0;
                    f[n[1][0]][n[1][1]]=0;
                    f[n[3][0]][n[3][1]]=0;
                }
            }
            else//vertical
            {
                if(n[0][1]<2 || n[0][1]==9)
                    return;
                if(! (f[n[1][0]][n[1][1]-2] || f[n[1][0]][n[1][1]-1] || f[n[1][0]][n[1][1]+1]))
                {
                    f[n[1][0]][n[1][1]-2]=-1;
                    f[n[1][0]][n[1][1]-1]=-1;
                    f[n[1][0]][n[1][1]+1]=-1;
                    f[n[0][0]][n[0][1]]=0;
                    f[n[2][0]][n[2][1]]=0;
                    f[n[3][0]][n[3][1]]=0;
                }
            }
        break;
        case -2://  O
        break;
        case -3://  T
            if(f[n[0][0]][n[0][1]+1]>=0)//right
            {
                if(!n[1][1] || f[n[1][0]][n[1][1]-1])
                    return;
                f[n[1][0]][n[1][1]-1]=-3;
                f[n[2][0]][n[2][1]]=0;
            }
            else if(f[n[3][0]][n[3][1]-1]>=0)//left
            {
                if(n[2][1]==9 || f[n[2][0]][n[2][1]+1])
                    return;
                f[n[2][0]][n[2][1]+1]=-3;
                f[n[1][0]][n[1][1]]=0;
            }
            else if(f[n[1][0]][n[1][1]+1]>=0)//down
            {
                if(f[n[2][0]-1][n[2][1]])
                    return;
                f[n[3][0]][n[3][1]]=0;
                f[n[2][0]-1][n[2][1]]=-3;
            }
            else if(f[n[2][0]][n[2][1]-1]>=0)//up
            {
                if(n[1][0]==21 || f[n[1][0]+1][n[1][1]])
                    return;
                f[n[0][0]][n[0][1]]=0;
                f[n[1][0]+1][n[1][1]]=-3;
            }
        break;
        case -4://  J
            if(f[n[0][0]][n[0][1]+1]>=0)//top-right
            {
                if(!n[1][1] || f[n[1][0]][n[1][1]-1] || f[n[1][0]][n[1][1]+1] || f[n[0][0]][n[0][1]+1])
                    return;
                f[n[1][0]][n[1][1]-1]=-4;
                f[n[1][0]][n[1][1]+1]=-4;
                f[n[0][0]][n[0][1]+1]=-4;
                f[n[0][0]][n[0][1]]=0;
                f[n[2][0]][n[2][1]]=0;
                f[n[3][0]][n[3][1]]=0;
            }
            else if(f[n[3][0]][n[3][1]-1]>=0)//bot-left
            {
                if(n[2][1]==9 || f[n[3][0]][n[3][1]-1] || f[n[2][0]][n[2][1]-1] || f[n[2][0]][n[2][1]+1])
                    return;
                f[n[3][0]][n[3][1]-1]=-4;
                f[n[2][0]][n[2][1]-1]=-4;
                f[n[2][0]][n[2][1]+1]=-4;
                f[n[0][0]][n[0][1]]=0;
                f[n[1][0]][n[1][1]]=0;
                f[n[3][0]][n[3][1]]=0;
            }
            else if(f[n[1][0]][n[1][1]+1]>=0)//top-left
            {
                if(!n[2][0] || f[n[2][0]+1][n[2][1]] || f[n[2][0]-1][n[2][1]] || f[n[3][0]-1][n[3][1]])
                    return;
                f[n[2][0]+1][n[2][1]]=-4;
                f[n[2][0]-1][n[2][1]]=-4;
                f[n[3][0]-1][n[3][1]]=-4;
                f[n[0][0]][n[0][1]]=0;
                f[n[1][0]][n[1][1]]=0;
                f[n[3][0]][n[3][1]]=0;
            }
            else//bot-right
            {
                if(f[n[1][0]-1][n[1][1]] || f[n[1][0]+1][n[1][1]] || f[n[1][0]+1][n[1][1]-1])
                    return;
                f[n[1][0]-1][n[1][1]]=-4;
                f[n[1][0]+1][n[1][1]]=-4;
                f[n[1][0]+1][n[1][1]-1]=-4;
                f[n[0][0]][n[0][1]]=0;
                f[n[2][0]][n[2][1]]=0;
                f[n[3][0]][n[3][1]]=0;
            }
        break;
        case -5://  L
            if(f[n[3][0]][n[3][1]-1]>=0)//top-right
            {
                if(!n[1][0] || f[n[1][0]-1][n[1][1]] || f[n[1][0]+1][n[1][1]] || f[n[2][0]+1][n[2][1]])
                    return;
                f[n[1][0]-1][n[1][1]]=-5;
                f[n[1][0]+1][n[1][1]]=-5;
                f[n[2][0]+1][n[2][1]]=-5;
                f[n[0][0]][n[0][1]]=0;
                f[n[2][0]][n[2][1]]=0;
                f[n[3][0]][n[3][1]]=0;
            }
            else if(f[n[2][0]+1][n[2][1]]>=0)//bot-left
            {
                if(f[n[1][0]-1][n[1][1]] || f[n[2][0]-1][n[2][1]] || f[n[2][0]+1][n[2][1]])
                    return;
                f[n[1][0]-1][n[1][1]]=-5;
                f[n[2][0]-1][n[2][1]]=-5;
                f[n[2][0]+1][n[2][1]]=-5;
                f[n[0][0]][n[0][1]]=0;
                f[n[1][0]][n[1][1]]=0;
                f[n[3][0]][n[3][1]]=0;
            }
            else if(f[n[2][0]-1][n[2][1]]>=0)//bot-right
            {
                if(!n[1][1] || f[n[0][0]][n[0][1]-1] || f[n[1][0]][n[1][1]-1] || f[n[1][0]][n[1][1]+1])
                    return;
                f[n[0][0]][n[0][1]-1]=-5;
                f[n[1][0]][n[1][1]-1]=-5;
                f[n[1][0]][n[1][1]+1]=-5;
                f[n[0][0]][n[0][1]]=0;
                f[n[2][0]][n[2][1]]=0;
                f[n[3][0]][n[3][1]]=0;
            }
            else
            {
                if(n[2][1]==9 || f[n[2][0]][n[2][1]-1] || f[n[2][0]][n[2][1]+1] || f[n[3][0]][n[3][1]+1])
                    return;
                f[n[2][0]][n[2][1]-1]=-5;
                f[n[2][0]][n[2][1]+1]=-5;
                f[n[3][0]][n[3][1]+1]=-5;
                f[n[0][0]][n[0][1]]=0;
                f[n[1][0]][n[1][1]]=0;
                f[n[3][0]][n[3][1]]=0;
            }
        break;
        case -6://  S
            if(f[n[0][0]-1][n[0][1]]<0)
            {
                if(!n[0][1] || f[n[0][0]+1][n[0][1]] || f[n[0][0]+1][n[0][1]-1])
                    return;
                f[n[0][0]+1][n[0][1]]=-6;
                f[n[0][0]+1][n[0][1]-1]=-6;
                f[n[1][0]][n[1][1]]=0;
                f[n[2][0]][n[2][1]]=0;
            }
            else
            {
                if(f[n[2][0]-1][n[2][0]] || f[n[1][0]][n[1][1]+1])
                    return;
                f[n[2][0]-1][n[2][1]]=-6;
                f[n[1][0]][n[1][1]+1]=-6;
                f[n[0][0]][n[0][1]]=0;
                f[n[1][0]][n[1][1]]=0;
            }
        break;
        case -7://  Z
            if(f[n[0][0]-1][n[0][1]]<0)
            {
                if(!n[0][1] || f[n[0][0]][n[0][1]+1] || f[n[1][0]][n[1][1]-1])
                    return;
                f[n[0][0]][n[0][1]+1]=-7;
                f[n[1][0]][n[1][1]-1]=-7;
                f[n[2][0]][n[2][1]]=0;
                f[n[3][0]][n[3][1]]=0;
            }
            else
            {
                if(f[n[3][0]-1][n[3][0]] || f[n[3][0]-2][n[3][1]])
                    return;
                f[n[3][0]-1][n[3][1]]=-7;
                f[n[3][0]-2][n[3][1]]=-7;
                f[n[0][0]][n[0][1]]=0;
                f[n[3][0]][n[3][1]]=0;
            }
        break;
    }
}
int lineCheck (int f[][10],int lines[])
{
    int count=0,i=0;
    for(int r=2;r<22;r++,i=0)
    {
        for(int c=0;c<10;c++)
        {
            if(f[r][c])
                i++;
        }
        if(i==10)
        {
            lines[count]=r;
            count++;
        }
    }
    return count;
}
void clearLines(int f[][10],int lines[], int count)
{
    //printf("%2d%2d%2d%2d\b\b\b\b\b\b\b\b",lines[1],lines[2],lines[3],lines[4]);
    //printf("%d\b",count);
    for(int r=0,i=0;r<22 && count;r++)
    {
        if(lines[i]==r)
        {
            for(int c=0;c<10;c++)
                f[r][c]=0;
            for(int w=r;w>=0;w--)
            {
                for(int n=0;n<10;n++)
                    f[w][n]=f[w-1][n];
            }
            count--;
            i++;
        }
    }
    updateBlocks(f);
}
