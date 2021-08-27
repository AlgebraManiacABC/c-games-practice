#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdio.h>

#define SHOW 1
#define HIDE 0

#define UP "\e[A"
#define DOWN "\e[B"
#define RIGHT "\e[C"
#define LEFT "\e[D"
#define HOME "\e[H"

int kbhit()
{
    int bytesWaiting;
    ioctl(STDIN_FILENO,FIONREAD,&bytesWaiting);
    return bytesWaiting;
}

void setTerm(int set)
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    if(set)
    {
        term.c_lflag |= (ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
    }
    else
    {
        term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
    }
}

void cursorc(int shown)
{
    if(shown)
        printf("\e[?25h");
    else
        printf("\e[?25l");
}

void clear(struct winsize w)
{
    for(int i=0;i<w.ws_row;printf("\n"),i++);
    printf(HOME);
}
