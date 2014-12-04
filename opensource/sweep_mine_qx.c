#include<stdio.h>
#include<stdlib.h>

int N;                  //棋盘矩阵行列数
int M;                  //地雷个数

typedef struct Chess 
{
    int x;              //棋盘坐标
    int y;
    char usrchess;      //用户操作后的棋盘
    char mine;          //隐藏的实际棋盘
    struct Chess* next;
}Chess;

Chess* chessHead;        //棋盘链表头

void initPanel();

void userChoice();

void check();

void paintChess();

void initRandomMine();

void initChess();

void showChess();

void main()
{
    chessHead = malloc(sizeof(Chess));
    initPanel();

    showChess(); //test
  /*  while(1) {

         userChoice();
         check();
    } */ 
}

/*
初始化棋盘
*/
void initPanel()
{
    printf("请输入棋盘大小(行数):");
    scanf("%d",&N);

    printf("请输入地雷个数:");                                                                             
    scanf("%d",&M); 

    paintChess();

    initChess();

    initRandomMine();
    return;
}

void userChoice()
{
    return;
}

void check()
{
    return;
}

/*
绘制棋盘
*/
void paintChess() {
    int i=0;
    for(i=0;i<N*N;i++) {
       printf("%-3s","+"); 
       if((i != 0) && ((i+1)%N == 0)) {
           printf("\n");
       }
    }

    return;
}

/*
产生地雷
*/
void initRandomMine() {
    int a=0;
    int count = M;
    int mineIndex;
    int i=0;
    Chess* node = chessHead->next;

    while(count>0) {
       srand(time(NULL));
       a = rand();
       mineIndex = a%(N*N); 
       for(i=0; i<mineIndex; i++) {
           if(i!=mineIndex -1) {
               node->mine = 'O';
               node = node->next;
           } else {
               if(node->mine != 'X') {
                   node->mine = 'X';
                   count--;
               }
           }
       }  
    }
}

void initChess() {
   int i=0;
   int j=0;
   Chess* node;
   Chess* tempNode;

   tempNode = chessHead;
 
   for(i=0;i<N;i++) {
      for(j=0;j<N;j++) {
          node = malloc(sizeof(Chess));
          node->x = i;
          node->y = j;
          node->usrchess = 'O';
          node->mine = 'O';          
          node->next = NULL;

          tempNode->next = node;
          tempNode = node; 
      } 
   } 

}

void showChess() {
   int i=0;
char a='a';
   Chess* node;

   node = chessHead->next;

system("clear");
   for(i=0; i<N*N; i++) {

       printf("%-3c",node->mine);

       if((i+1)%N ==0) {
           printf("\n");
       }
       node = node -> next;
   }
}
