// C program to build the complete 
// snake game 
#include <conio.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <io.h> 
#include <Windows.h>
#include <time.h>



struct BODY
{
	int x, y, direction; //direction : 1 -> -y, 2 -> -x, 3 -> +y, 4-> +x
	struct BODY* next;
};
typedef struct BODY body; // Ÿ�� ����




body * head = NULL; // �Ӹ� ����

// ���Ḯ��Ʈ �̿�����  ������ �ִ� ���� x, y �ʿ� �������� ���� ����
int i, j, height = 22, width = 22;  
int gameover, score;
int fruitx, fruity, flag;
int eventx, eventy, bombx, bomby;  // event -> '?', bomb -> ��
int event_bool = 0, bomb_bool = 0;// event�� bomb�� �����Ǿ��ִ°�Ȯ��
int body_length = 1;
int speed = 200; // easy -> 200, normal -> 100, hard -> 50 ������ �����ϸ� ����� ��






// ���� ����, �Ӹ� Ȥ�� �� ������ ��ǥ, ���� �ִ� ������ �޾� �� ���� ��ġ�� ��ǥ ����, �ٶ󺸴� ���� ����
body* create_body(int x, int y, int direction) {

	body* new_body = (body*)malloc(sizeof(body));

	switch (direction) {
	case 1:
		new_body->x = x;
		new_body->y = y - 1;
		break;
	case 2:
		new_body->x = x - 1;
		new_body->y = y;
		break;
	case 3:
		new_body->x = x;
		new_body->y = y + 1;
		break;
	case 4:
		new_body->x = x + 1;
		new_body->y = y;
		
		break;
	}
	new_body->next = NULL;
	new_body->direction = direction;

	return new_body;
}

// ������ ����(����) Ž��
body* find_tail() {
	body* cur = head;
	while (cur->next != NULL) {
		cur = cur->next;
	}

	return cur;
}

// ������ ������ ���� �������� ����
void insert_tail(body* new_body) {
	body* tail = find_tail();
	tail->next = new_body;
}

// ��ǥ�� �޾� �ش���ǥ�� ������ �ִ��� Ȯ���ϴ� �Լ�
int round_snake(int x, int y) {
	body* cur = head->next;

	while (cur != NULL) {
		if (x == cur->x && y == cur->y) {
			return 1;
		}
		
		cur = cur->next;
	}
	
	return 0;
}






// �Ӹ��� ������ ������ �����̴� ���
// �� ������ ��ǥ ����(prev_x, prev_y), �ڽ��� ��ǥ �ӽ� ����(temp_x, temp_y), �ڽſ��� �� ���� ��ǥ ����, �ڽ��� ��ǥ ���� ��ǥ prev_x, prevy�� ����
void move_body() {
	body* cur = head;
	int prev_x = cur->x;
	int prev_y = cur->y;
	int prev_direction = cur->direction;
	cur = cur->next;

	while (cur != NULL) {
		int temp_x = cur->x;
		int temp_y = cur->y;
		int temp_direction = cur->direction;

		cur->x = prev_x;


		cur->y = prev_y;
		cur->direction = prev_direction;

		prev_x = temp_x;
		prev_y = temp_y;
		prev_direction = temp_direction;

		cur = cur->next;
	}
}





// **** ���� ���� ****
// 1 -> 0~149 ������ ���� speed���� �� -> �ӵ� ����
// 2 -> 0~99 ������ ���� speed�� ���� -> �ӵ� ����
void rand_speed(int num) {
	if (num == 1)
		speed -= rand() % 150;
	
	if (num == 2)
		speed += rand() % 100;
}

// 1 -> 1~50 ������ ���� score���� ��.....+ ������ 0 �����̸� ���� ���� X (2024.11.22 ����)
// 2 -> 1~50 ������ ���� score�� ����
void rand_score(int num) {
	if (num == 1)
		score -= score > 0 ? ((rand() % 50) + 1) : 0;

	if (num == 2)
		score += (rand() % 50) + 1;
}

//  1~5 ���� ������ ���� �߰�
void rand_body_add() {
	for (int i = 1; i <= (rand() % 5) + 1; i++) {
		insert_tail(create_body(head->x, head->y, head->direction));
		body_length++;
	}

}






void gotoxy(int x, int y)

