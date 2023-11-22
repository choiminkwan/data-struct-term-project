#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define LEFT   75      // 좌측방향키
#define RIGHT  77      // 우측방향키
#define UP     72      // 위쪽방향키
#define DOWN   80      // 아래방향키
#define ENTER  13	   // 엔터
#define SPACE  32	   // 스페이스
#define MAX_CARD_SIZE 60
#define MAX_LIST_SIZE 15

void Print(void);//----------------------------------카드 섞는함수---------------------------//

//---------------------------카드정보구조체선언------------------------------//
typedef struct Cardinfo{
	int num;
	char shape;
}Cardinfo;

Cardinfo *deck;
//--------------------------------------------------------------------------//

//------------------------------스택 구조체선언-----------------------------//
typedef struct{
	Cardinfo item[MAX_CARD_SIZE];			//stack -> Cardinfo item
	int top;
}CardStack;

CardStack *deck2;		
CardStack *deck3;		
int turn=0;
//--------------------------------------------------------------------------//

//-----------------------------커서,gotoxy함수-----------------------------//
void removeCursor(void) // 커서를 안보이게한다
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible=0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void gotoxy(int x, int y) // 커서 이동
{
	COORD Cur;
	Cur.X=x;
	Cur.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Cur);
}

void print_win() {
	gotoxy(20,8);
	printf("■      ■    ■■■    ■    ■");
	gotoxy(20,9);
	printf("■  ■  ■      ■      ■■  ■");
	gotoxy(20,10);
	printf("■  ■  ■      ■      ■ ■ ■");
	gotoxy(20,11);
	printf(" ■ ■ ■       ■      ■  ■■");
	gotoxy(20,12);
	printf("  ■■■      ■■■    ■    ■");
	gotoxy(30,13);
	printf("Pleas any key~");
}

void print_lose() {
	gotoxy(20,8);
	printf("■        ■     ■■   ■■■");
	gotoxy(20,9);
	printf("■      ■  ■  ■      ■    ");
	gotoxy(20,10);
	printf("■     ■    ■  ■■   ■■■");
	gotoxy(20,11);
	printf("■      ■  ■      ■  ■    ");
	gotoxy(20,12);
	printf("■■■    ■     ■■   ■■■");
	gotoxy(30,13);
	printf("Pleas any key~");
}
//--------------------------------------------------------------------------//

//---------------------------------스택선언---------------------------------//
void init(CardStack *s)
{                                                
	s->top = -1;			// top변수는 스택에 아무런데이터가 없으면 -1값을 가진다.
}

int is_empty(CardStack *s)
{
	return (s->top == -1);	// top변수는 스택에 아무런데이터가 없으면 -1값을 가진다.
}

int is_full(CardStack *s)
{
	return (s->top == (MAX_CARD_SIZE-1));	// 더이상 삽입불가
}

void push(CardStack *s, Cardinfo item)			//스택에 새로운 요소를 삽입
{
	if(is_full(s))								// push()에서 스택에 새로운 요소를 삽입하기전에 필요한 것은											// is_full을 호출하여 스택이 가득차지 않았나 검사하는 것이다. 
	{
		fprintf(stderr,"스택 포화 에러\n");		
		return;
	}
	else
		s->item[++(s->top)] = item;

}

Cardinfo pop(CardStack *s)						//	스택에서 하나의 요소를 제거
{												//	top이 가르키는 요소를 스택에서 꺼내어 외부로 건제주는 연산
	if(is_empty(s))								//  제거하기전에 요소가 비어있는지를 검사 is_empty()
	{
		fprintf(stderr, "스택 공백 에러\n");	//  스택이 비어있으면 오류메세지 출력
		exit(1);
	}
	else return s->item[(s->top)--];				// 비어있지 않으면 top이 가리키는 값을 반환하고 top을 하나 감소시킨다.
}

