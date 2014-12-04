/*sweep_mine.c
程序功能：在Linux终端通过输入每个小格子的坐标和flag来模拟Window平台自带的扫雷小游戏
作者：张峰 [mailto:frankzhang02010@gmail.com]
修改日期：2013-12-6
------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define Symbol 'x'          //打印雷的符号
 
int M,N;                    //M:用户设置雷的个数，N:用户输入棋盘的大小（N*N）
 
typedef struct node{
    int x;                  //节点映射到二维数组上的行坐标
    int y;                  //节点映射到二维数组上的列坐标
    char chess;            //存放棋盘节点的值
    char mine;              //存放雷盘节点的值
    struct node* next;      //节点的指针域
}Node;
 
Node *head;             //存放雷盘和棋盘及二维数组坐标的结构体的头指针
 
/*----------------------------------------------------------
set_size():让用户设置N和M
title():用来实时显示棋盘大小和当前雷的数量
panel_init():初始化棋盘，让其刚开始时全部显示为‘*’
update_panel():将更新后的棋盘打印出来
-----------------------------------------------------------*/
void set_size(),title(),panel_init(),update_panel();
/*---------------------------------------------------------
get_chess():返回棋盘(x,y)节点的棋子的值
set_chess():设置棋盘(x,y)节点的值为ch
get_mine():返回雷盘(x,y)节点的棋子的值
set_mine():设置雷盘(x,y)节点的值为ch
---------------------------------------------------------*/
char get_chess(int x,int y);
void set_chess(int x,int y,char ch);
char get_mine(Node* head,int x,int y);
void set_mine(Node* head,int x,int y,char ch);
/*-------------------------------------------------------
free_all():将使用完的链表的所有节点内存空间释放
mine_assign():让雷盘上N*N个点随机产生M个雷，并计算出所有非雷点相邻8个点雷的数量
show_mine():将雷盘打印出来
check():判断棋盘和雷盘是否相等，若相等则玩家胜出
---------------------------------------------------------*/
void free_all(Node* head);
void mine_assign(),show_mine(),check();
/*-------------------------------------------------------
zero_open():当玩家输入的坐标刚好为周围一个雷都没有时，则用递归方式将其周围的非雷点都显示出来
---------------------------------------------------------*/
void zero_open(int x,int y);
 
int main()
{   
    int x,y,flag;               //x:存放用户输入的x坐标  y:存放用户输入的y坐标    flag:存放用户输入的选择，0为该点非雷，1为该点是雷
    char ch;
    set_size();
    title();
    panel_init();
    mine_assign();
        
    while(1)
    {
        printf("[(x,y)_flag]>");
        scanf("%d,%d %d",&x,&y,&flag);
        if(1>x || x>N || 1>y || y>N || 0>flag || flag>1)        //若用户输入的坐标值超出棋盘范围，则给出提示并重新输入
        {
            printf("超出范围,请重新输入\n");
            sleep(1);
            continue;
        }
        system("clear");
        
        x=x-1;y=y-1;                                            //将用户习惯的坐标转换为与二维数组对应的坐标
        ch=get_mine(head,x,y);                                  //根据用户输入的坐标值，获取雷盘对应位置的值
        if(ch!='-' && ch!=Symbol && flag==0)                    //根据输入坐标对应的雷盘值来决策玩家是否胜出
        {
            set_chess(x,y,ch);
            update_panel();
        }
        else if(ch==Symbol && flag==1)
        { 
            M--;                                              //该点为雷，玩家找出雷点，雷的数量减1
            set_chess(x,y,ch);
            update_panel();
        }
        else if(ch==Symbol && flag==0)
        {
            show_mine();
            printf("Game Over\n");
            exit(-1);
        }
        else if(ch=='-' && flag==0)
        {
            zero_open(x,y);
            update_panel();
        }
        else
        {
            M--;                                            //如果玩家将flag设置为1,表示玩家认为该点为雷，则雷的数量减1
            set_chess(x,y,Symbol);
            update_panel();
        }
        check();
    }
 
    return 0;
}
 