{

	COORD pos = { x,y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

}


// Function to generate the fruit 
// within the boundary 
void setup()
{
	// Ŀ�� ����
	CONSOLE_CURSOR_INFO cursorInfo = { 0, }; // ã�ƺ��� ��
	cursorInfo.dwSize = 1; // Ŀ�� ���� (1 ~ 100)
	cursorInfo.bVisible = FALSE; // Ŀ�� Visible TRUE(����) FALSE(����)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo); // ���� ����


	gameover = 0;

	head = (body*)malloc(sizeof(body));

	// Stores height and width 
	head->x = height / 2;
	head->y = width / 2;
	head->next = NULL;
	head->direction = NULL;

	// fruit
	fruitx = 0;
	while (fruitx == 0) {
		fruitx = rand() % 20;
	}

	fruity = 0;
	while (fruity == 0) {
		fruity = rand() % 20;
	}


	// bomb
	bombx = 0;
	bomby = 0;
	while (bombx == 0 || bomby == 0
		||(bombx == fruitx  && bomby == fruity)
		||(bombx == eventx && bomby == eventy)) {

		bombx = rand() % 20;
		bomby = rand() % 20;
	}

	

	// event
	eventx = 0;
	eventy = 0;
	while (eventx == 0 || eventy == 0
		|| (eventx == fruitx && eventy == fruity)
		|| (eventx == bombx && eventy == bomby)) {
		eventx = rand() % 20;
		eventy = rand() % 20;
	}

	

	score = 0;
}

// Function to draw the boundaries 
void draw()
{
	gotoxy(0, 0); // Ŀ�� ��ġ ó������ ������ -> ����� �ٽ� �ۼ��ϹǷν� �߻��� ������ ����
	//system("cls");

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			if (i == 0 || i == width - 1
				|| j == 0
				|| j == height - 1) {
				printf("#");
			}
			else {
				if (i == head->x && j == head->y) {
					printf("O");
				}
				else if (round_snake(i, j) == 1) // ������ �׸��� �ִ� ��ġ�� ������ �ִ��� Ȯ�� -> ���� O => 'o'���
				{
					printf("o");
				}
				else if (i == fruitx && j == fruity)
				{
					printf("*");

				}
				else if (i == bombx && j == bomby)
				{
					if (bomb_bool == 1) {
						printf("��");
					}
					else if (rand() % 100 < 5 && bomb_bool == 0) {
						bomb_bool = 1;
						printf("��");
					}
					else
					{
						printf(" ");
					}
				}
				else if (i == eventx && j == eventy )
				{
					if (event_bool == 1) {
						printf("?");
					}
					else if (rand() % 100 < 10 && event_bool == 0) {
						event_bool = 1;
						printf("?");
					}
					else
					{
						printf(" ");
					}
				}
				else
				{
					printf(" ");
				}
			}
					
			
		}
		printf("\n");
	}

	// Print the score after the 
	// game ends 
	printf("score = %d | body_length = %d |", score, body_length);
	printf("\n");
	printf("press X to quit the game\n");
}

// Function to take the input 
void input()
{
	if (_kbhit()) {
		switch (getch()) {
		case 'a':
			flag = 1;
			break;
		case 's':
			flag = 2;
			break;
		case 'd':
			flag = 3;
			break;
		case 'w':
			flag = 4;
			break;
		case 'x':
			gameover = 1;
			break;
		}
	}
}

// Function for the logic behind 
// each movement 
void logic()
{
	Sleep(speed); // �ӵ� ������ ���� (�⺻�� : 200, Ư�� �������� ������ �������� �� ����)
	switch (flag) {
	case 1:
		// �Ӹ��� ��ǥ�� ���� �����ϸ� �Ӹ� �ٷ� ���� ������ ������� ���� �߰� 
		// -> ������ ��ǥ�� ���� ������ ����, �Ӹ� ��ǥ���� 
		// (������ �Ӹ��� �ִ� �������� �̵� �� �Ӹ��� ���� �ִ� �������� ���Ⱚ ����)
		move_body(); 
		head->y--;
		break;
	case 2:
		move_body();
		head->x++;
		break;
	case 3:
		move_body();
		head->y++;
		break;
	case 4:
		move_body();
		head->x--;
		break;
	default:
		break;
	}

	// If the game is over 
	if (head->x < 0 || head->x > height
		|| head->y < 0 || head->y > width - 1)
		gameover = 1;

	if (round_snake(head->x, head->y) == 1) // ���뿡 �Ӹ��� ������ ����
		gameover = 1;


	// If snake reaches the fruit 
	// then update the score 
	if ((head->x == fruitx && head->y == fruity) /*|| (round_snake(fruitx, fruity) == 1 )*/) {

		fruitx = 0;
		while (fruitx == 0 ) {
			fruitx = rand() % 20;
		}

		fruity = 0;
		while (fruity == 0) {
			fruity = rand() % 20;
		}


		insert_tail(create_body(head->x, head->y, head->direction)); // fruit�� �Ծ��� �� ���� �߰�
		body_length++;

		score += 10;
	}


	// snake�� bomb�� ����� ��
	if ((head->x == bombx && head->y == bomby) /*|| (round_snake(bombx, bomby) == 1)*/) {
		gameover = 1;
	}


	// snake�� event�� ����� ��
	if ((head->x == eventx && head->y == eventy) /*|| (round_snake(eventx, eventy) == 1)*/) {

		// event
		eventx = 0;
		eventy = 0;
		while (eventx == 0 || eventy == 0
			|| (eventx == fruitx && eventy == fruity)
			|| (eventx == bombx && eventy == bomby)) {
			eventx = rand() % 20;
			eventy = rand() % 20;
		}

		
		int effect = rand() % 5;
		switch (effect) {
		case 0:
			rand_speed(1);
			break;

		case 1:
			rand_speed(2);
			break;

		case 2:
			rand_score(1);
			break;

		case 3:
			rand_score(2);
			break;
		case 4:
			rand_body_add();
			break;
		}
		

		event_bool = 0;

	}
}







// ���� �Ҵ�� ��� body(node) ���� �Ҵ� ����
void free_snake(body* node) {
	body* cur = node;
	body* next;

	while (cur->next != NULL) {
		next = cur->next;
		free(cur);
		cur = next;
	}
}








void main()
{
	srand(time(NULL));

	// Generate boundary 
	setup();

	// Until the game is over 
	while (!gameover) {

		// Function Call 
		draw();
		input();
		logic();
	}


	free_snake(head); // snake �����Ҵ� ����
}