Cardinfo peek(CardStack *s)						
{
	if(is_empty(s))
	{
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->item[s->top];
}
//--------------------------------------------------------------------------//
typedef struct {
	Cardinfo list[MAX_LIST_SIZE];	  // 배열 정의
	int length;		  // 현재 배열에 저장된 항목들의 개수
} ArrayListType;
//--------------------------------------------------------------------------//

void init_list(ArrayListType *l)
{
	l->length = 0;
}

int is_empty_list(ArrayListType *l)
{
	return l->length == 0;
}


int is_full_list(ArrayListType *l)
{
	return l->length == MAX_LIST_SIZE;
}

void card_charge(void)
{
	Cardinfo tmp;
	tmp=pop(deck3);
	while(deck3->top>=0)
	{
		push(deck2,pop(deck3));
		gotoxy(35,15); printf("카드가 다시 섞였습니다.");
	}
	push(deck3,tmp);
}

Cardinfo delete_list(ArrayListType *l, int position)
{
	int i;
	Cardinfo item;
	//-----------------------------------추가---------------------------------
	if(l->length==1 && turn == 0)
	{
		system("cls");
		print_win();
		exit(1);
	}
	else if(l->length==1 && turn == 1)
	{
		system("cls");
		print_lose();
		exit(1);
	}
	//------------------------------------------------------------------------
	item = l->list[position];
	for(i=position; i<(l->length-1);i++)
		l->list[i] = l->list[i+1];
	l->length--;
	return item;
}

ArrayListType you;
ArrayListType computer;
//----------------------------------카드 섞는함수---------------------------//
void shuffle(Cardinfo *deck)
{
	int i,j,n=0;
	int tmp;
	char tempshape;
	
	for(i=0; i<52; i++)
		srand((unsigned)time(NULL));	//랜덤함수 쓰기위해 사용
	for(n=0; n<100; n++){
		i=rand()%52;		//0~52까지섞는다
		j=rand()%52;
	
		
		tmp = deck[i].num;
		deck[i].num = deck[j].num;
		deck[j].num = tmp;

		tempshape = deck[i].shape;
		deck[i].shape = deck[j].shape;
		deck[j].shape = tempshape;
	
	}
}
//---------------------------------------------------------------------//

//---------------------------카드 초기화함수---------------------------//
void card_init()
{
	int i,j;
	char shape;
	deck = (Cardinfo *)malloc(sizeof(Cardinfo)*52);	//포인터 주소만있기때문에 동적할당
		for(i=0; i<52; i++)
		{
		deck[i].num = (i%13)+1; // % 나머지
		if(i/13 == 0)
			shape = 'h';
		else if(i/13 == 1)
			shape = 'd';
		else if(i/13 == 2)
			shape = 'c';
		else if(i/13 == 3)
			shape = 's';
		deck[i].shape = shape;
		}
		shuffle(deck);
		deck2 = (CardStack *)malloc(sizeof(CardStack));		
		init(deck2);
		
		for(i=0; i<52; i++)
		{
			push(deck2,deck[i]);
		}
}
//----------------------------------------------------------------------//

//---------------------------게임 초기화함수----------------------------//
void Game_init()
{
	int i=0;
	init_list(&you);
	init_list(&computer);
	card_init();
	for(i=0; i<5; i++)
	{
		you.list[you.length++] = pop(deck2);
		computer.list[computer.length++] = pop(deck2);
	}
	deck3 = (CardStack *)malloc(sizeof(CardStack));
	init(deck3);
	push(deck3, pop(deck2));//한장
}
//---------------------------------------------------------------------//
void Card_Attack(ArrayListType * u, Cardinfo a)
{
	if(a.num == 2)			// 숫자2 == 카드2장받기
	{
		u->list[u->length++]=pop(deck2);
		u->list[u->length++]=pop(deck2);
	}
	if(a.num == 1)			// A == 카드3장받기
	{
		u->list[u->length++]=pop(deck2);
		u->list[u->length++]=pop(deck2);
		u->list[u->length++]=pop(deck2);
	}
	if(a.num == 11 || a.num == 13)
	{
		if(turn==1)
			turn=0;
		else
			turn=1;
	}
}
//---------------------------카드 내용출력함수-------------------------//
void Print2()   
{
   
   int i=0;
   for(i=0;i<you.length; i++)
   {
	   if(you.list[i].shape=='d')
	   {
		  gotoxy(8+(i*4),18);   printf("◆",you.list[i].shape);
	   }
	   else if(you.list[i].shape=='c')
	   {
		   gotoxy(8+(i*4),18);   printf("♣",you.list[i].shape);
	   }
	   else if(you.list[i].shape=='s')
	   {
		   gotoxy(8+(i*4),18);   printf("♠",you.list[i].shape);
	   }
	   else if(you.list[i].shape=='h')
	   {
		   gotoxy(8+(i*4),18);   printf("♥",you.list[i].shape);
	   }

	  if(you.list[i].num == 1){
         gotoxy(8+(i*4),19);  printf("A ");
      }
      else if(you.list[i].num == 11){
         gotoxy(8+(i*4),19); printf("J ");
      }
      else if(you.list[i].num == 12){
         gotoxy(8+(i*4),19); printf("Q ");
      }
      else if(you.list[i].num == 13){
         gotoxy(8+(i*4),19); printf("K ");
      }
      else {
         gotoxy(8+(i*4),19); printf("%d ", you.list[i].num);
      }
   }
    
   for(i=you.length;i<15; i++)
   {
	  gotoxy(8+(i*4),18);  printf("  ");
	  gotoxy(8+(i*4),19);  printf("  ");
   }
   
   for(i=0; i<computer.length; i++)
   {
	  gotoxy(8+(i*4),3); printf("%c",computer.list[i].shape);
	   gotoxy(8+(i*4),4); printf("%d",computer.list[i].num);
	   //gotoxy(8+(i*4),3); printf("?");
	  // gotoxy(8+(i*4),4); printf("?");
   }
   
   for(i=computer.length;i<15; i++)
   {
	  gotoxy(8+(i*4),3);  printf("  ");
	  gotoxy(8+(i*4),4);  printf("  ");
   }
   
   	   if(peek(deck3).shape=='d')
	   {
		    gotoxy(32,10); printf("◆",peek(deck3).shape);
	   }
	   else if(peek(deck3).shape=='c')
	   {
		    gotoxy(32,10); printf("♣",peek(deck3).shape);
	   }

	   else if(peek(deck3).shape=='s')
	   {
		    gotoxy(32,10); printf("♠",peek(deck3).shape);
	   }
	   else if(peek(deck3).shape=='h')
	   {
		    gotoxy(32,10); printf("♥",peek(deck3).shape);
	   }
   gotoxy(32,11); printf("%d ",peek(deck3).num);
   if(peek(deck3).num == 1){
	gotoxy(32,11); printf("A ");
   }
   else if(peek(deck3).num == 11){
	   gotoxy(32,11); printf("J ");
   }
   else if(peek(deck3).num == 12){
	   gotoxy(32,11); printf("Q ");
   }
   else if(peek(deck3).num == 13){
	   gotoxy(32,11); printf("K ");
   }
}
//---------------------------------------------------------------------------//
void card_select(ArrayListType *u)
{
	if(is_full_list(u))
	{
		//printf("꽉참");
		system("cls");
		print_lose();
		exit(1);
	}
	else
	{
		u->list[u->length++]=pop(deck2);
		Print2();		//카드받아오고 출력
	}
}

void card_throw(int position)
{
	if((you.list[position].num == peek(deck3).num) || (you.list[position].shape == peek(deck3).shape))
	{
		
		push(deck3,delete_list(&you,position));
		Card_Attack(&computer,peek(deck3));
		Print();		// 낼 수 없는 카드를 없앰
		Print2();
		turn=1;
		
	}
	else
	{
		
		gotoxy(32,15); printf("낼 수 없는 카드입니다.");
		fflush(stdin);	//엔터를 누르면 해당변수로 매칭, 이 버퍼를 지운다.
		
	}
}

void card_throw_c(void)
{
	int i=0;
	for(i=0; i<computer.length; i++)
	{
		if((computer.list[i].num == peek(deck3).num) || (computer.list[i].shape == peek(deck3).shape))
		{			
			
			push(deck3,delete_list(&computer,i));
			Card_Attack(&you,peek(deck3));
			Print2();
			turn=0;
			break;
		}
	}
	if(turn==1)		//낼 카드가 없을시에
	{
		computer.list[computer.length++]=pop(deck2);
		Print2();
		turn=0;
	}
}

//------------------------게임화면 출력함수---------------------------------//
void Print() // 게임화면 출력
{
	int i=0,j=0;
	removeCursor(); // 커서를 없앰
	system("cls"); // 화면 클리어
	gotoxy(0,0);
	printf(" ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	
	
	//gotoxy(70,1); printf("1093060");
	//gotoxy(70,2); printf(" 박상규");
	//gotoxy(55,8); printf("총 남은 카드 :  장");
	//gotoxy(55,10); printf("남은 카드 :  장 ");
	//gotoxy(55,12); printf("차례 :  ");
	//gotoxy(55,14); printf("모양 :  ");
	

	for(i=0; i<15; i++)
	{
		gotoxy(6+(i*4),2); printf("┏━━┓");
		gotoxy(6+(i*4),3); printf("┃    ┃");
		gotoxy(6+(i*4),4); printf("┃    ┃");
		gotoxy(6+(i*4),5); printf("┗━━┛");	
	}
	for(i=0; i<15; i++)
	{
		gotoxy(6+(i*4),17); printf("┏━━┓");
		gotoxy(6+(i*4),18); printf("┃    ┃");
		gotoxy(6+(i*4),19); printf("┃    ┃");
		gotoxy(6+(i*4),20); printf("┗━━┛");
	}	
	
	gotoxy(30,9); printf("┏━━┓");
	gotoxy(30,10); printf("┃    ┃");
	gotoxy(30,11); printf("┃    ┃");
	gotoxy(30,12); printf("┗━━┛");

	gotoxy(40,9); printf("┏━━┓");
	gotoxy(40,10); printf("┃    ┃");
	gotoxy(40,11); printf("┃    ┃");
	gotoxy(40,12); printf("┗━━┛");
	
}
//----------------------------------------------------//
//-------------------게임 메뉴함수--------------------//
int MENU()  //게임 메뉴
{
	int key, menu=1;
	system("cls"); // 화면 클리어
	gotoxy(0,0); 
	printf(" ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┃                                                                          ┃\n");
	printf(" ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	gotoxy(70,1); printf("1093060");
	gotoxy(70,2); printf(" 박상규");
	gotoxy(30,2);printf("        < 메 뉴 >          ");
	gotoxy(30,5);printf(" ┏━━━━━━━━━┓");
	gotoxy(30,6);printf(" ┃    게임 시작     ┃");
	gotoxy(30,7);printf(" ┗━━━━━━━━━┛");

	gotoxy(30,9);printf(" ┏━━━━━━━━━┓");
	gotoxy(30,10);printf(" ┃    게임 설명     ┃");
	gotoxy(30,11);printf(" ┗━━━━━━━━━┛");

	gotoxy(30,13);printf(" ┏━━━━━━━━━┓");
	gotoxy(30,14);printf(" ┃    종료 하기     ┃");
	gotoxy(30,15);printf(" ┗━━━━━━━━━┛");
	
	
	while(1)
	{
		removeCursor(); // 커서를 없앰
		gotoxy(28,(menu*4)+2);//선택한 메뉴의 왼쪽에 ☞를 출력
		printf("☞");
		key=_getch();
		gotoxy(28,(menu*4)+2);//☞자리에 빈칸을 채워서 ☞모양을 없앰
		printf(" ");
		switch(key)
		{
			case UP : menu--; // 화살표 위를 받았을 경우
				if(menu<=0)
					menu=3;
				break;

			case DOWN : menu++; // 화살표 아래를 받았을 경우 
				if(menu>=4)
					menu=1;
				break;

			case ENTER : return menu; // 엔터를 누른경우
		}
	}
}
//----------------------------------------------------//
//------------------- 도움말 함수 --------------------//
void help() // 도움말 출력
{
	gotoxy(2,1); printf("☞ 원카드게임\n");
	gotoxy(2,3); printf("1. 카드는 총 54장이다. ♣,♥,◆,♠ = A~10,J,Q,K 52장 JOKER 2장\n");
	gotoxy(2,4); printf("2. 앞 사람이 낸 숫자에 이어서 같은 모양이나 같은 숫자의 카드를 내면\n"); 
	gotoxy(2,5); printf("   되는 게임이다. [예 : ♠->♠, 10 -> 10 ]\n");
	gotoxy(2,6); printf("3. play는 컴퓨터와 1:1 대결구도 \n");
	gotoxy(2,9); printf("☞ 게임 규칙\n");
	gotoxy(2,11); printf("1. 게임시작시 랜덤카드를 5장씩 나눠가지고 1장을 따로 중앙에 배치한다.\n");
	gotoxy(2,12); printf("2. 같은 숫자나 모양 카드를 낼 수 있다. 낼 것이 없을 시에는 한장을 받는다.\n");
	gotoxy(2,13); printf("3. J- 턴점프 , K- 한번더 , Q- 순서바꿈\n");
	gotoxy(2,14); printf("4. 7 - (♣,♥,◆,♠) 모양 바꿈\n");
	gotoxy(2,15); printf("5. ♣2,♥2,◆2,♠2 < ♣A,♥A,◆A < ♠A \n");
    gotoxy(2,16); printf("6. 공격을 막으려면 같은무늬나 더 쎈 카드로 방어\n");
	gotoxy(2,17); printf("7. 카드를 먼저 없애는 쪽이 '승리'\n");
	gotoxy(2,18); printf("8. 카드가 20장이 넘는 쪽이 '패배'\n");
	gotoxy(2,22);printf("-아무키를 누르면 목차화면으로 돌아갑니다-\n");
	removeCursor();
}
//----------------------------------------------------//
//------------------------게임함수--------------------//
void Game()
{
	int key, onbar=1;
	int i=0;
	Print();
	Game_init();
	Print2();

	
	
	while(1)
	{
		if(deck2->top<8)
		{
			card_charge();
		}
		if(turn==0)
		{
			removeCursor(); // 커서를 없앰
			gotoxy((onbar*4)+2,21); 
			printf(" △");
			key=_getch();
			gotoxy((onbar*4)+2,21);
			printf("  ");
			switch(key)
			{
			
				case UP:
					gotoxy(41,13);
					printf(" △");
					key=_getch();
					if(key==ENTER)
					{
						card_select(&you);
						turn=1;
					}
					gotoxy(41,13); printf("  ");
					break;
				
				case LEFT: onbar--;		//화살표 왼쪽을 받았을 경우
					if(onbar<=0)
						onbar=15;
					break;
				case RIGHT: onbar++;		//화살표 오른쪽을 받았을 경우
					if(onbar>=16)
						onbar=1;
					break;
				case DOWN:
					onbar=1;
					break;
				case ENTER:
						card_throw(onbar-1);
						
			}
		}
		else if(turn==1)
		{
			card_throw_c();
		}
	}
}

//------------------------------------------------------//

//------------------메뉴 초이스 함수--------------------//
void choice()
{
	int menu;
	while(1)
	{
		menu=MENU();
		if(menu==1)
		{
			system("cls");
			Game();
			getchar();		//입력받을때까지 기다림
		}
		if(menu==2)
		{
			system("cls");
			help();
			getchar();
		}
		if(menu==3)
		{
			system("cls");
			exit(0);
		}
	}
}
	
//----------------------------------------------------//

//---------------------메인 함수----------------------//
int main()
 {
	choice();
}
//----------------------------------------------------//
