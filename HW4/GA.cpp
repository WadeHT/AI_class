#include<stdio.h>
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#define NSIZE 10
#define RunTimes 50
#define SetNum 100
#define Plus 1
#define mutationRate 100
#define Runs 10000
#define GenerateGap 50
using namespace std;
/********data*********/
typedef struct
{
    int Hor,SlopePlus,SlopeMinus;
} Board;
typedef struct
{
    int state[NSIZE],Point;
} Set;
int BaseRate=(int)pow(NSIZE,2)+Plus;
/********data*********/
/********function********/
void Initialize(Set Population[SetNum]);
void GA(Set Population[SetNum],int runs);
void Selection(Set Parents[2],Set Population[SetNum]);
void SWAP(void *a,void *b,int SIZE);
int GetPoint(int state[]);
int MakeBoard(int state[]);
void Crossover(Set Child[2],Set Parents[2]);
void Mutation(Set Child[2]);
void Survivor(Set Population[SetNum],Set Child[2],Set All[GenerateGap+SetNum]);
void OUTput(Set Population[],int Best);
int CMP(const void *a,const void *b);
/********function********/




int main(void)
{
    Set Populatoin[SetNum];
    srand((unsigned)time(NULL));
    int attack,total=0,SuccessTimes=0;
    double Start=clock(),End;
    printf("GA:\nn=%d\n",NSIZE);
    for(int i=0; i<RunTimes; i++)
    {
        Initialize(Populatoin);
        GA(Populatoin,Runs);
        attack=BaseRate-Populatoin[0].Point;
        total+=attack;
        if(attack==0){SuccessTimes++;}
    }
    End=clock();
    printf("avg attacks=%lf avg Running Times=%lf Success rate=%lf\n",(double)(((double)total)/((double)RunTimes)),(End-Start)/RunTimes,(double)SuccessTimes/RunTimes);
}








/********function********/
void Initialize(Set Population[SetNum])
{
    for(int j=0; j<SetNum; j++)
        for(int i=0; i<NSIZE; i++)Population[j].state[i]=rand()%NSIZE;
    return;
}
void GA(Set Population[SetNum],int runs)
{

    Set Child[2],Parents[2],ChildSet[GenerateGap],All[GenerateGap+SetNum];
    int Generate=0,i;
    for(i=1; i<=runs; i++)
    {
        Selection(Parents,Population);
        Crossover(Child,Parents);
        Mutation(Child);
        ChildSet[Generate++]=Child[0];
        ChildSet[Generate++]=Child[1];
        if((Generate%GenerateGap)==0)
        {
            if(Population[0].Point==BaseRate)return;
            Survivor(Population,ChildSet,All);
            Generate=0;
        }
    }
    return;
}
void Survivor(Set Population[],Set ChildSet[],Set All[])
{
    int state[NSIZE],total=-1;

    for(int i=0; i<GenerateGap; i++)
    {
        memcpy(state,&ChildSet[i].state,sizeof(int)*(NSIZE));
        memcpy(&All[total],state,sizeof(int)*(NSIZE));
        All[total++].Point=BaseRate-GetPoint(state);
    }
    //All population CMP with child, the least GenerateGap persons will die//
    for(int i=0; i<SetNum; i++)memcpy(&All[total++],&Population[i],sizeof(Set));
    qsort(All,total,sizeof(Set),CMP);
    memcpy(Population,All,sizeof(Set)*(SetNum));
    return;
}
void Selection(Set Parents[2],Set Population[SetNum])
{
    int state[NSIZE],Total=0;
    for(int i=0; i<SetNum; i++)
    {
        memcpy(state,&Population[i].state,sizeof(int)*(NSIZE));
        Population[i].Point=BaseRate-GetPoint(state);
        Total+=Population[i].Point;
    }
    //Tournament random choose
    int Tournament[Total],rootTotal=(int)pow(Total,3/4);
    for(int i=0,k=0; i<SetNum; i++)for(int j=0; j<Population[i].Point; j++)Tournament[k++]=i;
    for(int i=0; i<rootTotal; i++)SWAP(&Tournament[(unsigned)rand()%Total],&Tournament[(unsigned)rand()%Total],sizeof(int));
    //choose Parents
    memcpy(&Parents[0],&Population[Tournament[(unsigned)rand()%Total]],sizeof(int)*(NSIZE));
    memcpy(&Parents[1],&Population[Tournament[(unsigned)rand()%Total]],sizeof(int)*(NSIZE));
    return;
}
void SWAP(void *a,void *b,int SIZE)
{
    void *t=malloc(SIZE);
    memcpy(t,a,SIZE);
    memcpy(a,b,SIZE);
    memcpy(b,t,SIZE);
    return;
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
void Crossover(Set Child[2],Set Parents[2])
{
    int Length=rand()%NSIZE,From=rand()&1;
    memcpy(&Child[0],&Parents[From].state,sizeof(int)*(Length));
    memcpy(&Child[1],&Parents[!From].state,sizeof(int)*(Length));
    memcpy(&Child[0].state[Length],&Parents[!From].state[Length],sizeof(int)*(NSIZE-Length));
    memcpy(&Child[1].state[Length],&Parents[From].state[Length],sizeof(int)*(NSIZE-Length));
    return;
}
void Mutation(Set Child[2])
{
    for(int i=0; i<NSIZE; i++)
    {
        if(!(rand()%mutationRate))Child[0].state[i]=rand()%NSIZE;
        if(!(rand()%mutationRate))Child[1].state[i]=rand()%NSIZE;
    }
    return;
}
int CMP(const void *a,const void *b)
{
    return(((Set*)b)->Point-((Set*)a)->Point);
}
void OUTput(Set Population[],int Best)
{

    for(int j=0; j<NSIZE; j++)
    {
        for(int i=0; i<NSIZE; i++)
        {
            if(j==Population[Best].state[i])printf("C ");
            else printf("+ ");
        }
        printf("\n");
    }
    return;
}
