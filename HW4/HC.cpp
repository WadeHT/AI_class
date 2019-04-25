#include<stdio.h>
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#define NSIZE 8
#define RunTimes 30
/********data*********/
typedef struct
{
    int Hor,SlopePlus,SlopeMinus;
} Board;
/********data*********/
/********function********/
void Initialize(int state[]);
int HC(int state[]);
int GetPoint(int state[]);
void OUTput(int state[]);
int GetPoint(int state[]);
int MakeBoard(int state[]);
/********function********/
int main(void)
{
    int state[NSIZE],total=0,SuccessTimes=0,Point;
    double Start,End;
    srand((unsigned)time(NULL));
    Start=clock();
    printf("HC:\nn=%d\n",NSIZE);
    for(int i=0; i<RunTimes; i++)
    {
        Initialize(state);
        Point=HC(state);
        total+=Point;
        if(Point==0)SuccessTimes++;
        //OUTput(state);
    }
    End=clock();
    printf("avg attacks=%lf avg Running Times=%lf Success Time=%lf\n",(double)total/RunTimes,(End-Start)/RunTimes,(double)SuccessTimes/RunTimes);
}
/********function********/
void Initialize(int state[])
{
    for(int i=0;i<NSIZE;i++)state[i]=rand()%NSIZE;
    return;
}
int HC(int state[])
{
    int successor[NSIZE],next[NSIZE],i,j;
    int StatePoint=GetPoint(state);
    //get successor//
    for(i=0; i<NSIZE; i++)
        for(j=0; j<NSIZE; j++)
        {
            memcpy(successor,state,sizeof(int)*(NSIZE));
            successor[i]=j;
            if(GetPoint(successor)<StatePoint)memcpy(next,successor,sizeof(int)*(NSIZE));
        }
    //get successor//
    if(GetPoint(next)>=StatePoint)return StatePoint;
    memcpy(state,next,sizeof(int)*NSIZE);
    return HC(state);
}
int GetPoint(int state[])
{
    return MakeBoard(state);
}
int MakeBoard(int state[])
{
    int Point=0;
    Board ChessBoard[NSIZE<<2];
    memset(ChessBoard,0,sizeof(Board)*(NSIZE*3));
    for(int i=0; i<NSIZE; i++)
    {
        if(++ChessBoard[state[i]].Hor > 1)Point+=((ChessBoard[state[i]].Hor-1)<<1);
        if(++ChessBoard[state[i]-i+NSIZE].SlopePlus > 1)Point+=((ChessBoard[state[i]-i+NSIZE].SlopePlus-1)<<1);
        if(++ChessBoard[state[i]+i].SlopeMinus > 1)Point+=((ChessBoard[state[i]+i].SlopeMinus-1)<<1);
    }
    return Point;
}
