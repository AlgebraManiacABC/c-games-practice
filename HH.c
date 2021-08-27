#include <stdio.h>
#include <math.h>
#include "./select.h"

#define SELECT BACK_SKY
#define RESULT BACK_PUKE TEXT_BLACK
#define PH 0
#define PKA 1
#define BASE 2
#define ACID 3
#define RATIO 4

void prompt(int*,char**,double*);//asks the user for the values they have
double calculate(double*,int,int);//calculates the result

int main()
{
    printf("\nThis is a program that calculates the Henderson-Hasselbalch equation for you!\n");
    printf("What value are you trying to find?\n");

    setTerm(0);
    cursorc(HIDE);

    char * selections[]={"pH","pKa","[A-]","[HA]","[A-]/[HA]"};
    int sel = makeSelection(5,SELECT,selections)-1;
    printf("%d\n",sel);

    int select=0;
    int ask[3]={-1,-1,-1};
    if(sel==PH || sel==PKA)
    {
        char * options[]={"Both","Just a ratio"};
        printf("\nDo you have both [A-] and [HA] or just a ratio?\n");
        select = makeSelection(2,SELECT,options);
        printf("%d\n",select);

        if(select==1)
            ask[1]=BASE,ask[2]=ACID;
        else if(select==2)
            ask[1]=RATIO;

        if(sel==PH)
            ask[0]=PKA;
        else
            ask[0]=PH;
    }
    else if(sel==BASE)
        ask[0]=PH,ask[1]=PKA,ask[2]=ACID;
    else if(sel==ACID)
        ask[0]=PH,ask[1]=PKA,ask[2]=BASE;
    else if(sel==RATIO)
        ask[0]=PH,ask[1]=PKA;

    double answer[5]={0};
    prompt(ask,selections,answer);
    
    printf("Finding %s...\n",selections[sel]);

    printf(RESULT"%s = %lf"COLOR_RESET"\n",
        selections[sel],calculate(answer,sel,select));

    setTerm(1);
    cursorc(SHOW);
}

void prompt(int ask[3],char** selections,double answer[5])
{
    setTerm(1);
    cursorc(SHOW);
    printf("Please enter the following values:\n");
    for(int i=0;i<3;i++)
    {
        if(ask[i]>-1)
        {
            printf("%s = ",selections[ask[i]]);
            scanf("%lf",&answer[ask[i]]);
        }
    }
    setTerm(0);
    cursorc(HIDE);
}

double calculate(double answer[5], int sel, int select)
{
    double result = 0;

    if(sel==PH && select==1)
        result = answer[PKA] + log10( answer[BASE] / answer[ACID] );
    else if(sel==PH && select==2)
        result = answer[PKA] + log10(answer[RATIO]);
    else if(sel==PKA && select==1)
        result = answer[PH] - log10( answer[BASE] / answer[ACID] );
    else if(sel==PKA && select==2)
        result = answer[PH] - log10(answer[RATIO]);
    else if(sel==BASE)
        result = pow(10,(answer[PH]-answer[PKA])) * answer[ACID];
    else if(sel==ACID)
        result = answer[BASE] / pow(10,(answer[PH]-answer[PKA]));
    else if(sel==RATIO)
        result = pow(10,(answer[PH]-answer[PKA]));

    return result;
}

// pH = pKa + log( [A-]/[HA] )