/*
函数功能:让用户输入二维棋牌的大小和布雷的个数
参数：N为棋盘的行和列数（N*N），M为雷的个数
*/
void set_size()    
{
    printf("棋盘大小_雷的个数>");
    scanf("%d %d",&N,&M);
    system("clear"); 
}
 
/*
函数功能:在棋盘顶部显示棋盘大小和布雷的个数
参数：N为棋盘的行和列数（N*N
*/
void title()
{
    int i;
    for(i=0;i<3*(N-1)+1;i++)    printf("-");
    printf("\n");
    for(i=0;i<(3*N-22)/2;i++) printf(" ");
    printf("棋盘:%d*%d 雷的个数:%d\n",N,N,M);    
    for(i=0;i<3*(N-1)+1;i++)    printf("-");
    printf("\n");
}
 
/*
函数功能:初始化棋盘为N*N个‘*’
参数：N为棋盘的行和列数（N*N）
c_panel[N][N]:为全局变量，存放棋盘各点的字符
*/
void panel_init()
{
    int i,j;
    Node *p,*q;
	for(i=0;i<N;i++)
	{	for(j=0;j<N;j++)
		{
		    p=(Node*)malloc(sizeof(Node));
		    if(!p)  return;
		    p->x=i;
		    p->y=j;
		    p->chess='*';
		    printf("%-3c",p->chess);
		    
		    if(!head)
		    {
		        head=q=p;
		        head->next=NULL;
		    }
		    else
		    {
		        q->next=p;
		        q=p;
		        q->next=NULL;
		    }
		}
		printf("\n");
	}
	 for(i=0;i<3*(N-1)+1;i++)    printf("-");
    printf("\n");
}
 
/*
函数功能:当玩家输入坐标后根据雷盘来更新棋盘
参数：N为棋盘的行和列数（N*N）
*/
void update_panel()
{	
	Node* p;
	p=head;
	title();
	while(p)
	{
	    printf("%-3c",p->chess);
	    if(N-1==p->y) printf("\n");
	    p=p->next;
	}
	int i;
	for(i=0;i<3*(N-1)+1;i++)    printf("-");
    printf("\n");
}
 
char get_chess(int x,int y)
{
    Node* p=head;
    while(p)
    {
        if((x==p->x) && (y==p->y))
            return p->chess;       
        p=p->next;
    }    
}
 
void set_chess(int x,int y,char ch)
{
    Node* p=head;
    while(p)
    {
        if((x==p->x) && (y==p->y))
        {
            p->chess=ch; 
            break;
        }     
        p=p->next;
    }    
}
 
char get_mine(Node* head,int x,int y)
{
    Node* p=head;
    while(p)
    {
        if((x==p->x) && (y==p->y))
            return p->mine;       
        p=p->next;
    }    
}
 
void set_mine(Node* head,int x,int y,char ch)
{
   Node* p=head;
    while(p)
    {
        if((x==p->x) && (y==p->y))
        {
            p->mine=ch+48; 
            break;
        }     
        p=p->next;
    }    
}
 
void free_all(Node* head)
{
    Node* p;
    p=head;
    while(p)
    {
        free(p);
        p=p->next;
    }
}
 
