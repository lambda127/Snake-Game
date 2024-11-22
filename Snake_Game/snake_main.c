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


struct BOMB_BOX
{
	int x, y;
};
typedef struct BOMB_BOX BOMB;


enum difficulties {
	
};


body * head = NULL; // �Ӹ� ����

// ���ܵ� ���� ������, �迭�� �ϸ� ũ�� ������ �ָ��� -> ���� ����ũ�� �ϳ��� �� �����?
body* cut_bodies = NULL;

// ���Ḯ��Ʈ �̿�����  ������ �ִ� ���� x, y �ʿ� �������� ���� ����
int i, j, height = 22, width = 22;  
int gameover, score;
int fruitx, fruity, flag;
int mysteryx, mysteryy;  // mystery -> '?'
BOMB bomb[4]; // bomb -> ��
int mystery_cnt = 0, bomb_cnt = 0;// mystery�� bomb�� ����
int body_length = 1;
int speed = 200;








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
body* find_tail(body* st) {
	body* cur = st;
	while (cur->next != NULL) {
		cur = cur->next;
	}

	return cur;
}

// ������ ������ ���� �������� ����
void insert_body(body * st, body* new_body) {
	body* tail = find_tail(st);
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

// 1 -> �̽��׸� �ڽ� ���� ���� ��ɿ� ���, �Ű� ������ ���� change ���� �� ��(change�� ������ ��쿡 ������ 0 �����̸� ���� ��ȭ X (2024.11.23 ����)
// 2 -> ��ź �ڽ��� ���� ���, ���� �����ʹ� ��� ���� ������ ������ ��ȭ ��Ŵ
void change_score(int num,int change) {

	switch (num) {
	case 1:
		score += (score > 0 || change > 0) ? change : 0;
		break;

	case 2:
		score += change;
		break;
	
	default:
		break;
	
	}

}


// 1~r ������ ������ ���� ��ȯ
int random_num(int r) {
	return rand() % r + 1;
}



//  1~5 ���� ������ ���� �߰�
void body_add(int r) {
	for (int i = 0; i < r; i++) {
		insert_body(head, create_body(head->x, head->y, head->direction));
		body_length++;
	}

}





// Ŀ�� ��ġ �̵�
void gotoxy(int x, int y){

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
		fruitx = rand() % 20 + 1;
	}

	fruity = 0;
	while (fruity == 0) {
		fruity = rand() % 20 + 1;
	}


	// bomb
	for (int i = 0; i < 4; i++) {
		bomb[i].x = NULL;
		bomb[i].y = NULL;
	}

	bomb[0].x = 0;
	bomb[0].y = 0;
	while (bomb[0].x == 0 || bomb[0].y == 0
		||(bomb[0].x == fruitx  && bomb[0].y == fruity)
		||(bomb[0].x == mysteryx && bomb[0].y == mysteryy)) {

		bomb[0].x = rand() % 20 + 1;
		bomb[0].y = rand() % 20 + 1;
	}

	

	// event
	mysteryx = 0;
	mysteryy = 0;
	while (mysteryx == 0 || mysteryy == 0
		|| (mysteryx == fruitx && mysteryy == fruity)
		|| (mysteryx == bomb[0].x && mysteryy == bomb[0].y)) {
		mysteryx = rand() % 20 + 1;
		mysteryy = rand() % 20  +1;
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
				else if (i == bomb[0].x && j == bomb[0].y
					|| i == bomb[1].x && j == bomb[1].y
					|| i == bomb[2].x && j == bomb[2].y
					|| i == bomb[3].x && j == bomb[3].y) // bomb�� ��� ��ǥ Ȯ��
				{
					if (bomb_cnt == 0/*�ϴ� �ӽ÷� 0�� �̸� ����� �� ��, ������ ���� ���� �� �ְ� �� ��*/) {
						printf("��");
					
					}
					else
					{
						printf(" ");
					}
				}
				else if (i == mysteryx && j == mysteryy )
				{
					if (mystery_cnt == 1) {
						printf("?");
					}
					else if (rand() % 100 < 10 && mystery_cnt == 0) {
						mystery_cnt = 1;
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


		insert_body(head, create_body(head->x, head->y, head->direction)); // fruit�� �Ծ��� �� ���� �߰�
		body_length++;

		score += 10;
	}

	/*
	������ ���� bomb���� �ϴ� ��� ������ ��
	*/



	// snake�� bomb�� ����� ��
	for (int i = 0; i < 4; i++) {
		if ((head->x == bomb[i].x && head->y == bomb[i].y) /*|| (round_snake(bombx, bomby) == 1)*/) {
			change_score(2, -50);
		}
	}


	// snake�� event�� ����� ��
	if ((head->x == mysteryx && head->y == mysteryy) /*|| (round_snake(eventx, eventy) == 1)*/) {

		// event
		mysteryx = 0;
		mysteryy = 0;
		
		for (int i = 0; i < 4; i++) {
			while (mysteryx == 0 || mysteryy == 0
				|| (mysteryx == fruitx && mysteryy == fruity)
				|| (mysteryx == bomb[i].x && mysteryy == bomb[i].y)) {
				mysteryx = rand() % 20;
				mysteryy = rand() % 20;
			}
		}
		
		int effect = rand() % 3;
		switch (effect) {
		case 0: // ���� ����
			change_score(1, random_num(50));
			break;

		case 1: // ���� ����
			change_score(1, -random_num(50));
			break;

		case 2: // ���� ���� ����
			body_add(random_num(5));
			break;
		}
		

		mystery_cnt--;

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
