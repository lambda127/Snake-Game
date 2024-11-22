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
typedef struct BODY body; // 타입 정의




body * head = NULL; // 머리 선언

// 연결리스트 이용으로  기존에 있던 변수 x, y 필요 없어짐에 따라 제거
int i, j, height = 22, width = 22;  
int gameover, score;
int fruitx, fruity, flag;
int eventx, eventy, bombx, bomby;  // event -> '?', bomb -> ▣
int event_bool = 0, bomb_bool = 0;// event와 bomb이 생성되어있는가확인
int body_length = 1;
int speed = 200; // easy -> 200, normal -> 100, hard -> 50 정도로 세팅하면 재밌을 듯






// 몸통 생성, 머리 혹은 앞 몸통의 좌표, 보고 있는 방향을 받아 그 다음 위치에 좌표 설정, 바라보는 방향 설정
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

// 마지막 몸통(꼬리) 탐색
body* find_tail() {
	body* cur = head;
	while (cur->next != NULL) {
		cur = cur->next;
	}

	return cur;
}

// 생성한 몸통을 가장 마지막에 연결
void insert_tail(body* new_body) {
	body* tail = find_tail();
	tail->next = new_body;
}

// 좌표를 받아 해당좌표에 몸통이 있는지 확인하는 함수
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






// 머리를 제외한 몸통을 움직이는 기능
// 앞 몸통의 좌표 저장(prev_x, prev_y), 자신의 좌표 임시 저장(temp_x, temp_y), 자신에게 앞 몸통 좌표 저장, 자신의 좌표 였던 좌표 prev_x, prevy애 저장
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





// **** 삭제 예정 ****
// 1 -> 0~149 사이의 값을 speed에서 뺌 -> 속도 증가
// 2 -> 0~99 사이의 값을 speed에 더함 -> 속도 감소
void rand_speed(int num) {
	if (num == 1)
		speed -= rand() % 150;
	
	if (num == 2)
		speed += rand() % 100;
}

// 1 -> 1~50 사이의 값을 score에서 뺌.....+ 점수가 0 이하이면 점수 감소 X (2024.11.22 수정)
// 2 -> 1~50 사이의 값을 score에 더함
void rand_score(int num) {
	if (num == 1)
		score -= score > 0 ? ((rand() % 50) + 1) : 0;

	if (num == 2)
		score += (rand() % 50) + 1;
}

//  1~5 사이 개수의 몸통 추가
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
	// 커서 세팅
	CONSOLE_CURSOR_INFO cursorInfo = { 0, }; // 찾아봐야 함
	cursorInfo.dwSize = 1; // 커서 굵기 (1 ~ 100)
	cursorInfo.bVisible = FALSE; // 커서 Visible TRUE(보임) FALSE(숨김)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo); // 설정 적용


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
	gotoxy(0, 0); // 커서 위치 처음으로 보내기 -> 지우고 다시 작성하므로써 발생한 깜빡임 제거
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
				else if (round_snake(i, j) == 1) // 영역을 그리고 있는 위치에 몸통이 있는지 확인 -> 존재 O => 'o'출력
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
						printf("▣");
					}
					else if (rand() % 100 < 5 && bomb_bool == 0) {
						bomb_bool = 1;
						printf("▣");
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
	Sleep(speed); // 속도 조절용 변수 (기본갑 : 200, 특정 아이템을 먹으면 빨라지게 할 예정)
	switch (flag) {
	case 1:
		// 머리의 좌표를 먼저 변경하면 머리 바로 다음 몸통이 사라지는 것을 발견 
		// -> 몸통의 좌표를 먼저 변경한 이후, 머리 좌표변경 
		// (어차피 머리가 있는 방향으로 이동 및 머리가 보고 있던 방향으로 방향값 변경)
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

	if (round_snake(head->x, head->y) == 1) // 몸통에 머리가 닿으면 종료
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


		insert_tail(create_body(head->x, head->y, head->direction)); // fruit을 먹었을 때 몸통 추가
		body_length++;

		score += 10;
	}


	// snake가 bomb에 닿았을 때
	if ((head->x == bombx && head->y == bomby) /*|| (round_snake(bombx, bomby) == 1)*/) {
		gameover = 1;
	}


	// snake가 event에 닿았을 때
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







// 동적 할당된 모든 body(node) 동적 할당 해제
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


	free_snake(head); // snake 동적할당 해제
}
