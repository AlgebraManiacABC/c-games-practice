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
#include "./myterm.h"
#include "./color.h"

#define FOUR 1200
#define THREE 300
#define TWO 100
#define ONE 40
//definitions for line clearing scores

#define UWAIT 5000
//useconds the game waits before continuing without input

void title(struct winsize);
    //displays the title screen
void startGame(struct winsize);
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

int main()
{
    struct winsize w;
    ioctl(STDIN_FILENO,TIOCGWINSZ,&w);

    if(w.ws_row<22)
    {
        printf("Your terminal is too short! "
                "Tetris needs at least 22 rows.\n");
        return -1;
    }
    if(w.ws_col<45)
    {
        printf("Your terminal is too narrow! "
                "Tetris needs at least 45 columns.\n");
        return -1;
    }

    setTerm(0);
    cursorc(HIDE);
    
    title(w);
    startGame(w);
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
    cursorc(SHOW);
}

void title(struct winsize w)
{
    clear(w);
    for(int i=0;i<(w.ws_row-9)/2;i++)
        printf(DOWN);

    char * space = malloc(sizeof(char) * ((w.ws_col-45)/2) + 1);

    for(int i=0;i<(w.ws_col-45)/2;i++)
        *(space+i)=' ';
    *(space+1+(w.ws_col-45)/2)='\0';

    char admin[]=TEXT_ADMIN;
    char orange[]=TEXT_ORANGE;
    char brown[]=TEXT_BROWN;
    char yellow[]=TEXT_YELLOW;
    char puke[]=TEXT_PUKE;
    char lime[]=TEXT_LIME;
    char green[]=TEXT_GREEN;
    char sky[]=TEXT_SKY;
    char blue[]=TEXT_BLUE;
    char magenta[]=TEXT_MAGENTA;
    char reset[]=COLOR_RESET;

    printf(
    "%s%s████████╗%s███████%s╗%s████████%s╗%s██████%s╗%s ██%s╗%s███████╗%s\n"
    "%s%s╚══██╔══╝%s██%s╔════╝%s╚══%s██%s╔══╝%s██%s╔══%s██%s╗%s██%s║%s██╔════╝%s\n"
    "%s%s   ██║   %s█████%s╗     %s██%s║   %s██████%s╔╝%s██%s║%s███████╗%s\n"
    "%s%s   ██║   %s██%s╔══╝     %s██%s║   %s██%s╔══%s██%s╗%s██%s║%s╚════██║%s\n"
    "%s%s   ██║   %s███████%s╗   %s██%s║   %s██%s║  %s██%s║%s██%s║%s███████║%s\n"
    "%s%s   ╚═╝   %s╚══════╝   %s╚═╝   %s╚═╝  ╚═╝%s╚═╝%s╚══════╝%s\n\n"
    
    "%s             PRESS ANY KEY TO BEGIN",
    space,admin,orange,brown,yellow,puke,lime,green,sky,blue,magenta,reset,
    space,admin,orange,brown,puke,yellow,puke,lime,green,lime,green,sky,blue,magenta,reset,
    space,admin,orange,brown,yellow,puke,lime,green,sky,blue,magenta,reset,
    space,admin,orange,brown,yellow,puke,lime,green,lime,green,sky,blue,magenta,reset,
    space,admin,orange,brown,yellow,puke,lime,green,lime,green,sky,blue,magenta,reset,
    space,admin,brown,puke,green,blue,magenta,reset,
    space);

    getchar();
}//TETRIS = 6 lines, text = 1 line, 8 lines total including space.

void startGame(struct winsize w)
{
    clear(w);
    printf("\n\n\n\n\n\n");
    printf(TEXT_GRAY"████████████████████████████████████"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██▀▀▀▀▀▀▀▀▀▀██"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██  "TEXT_WHITE"SCORE:"TEXT_GRAY"  ██"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██          ██"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██▀▀▀▀▀▀▀▀▀▀██"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██  "TEXT_WHITE"LEVEL:"TEXT_GRAY"  ██"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██          ██"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██▀▀▀▀▀▀▀▀▀▀██"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██  "TEXT_WHITE"LINES:"TEXT_GRAY"  ██"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██          ██"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██████████████"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██▀▀▀▀▀▀▀▀▀▀██"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██   "TEXT_WHITE"NEXT"TEXT_GRAY"   ██"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██          ██"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██          ██"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██          ██"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██          ██"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██          ██"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██          ██"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██▄▄▄▄▄▄▄▄▄▄██"COLOR_RESET"\n"\
           TEXT_GRAY"██                    ██████████████"COLOR_RESET"\n"\
           TEXT_GRAY"████████████████████████████████████"COLOR_RESET);
    printf(UP LEFT LEFT LEFT LEFT LEFT LEFT LEFT LEFT LEFT LEFT LEFT LEFT);
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
        printf(UP);
    printf("  "\
           "%6d",score);
    printf(DOWN DOWN DOWN LEFT LEFT LEFT
           "%2d",level);
    printf(DOWN DOWN DOWN LEFT LEFT LEFT LEFT
           "%4d",lines);
    printf(DOWN DOWN DOWN DOWN DOWN LEFT LEFT LEFT LEFT LEFT LEFT);

    for(int r=0;r<4;r++)
    {
        for(int c=0;c<4;c++)
            printBlock(next[r][c]);
        printf(LEFT LEFT LEFT LEFT LEFT LEFT LEFT LEFT DOWN);
    }

    printf(DOWN DOWN LEFT LEFT);
}

void updateBlocks(int f[][10])
{
    for(int i=0;i<20;i++)
        printf(UP);

    for(int r=2;r<=21;r++)
    {
        printf("\r"DOWN RIGHT RIGHT);
        for(int c=0;c<10;c++)
            printBlock(f[r][c]);
    }
    printf(RIGHT RIGHT);
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
            printf(TEXT_AQUA"██"COLOR_RESET);
        break;
        case 2://   O
        case -2:
            printf(TEXT_YELLOW"██"COLOR_RESET);
        break;
        case 3://   T
        case -3:
            printf(TEXT_MAGENTA"██"COLOR_RESET);
        break;
        case 4://   J
        case -4:
            printf(TEXT_BLUE"██"COLOR_RESET);
        break;
        case 5://   L
        case -5:
            printf(TEXT_ORANGE"██"COLOR_RESET);
        break;
        case 6://   S
        case -6:
            printf(TEXT_LIME"██"COLOR_RESET);
        break;
        case 7://   Z
        case -7:
            printf(TEXT_ADMIN"██"COLOR_RESET);
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