/*
函数功能:根据玩家输入雷的数量来随机产生一个雷盘
参数：N为棋盘的行和列数（N*N)
g_panel[N][N]:为全局变量，存放生成的雷盘各点的字符
*/
void mine_assign()
{
/*-------创建一个动态数组存放M个雷的下标-------*/   
    int* a=(int*)malloc(M*sizeof(int));
    if(!a)
    {
        printf("内存分配失败\n");
        return;
    }
    
    int i=0,j,k;
/*-------在0～N*N内产生M个不同的下标-------*/   
    srand(time(NULL));
    a[i++]=rand()%(N*N);
    while(1)
    {
        k=rand()%(N*N);
        for(j=0;j<i;j++)
        {
            if(k==a[j]) break;
            if(j==i-1)
              { a[i]=k;i++;break;}
        }
        if(i==M)    break;
    }
/*创建一个临时链表用来辅助计算每个点的周围雷的个数*/       
    Node *head_t=NULL,*p,*q=NULL;
    for(i=0;i<N+2;i++)
	{	for(j=0;j<N+2;j++)
		{
		    p=(Node*)malloc(sizeof(Node));
		    if(!p)  return;
		    p->x=i;
		    p->y=j;
		    p->mine=0;
		    for(k=0;k<M;k++)
		    {
		        if(a[k]==(i-1)*N+j-1 && i>=1 && j>=1 && i<=N && j<=N)
		            p->mine=1;
		    }
		    
		    if(!head_t)
		    {
		        head_t=q=p;
		        head_t->next=NULL;
		    }
		    else
		    {
		        q->next=p;
		        q=p;
		        q->next=NULL;
		    }
		}
	}
/*---用临时链表来辅助计算每个点的周围雷的个数---*/
    char tmp;
    p=head_t;
    while(p)
    {
        if(p->x>=1 && p->x<=N && p->y>=1 && p->y<=N)    
        {    
                   tmp=get_mine(head_t,p->x-1,p->y-1)+
                       get_mine(head_t,p->x-1,p->y)+
                       get_mine(head_t,p->x-1,p->y+1)+
                       get_mine(head_t,p->x,p->y-1)+
                       get_mine(head_t,p->x,p->y+1)+
                       get_mine(head_t,p->x+1,p->y-1)+
                       get_mine(head_t,p->x+1,p->y)+
                       get_mine(head_t,p->x+1,p->y+1)  ;
            set_mine(head,p->x-1,p->y-1,tmp);
          }             
        p=p->next;
    }
    free_all(head_t);//辅助链表使用完毕，释放空间
/*--------将雷盘上有雷的地方置为'x'--------*/    
    p=head;
    while(p)
    {
         for(k=0;k<M;k++)
		  {
		        if(a[k]==(p->x)*N+p->y)
		            p->mine=Symbol;
  		 }
  		 if(p->mine=='0') p->mine='-';
        p=p->next;
    }
    free(a);//动态数组使用完毕，释放空间
}
 
void show_mine()
{
    Node* p;
	p=head;
	title();
	while(p)
	{
	    printf("%-3c",p->mine);
	    if(N-1==p->y) printf("\n");
	    p=p->next;
	}
	int i;
	for(i=0;i<3*(N-1)+1;i++)    printf("-");
    printf("\n");
    
}
 
void check()
{
    Node* p;
    p=head;
    while(p)
    {
        if(p->chess!=p->mine)
            return;
         p=p->next;
    }
    printf("You Win !\n");
    exit(1);
}
 
/*
zero_open():当玩家输入的坐标刚好为周围一个雷都没有时，则用递归方式将其周围的非雷点都显示出来
*/
void  zero_open(int x,int y)
{   
    if(x<0||x>=N||y<0||y>=N)                                        //如果坐标值越界则返回
        return;
        
    if(get_mine(head,x,y)=='-' && get_chess(x,y)=='*')              //如果雷盘上该点雷的个数为0并且该点用户还没有选择过,则其周围8个点肯定无雷，故用递归将其周围8个点都显示出来
    {
       set_chess(x,y,get_mine(head,x,y));
       zero_open(x-1,y-1);
    	zero_open(x-1,y);
      	zero_open(x-1,y+1);
    	zero_open(x,y-1);
    	zero_open(x,y+1);
    	zero_open(x+1,y-1);
    	zero_open(x+1,y);
    	zero_open(x+1,y+1);
    }
    else
        set_chess(x,y,get_mine(head,x,y));                      //如果雷盘上该点雷的个数不为零则只将其显示出来
    	
}
