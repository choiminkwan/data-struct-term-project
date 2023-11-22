#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define LEFT   75      // ��������Ű
#define RIGHT  77      // ��������Ű
#define UP     72      // ���ʹ���Ű
#define DOWN   80      // �Ʒ�����Ű
#define ENTER  13	   // ����
#define SPACE  32	   // �����̽�
#define MAX_CARD_SIZE 60
#define MAX_LIST_SIZE 15

void Print(void);//----------------------------------ī�� �����Լ�---------------------------//

//---------------------------ī����������ü����------------------------------//
typedef struct Cardinfo{
	int num;
	char shape;
}Cardinfo;

Cardinfo *deck;
//--------------------------------------------------------------------------//

//------------------------------���� ����ü����-----------------------------//
typedef struct{
	Cardinfo item[MAX_CARD_SIZE];			//stack -> Cardinfo item
	int top;
}CardStack;

CardStack *deck2;		
CardStack *deck3;		
int turn=0;
//--------------------------------------------------------------------------//

//-----------------------------Ŀ��,gotoxy�Լ�-----------------------------//
void removeCursor(void) // Ŀ���� �Ⱥ��̰��Ѵ�
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible=0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void gotoxy(int x, int y) // Ŀ�� �̵�
{
	COORD Cur;
	Cur.X=x;
	Cur.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Cur);
}

void print_win() {
	gotoxy(20,8);
	printf("��      ��    ����    ��    ��");
	gotoxy(20,9);
	printf("��  ��  ��      ��      ���  ��");
	gotoxy(20,10);
	printf("��  ��  ��      ��      �� �� ��");
	gotoxy(20,11);
	printf(" �� �� ��       ��      ��  ���");
	gotoxy(20,12);
	printf("  ����      ����    ��    ��");
	gotoxy(30,13);
	printf("Pleas any key~");
}

void print_lose() {
	gotoxy(20,8);
	printf("��        ��     ���   ����");
	gotoxy(20,9);
	printf("��      ��  ��  ��      ��    ");
	gotoxy(20,10);
	printf("��     ��    ��  ���   ����");
	gotoxy(20,11);
	printf("��      ��  ��      ��  ��    ");
	gotoxy(20,12);
	printf("����    ��     ���   ����");
	gotoxy(30,13);
	printf("Pleas any key~");
}
//--------------------------------------------------------------------------//

//---------------------------------���ü���---------------------------------//
void init(CardStack *s)
{                                                
	s->top = -1;			// top������ ���ÿ� �ƹ��������Ͱ� ������ -1���� ������.
}

int is_empty(CardStack *s)
{
	return (s->top == -1);	// top������ ���ÿ� �ƹ��������Ͱ� ������ -1���� ������.
}

int is_full(CardStack *s)
{
	return (s->top == (MAX_CARD_SIZE-1));	// ���̻� ���ԺҰ�
}

void push(CardStack *s, Cardinfo item)			//���ÿ� ���ο� ��Ҹ� ����
{
	if(is_full(s))								// push()���� ���ÿ� ���ο� ��Ҹ� �����ϱ����� �ʿ��� ����											// is_full�� ȣ���Ͽ� ������ �������� �ʾҳ� �˻��ϴ� ���̴�. 
	{
		fprintf(stderr,"���� ��ȭ ����\n");		
		return;
	}
	else
		s->item[++(s->top)] = item;

}

Cardinfo pop(CardStack *s)						//	���ÿ��� �ϳ��� ��Ҹ� ����
{												//	top�� ����Ű�� ��Ҹ� ���ÿ��� ������ �ܺη� �����ִ� ����
	if(is_empty(s))								//  �����ϱ����� ��Ұ� ����ִ����� �˻� is_empty()
	{
		fprintf(stderr, "���� ���� ����\n");	//  ������ ��������� �����޼��� ���
		exit(1);
	}
	else return s->item[(s->top)--];				// ������� ������ top�� ����Ű�� ���� ��ȯ�ϰ� top�� �ϳ� ���ҽ�Ų��.
}

Cardinfo peek(CardStack *s)						
{
	if(is_empty(s))
	{
		fprintf(stderr, "���� ���� ����\n");
		exit(1);
	}
	else return s->item[s->top];
}
//--------------------------------------------------------------------------//
typedef struct {
	Cardinfo list[MAX_LIST_SIZE];	  // �迭 ����
	int length;		  // ���� �迭�� ����� �׸���� ����
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
		gotoxy(35,15); printf("ī�尡 �ٽ� �������ϴ�.");
	}
	push(deck3,tmp);
}

