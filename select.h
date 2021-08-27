#include "myterm.h"
#include "color.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

//prints a set of selections
void printSelections(int count,int high,char* color,char** selections)
{
    for(int i=0;i<count;i++)
    {
        if(i==high-1)
            printf("%s%s"COLOR_RESET"   ",color,selections[i]);
        else
            printf("%s   ",selections[i]);
    }
    printf("\n");
}

int makeSelection(int count,char* color,char** selections)
{
    int find=0;
    int sel=1;
    while(sel)
    {
        printSelections(count,find,color,selections);
        while(!kbhit()){};
        if(kbhit())
        {
            if(kbhit()==3 && getchar()==27 && getchar()=='[')
            {
                switch(getchar())
                {
                    case 'C':
                        find<count?find++:0;
                        break;
                    case 'D':
                        find>1?find--:0;
                        break;
                }
            }
            else
            {
                if(getchar()=='\n')
                    sel=0;
            }
        }
        printf(UP"\r");
    }
    printf("\n");
    return find;
}

int makeSelectionComplex(int count,int len,char* color,...)
{
    va_list args;
    va_start(args,color);
    char ** selections = malloc(sizeof(char*) * count);
    for(int i=0;i<count;i++)
    {
        selections[i] = malloc(sizeof(char)*len);
        strcpy(selections[i],va_arg(args,char*));
    }
    va_end(args);

    int find=0;
    int sel=1;
    while(sel)
    {
        printSelections(count,find,color,selections);
        while(!kbhit()){};
        if(kbhit())
        {
            if(kbhit()==3 && getchar()==27 && getchar()=='[')
            {
                switch(getchar())
                {
                    case 'C':
                        find<5?find++:0;
                        break;
                    case 'D':
                        find>1?find--:0;
                        break;
                }
            }
            else
            {
                if(getchar()=='\n')
                    sel=0;
            }
        }
        printf(UP"\r");
    }
    printf("\n");
    return find;
}