Cardinfo delete_list(ArrayListType *l, int position)
{
	int i;
	Cardinfo item;
	//-----------------------------------�߰�---------------------------------
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
//----------------------------------ī�� �����Լ�---------------------------//
void shuffle(Cardinfo *deck)
{
	int i,j,n=0;
	int tmp;
	char tempshape;
	
	for(i=0; i<52; i++)
		srand((unsigned)time(NULL));	//�����Լ� �������� ���
	for(n=0; n<100; n++){
		i=rand()%52;		//0~52�������´�
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

//---------------------------ī�� �ʱ�ȭ�Լ�---------------------------//
void card_init()
{
	int i,j;
	char shape;
	deck = (Cardinfo *)malloc(sizeof(Cardinfo)*52);	//������ �ּҸ��ֱ⶧���� �����Ҵ�
		for(i=0; i<52; i++)
		{
		deck[i].num = (i%13)+1; // % ������
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

//---------------------------���� �ʱ�ȭ�Լ�----------------------------//
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
	push(deck3, pop(deck2));//����
}
//---------------------------------------------------------------------//
void Card_Attack(ArrayListType * u, Cardinfo a)
{
	if(a.num == 2)			// ����2 == ī��2��ޱ�
	{
		u->list[u->length++]=pop(deck2);
		u->list[u->length++]=pop(deck2);
	}
	if(a.num == 1)			// A == ī��3��ޱ�
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
//---------------------------ī�� ��������Լ�-------------------------//
void Print2()   
{
   
   int i=0;
   for(i=0;i<you.length; i++)
   {
	   if(you.list[i].shape=='d')
	   {
		  gotoxy(8+(i*4),18);   printf("��",you.list[i].shape);
	   }
	   else if(you.list[i].shape=='c')
	   {
		   gotoxy(8+(i*4),18);   printf("��",you.list[i].shape);
	   }
	   else if(you.list[i].shape=='s')
	   {
		   gotoxy(8+(i*4),18);   printf("��",you.list[i].shape);
	   }
	   else if(you.list[i].shape=='h')
	   {
		   gotoxy(8+(i*4),18);   printf("��",you.list[i].shape);
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
		    gotoxy(32,10); printf("��",peek(deck3).shape);
	   }
	   else if(peek(deck3).shape=='c')
	   {
		    gotoxy(32,10); printf("��",peek(deck3).shape);
	   }

	   else if(peek(deck3).shape=='s')
	   {
		    gotoxy(32,10); printf("��",peek(deck3).shape);
	   }
	   else if(peek(deck3).shape=='h')
	   {
		    gotoxy(32,10); printf("��",peek(deck3).shape);
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
		//printf("����");
		system("cls");
		print_lose();
		exit(1);
	}
	else
	{
		u->list[u->length++]=pop(deck2);
		Print2();		//ī��޾ƿ��� ���
	}
}

void card_throw(int position)
{
	if((you.list[position].num == peek(deck3).num) || (you.list[position].shape == peek(deck3).shape))
	{
		
		push(deck3,delete_list(&you,position));
		Card_Attack(&computer,peek(deck3));
		Print();		// �� �� ���� ī�带 ����
		Print2();
		turn=1;
		
	}
	else
	{
		
		gotoxy(32,15); printf("�� �� ���� ī���Դϴ�.");
		fflush(stdin);	//���͸� ������ �ش纯���� ��Ī, �� ���۸� �����.
		
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
	if(turn==1)		//�� ī�尡 �����ÿ�
	{
		computer.list[computer.length++]=pop(deck2);
		Print2();
		turn=0;
	}
}

//------------------------����ȭ�� ����Լ�---------------------------------//
void Print() // ����ȭ�� ���
{
	int i=0,j=0;
	removeCursor(); // Ŀ���� ����
	system("cls"); // ȭ�� Ŭ����
	gotoxy(0,0);
	printf(" ������������������������������������������������������������������������������\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ������������������������������������������������������������������������������\n");
	
	
	//gotoxy(70,1); printf("1093060");
	//gotoxy(70,2); printf(" �ڻ��");
	//gotoxy(55,8); printf("�� ���� ī�� :  ��");
	//gotoxy(55,10); printf("���� ī�� :  �� ");
	//gotoxy(55,12); printf("���� :  ");
	//gotoxy(55,14); printf("��� :  ");
	

	for(i=0; i<15; i++)
	{
		gotoxy(6+(i*4),2); printf("��������");
		gotoxy(6+(i*4),3); printf("��    ��");
		gotoxy(6+(i*4),4); printf("��    ��");
		gotoxy(6+(i*4),5); printf("��������");	
	}
	for(i=0; i<15; i++)
	{
		gotoxy(6+(i*4),17); printf("��������");
		gotoxy(6+(i*4),18); printf("��    ��");
		gotoxy(6+(i*4),19); printf("��    ��");
		gotoxy(6+(i*4),20); printf("��������");
	}	
	
	gotoxy(30,9); printf("��������");
	gotoxy(30,10); printf("��    ��");
	gotoxy(30,11); printf("��    ��");
	gotoxy(30,12); printf("��������");

	gotoxy(40,9); printf("��������");
	gotoxy(40,10); printf("��    ��");
	gotoxy(40,11); printf("��    ��");
	gotoxy(40,12); printf("��������");
	
}
//----------------------------------------------------//
//-------------------���� �޴��Լ�--------------------//
int MENU()  //���� �޴�
{
	int key, menu=1;
	system("cls"); // ȭ�� Ŭ����
	gotoxy(0,0); 
	printf(" ������������������������������������������������������������������������������\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ��                                                                          ��\n");
	printf(" ������������������������������������������������������������������������������\n");
	gotoxy(70,1); printf("1093060");
	gotoxy(70,2); printf(" �ڻ��");
	gotoxy(30,2);printf("        < �� �� >          ");
	gotoxy(30,5);printf(" ����������������������");
	gotoxy(30,6);printf(" ��    ���� ����     ��");
	gotoxy(30,7);printf(" ����������������������");

	gotoxy(30,9);printf(" ����������������������");
	gotoxy(30,10);printf(" ��    ���� ����     ��");
	gotoxy(30,11);printf(" ����������������������");

	gotoxy(30,13);printf(" ����������������������");
	gotoxy(30,14);printf(" ��    ���� �ϱ�     ��");
	gotoxy(30,15);printf(" ����������������������");
	
	
	while(1)
	{
		removeCursor(); // Ŀ���� ����
		gotoxy(28,(menu*4)+2);//������ �޴��� ���ʿ� �Ѹ� ���
		printf("��");
		key=_getch();
		gotoxy(28,(menu*4)+2);//���ڸ��� ��ĭ�� ä���� �Ѹ���� ����
		printf(" ");
		switch(key)
		{
			case UP : menu--; // ȭ��ǥ ���� �޾��� ���
				if(menu<=0)
					menu=3;
				break;

			case DOWN : menu++; // ȭ��ǥ �Ʒ��� �޾��� ��� 
				if(menu>=4)
					menu=1;
				break;

			case ENTER : return menu; // ���͸� �������
		}
	}
}
//----------------------------------------------------//
//------------------- ���� �Լ� --------------------//
void help() // ���� ���
{
	gotoxy(2,1); printf("�� ��ī�����\n");
	gotoxy(2,3); printf("1. ī��� �� 54���̴�. ��,��,��,�� = A~10,J,Q,K 52�� JOKER 2��\n");
	gotoxy(2,4); printf("2. �� ����� �� ���ڿ� �̾ ���� ����̳� ���� ������ ī�带 ����\n"); 
	gotoxy(2,5); printf("   �Ǵ� �����̴�. [�� : ��->��, 10 -> 10 ]\n");
	gotoxy(2,6); printf("3. play�� ��ǻ�Ϳ� 1:1 ��ᱸ�� \n");
	gotoxy(2,9); printf("�� ���� ��Ģ\n");
	gotoxy(2,11); printf("1. ���ӽ��۽� ����ī�带 5�徿 ���������� 1���� ���� �߾ӿ� ��ġ�Ѵ�.\n");
	gotoxy(2,12); printf("2. ���� ���ڳ� ��� ī�带 �� �� �ִ�. �� ���� ���� �ÿ��� ������ �޴´�.\n");
	gotoxy(2,13); printf("3. J- ������ , K- �ѹ��� , Q- �����ٲ�\n");
	gotoxy(2,14); printf("4. 7 - (��,��,��,��) ��� �ٲ�\n");
	gotoxy(2,15); printf("5. ��2,��2,��2,��2 < ��A,��A,��A < ��A \n");
    gotoxy(2,16); printf("6. ������ �������� �������̳� �� �� ī��� ���\n");
	gotoxy(2,17); printf("7. ī�带 ���� ���ִ� ���� '�¸�'\n");
	gotoxy(2,18); printf("8. ī�尡 20���� �Ѵ� ���� '�й�'\n");
	gotoxy(2,22);printf("-�ƹ�Ű�� ������ ����ȭ������ ���ư��ϴ�-\n");
	removeCursor();
}
//----------------------------------------------------//
//------------------------�����Լ�--------------------//
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
			removeCursor(); // Ŀ���� ����
			gotoxy((onbar*4)+2,21); 
			printf(" ��");
			key=_getch();
			gotoxy((onbar*4)+2,21);
			printf("  ");
			switch(key)
			{
			
				case UP:
					gotoxy(41,13);
					printf(" ��");
					key=_getch();
					if(key==ENTER)
					{
						card_select(&you);
						turn=1;
					}
					gotoxy(41,13); printf("  ");
					break;
				
				case LEFT: onbar--;		//ȭ��ǥ ������ �޾��� ���
					if(onbar<=0)
						onbar=15;
					break;
				case RIGHT: onbar++;		//ȭ��ǥ �������� �޾��� ���
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

//------------------�޴� ���̽� �Լ�--------------------//
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
			getchar();		//�Է¹��������� ��ٸ�
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

//---------------------���� �Լ�----------------------//
int main()
 {
	choice();
}
//----------------------------------------------------//
