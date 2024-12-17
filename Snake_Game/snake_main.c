// C program to build the complete 
// snake game 
#include <conio.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <io.h> 
#include <Windows.h>
#include <time.h>
#include <string.h>


/// 구조체 선언 ///

typedef struct BODY
{
	int x, y, direction; //direction : 1 -> -y, 2 -> -x, 3 -> +y, 4-> +x
	int body_no;
	struct BODY* next;
} body; // 타입 정의


typedef struct BOMB_BOX
{
	int x, y;
}  BOMB;

/// 구조체 선언 종료 ///


/// enum 선언 ///

enum Speed {
	lvl1 = 300,
	lvl2 = 250,
	lvl3 = 175,
	lvl4 = 120,
	lvl5 = 80
};


enum ColorType {
	Black,  	//0
	DarkBlue,	//1
	DarkGreen,	//2
	DarkSkyBlue,    //3
	DarkRed,  	//4
	DarkPurple,	//5
	DarkYellow,	//6
	Gray,		//7
	DarkGray,	//8
	Blue,		//9
	Green,		//10
	SkyBlue,	//11
	Red,		//12
	Purple,		//13
	Yellow,		//14
	White		//15
};

/// enum 선언 동료 ///



/// 전역 변수 선언 ///

body * head = NULL; // 머리 선언

// 절단된 몸통 보관용, 배열로 하면 크기 배정이 애매함 -> 연결 리스크를 하나를 더 만들까? 
body* cut_bodies = NULL;

// 연결리스트 이용으로  기존에 있던 변수 x, y 필요 없어짐에 따라 제거
int i, j, height = 27, width = 27;  
int gameover, score;

int fruitx, fruity, flag;
int mysteryx, mysteryy;  // mystery -> '?'
int trapx, trapy;  // trap -> ◎

BOMB bomb[4]; // bomb -> ▣

int mystery_cnt = 0, bomb_cnt = 1;// mystery와 bomb의 개수
int body_length = 1;
int speed = lvl1; 
int st_level = 0;
int level = 1;

char player[20] = "Anonym"; // 플레이어 이름, 기본값 = "Anonym"

char best_player[20]; // 최고 점수 기록자 이름
int highest_score = 0; // 최고 점수

/// 전역 변수 선언 종료 ///


/// 함수 선언 ///

// 레벨 선택, 처음 인트로 부분
int select_level();

// 대화 상자 그리기
void draw_dlg_box(int box_width, int box_height, int start_x);

// 입력 예시(힌트) 설정, 입력 받은 좌표에 입력받은 힌트 메시지 표시
void set_hint(int x, int y, char hint[30]);

// 엔터 입력을 기다리는 함수
void wait_enter();

// 지정한 좌표로 커서를 이동해 " "(공백)를(을) 출력하여 보이는 글자를 지우는 함수
void delete_line(int x, int y);

// 몸통 생성, 머리 혹은 앞 몸통의 좌표, 보고 있는 방향을 받아 그 다음 위치에 좌표 설정, 바라보는 방향 설정
body* create_body(int x, int y, int direction, int body_no);

// 마지막 몸통(꼬리) 탐색
body* find_tail(body* st);

// 생성한 몸통을 가장 마지막에 연결
void insert_body(body* st, body* new_body);

// 좌표를 받아 해당좌표에 몸통이 있는지 확인하는 함수
int round_snake(body* st, int x, int y);

// 머리를 제외한 몸통을 움직이는 기능
// 앞 몸통의 좌표 저장(prev_x, prev_y), 자신의 좌표 임시 저장(temp_x, temp_y), 자신에게 앞 몸통 좌표 저장, 자신의 좌표 였던 좌표 prev_x, prevy애 저장
void move_body();

// 1 -> 미스테리 박스 점수 변동 기능에 사용, 매개 변수로 받은 change 값을 더 함(change가 음수인 경우에 점수가 0 이하이면 점수 변화 X
// 2 -> 폭탄 박스를 위한 기능, 현재 점수와는 상관 없이 무조건 점수를 변화 시킴
void change_score(int num, int change);

// 1~r 사이의 랜덤한 값을 반환
int random_num(int r);

//  1~5 사이 개수의 몸통 추가
void body_add(int r);

// 커서 위치 이동
void gotoxy(int x, int y);

// 글자 색깔 바꾸기
void textcolor(enum ColorType color);

// 한영키 누르기
int KEYINPUT();

// 기본적인 시스템적인 요소를 설정하고 게임 요소를 만드는 함수 
void setup();

// Function to draw the boundaries 
void draw();

// Function to take the input 
void input();

// Function for the logic behind 
// each movement 
void logic();

// 게임 종료시 화면, 기록을 보여주며 하이 스코어가 바뀌는지 알려준다.
void gameover_screen();

// 동적 할당된 모든 body(node) 동적 할당 해제
void free_snake(body* node);

// record.txt 파일에서 기록을 읽어와 최고 점수와 플레이어를 화면에 띄울 수 있도록 하는 함수
int read_record();

// 게임 종료 시점에 현재 플레이어의 점수와 최고 점수를 비교하여 최고 점수를 갱신 혹은 유지하는 함수, record.txt 파일에 점수를 저장한다.
int write_record();


/// 함수 선언 종료 ///



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

	write_record();
	free_snake(head); // snake 동적할당 해제
	free_snake(cut_bodies);

	gameover_screen();
	system("pause");
}


/// 함수 정의 ///

/* 대화 파트 */

// 레벨 선택, 처음 인트로 부분
int select_level() {
	enum Speed speed = lvl1; // 지역 변수 speed
	int sel = 0;
	char answer[20];


	draw_dlg_box(80, 17, 0);


	gotoxy(3, 4);
	printf("(∩^o^)⊃━☆ : ");

	gotoxy(3, 11);
	printf("??? : ");

	gotoxy(15, 4);
	printf("안녕하지렁~ 나는 지렁이지렁!!             ");

	set_hint(9, 11, "(다음 : ENTER)");

	wait_enter();
	
	delete_line(9, 11);

	char s[20];
	gotoxy(15, 4);
	printf("게임 방법을 알고 있지렁?                      ");

	KEYINPUT();
	
	set_hint(9, 11, "(모르지렁 or 알지렁)");


	gotoxy(9, 11);
	scanf_s("%s", s, (int)sizeof("모르지렁"));


	delete_line(9, 11);


	// 게임 방법 설명
	if (!strcmp(s, "모르지렁")) {
		gotoxy(15, 4);
		printf("설명 시작하겠지렁                ");


		set_hint(9, 11, "(다음 : ENTER)");
		gotoxy(9, 11);

		wait_enter();
		
		


		gotoxy(15, 4);
		printf("이동은 w, a, s, d로 하지렁");

		gotoxy(9, 11);

		wait_enter();


		gotoxy(15, 4);
		printf("벽에 닿으면 죽지렁");

		gotoxy(9, 11);
		
		wait_enter();


		gotoxy(15, 4);
		printf("*는 먹이지렁, 먹으면 점수가 증가하고 몸통이 늘어나지렁");

		gotoxy(9, 11);

		wait_enter();

		


		gotoxy(15, 4);
		printf("?는 미스테리박스지렁, 먹으면 랜덤으로 효과가 발생하지렁");
		gotoxy(15, 5);
		printf("점수 감소, 점수 증가, 몸통 길이 증가가 있지렁 ");

		gotoxy(9, 11);

		wait_enter();

		delete_line(15, 4);
		delete_line(15, 5);


		gotoxy(15, 4);
		printf("▣는 폭탄이지렁, 먹으면 점수가 감소하지렁");

		gotoxy(9, 11);

		wait_enter();

		delete_line(15, 4);


		gotoxy(15, 4);
		printf("◎는 부비트랩이지렁, 먹으면 몸통이 잘려 버리지렁... 무섭지렁...");
		gotoxy(15, 5);
		printf("잘린 몸통은 남아 있고, 몸통을 먹으면 죽는 것이지렁.");

		gotoxy(9, 11);

		wait_enter();

		delete_line(15, 4);
		delete_line(15, 5);


		gotoxy(15, 4);
		printf("설명 끝났지렁");

		gotoxy(9, 11);

		wait_enter();

		delete_line(15, 4);
	}


	// 게임 플에이 여부 질문
	gotoxy(15, 4);
	printf("내가 성장할 수 있도록 잘 도와줄 수 있지렁?");


	set_hint(9, 11, "(당연하지렁 or 싫지렁)");

	
	gotoxy(9, 11);
	scanf_s("%s", answer, (int)sizeof("당연하지렁"));

	
	delete_line(9, 11);


	// "당연하지렁" -> 게임 진행
	// (다른 입력) -> 종료
	if (strcmp(answer, "당연하지렁") == 0) {
		delete_line(15, 4);
		delete_line(15, 5);


		gotoxy(15, 4);
		printf("좋았지렁!! 그럼 이제 이름을 말해지렁!        ");


		gotoxy(9, 11);
		scanf_s("%s", player, 20);

		gotoxy(3, 11);
		printf("%s :                ", player);

		
		gotoxy(15, 4);
		printf("%s !! 알겠어 잘 기억하겠지렁.>,<       ", player);

		gotoxy(15, 4);
		printf("그럼 이제 원하는 레벨을 말해지렁!      ");


		gotoxy(strlen(player) + 6, 11);
		printf("Level ");
		scanf_s("%d", &sel);

		KEYINPUT(); // 한영키 입력
	}
	else {
		gotoxy(15, 4);
		printf("알겠지렁..그럼 아쉽지만 여기서 종료하겠지렁..");

		gotoxy(0, 21);

		system("pause");
		exit(0);

		
	}

	

	switch (sel) {
	case 1:
		st_level = 1;
		level = 1;
		speed = lvl1;
		break;
	case 2:
		st_level = 2;
		level = 2;
		speed = lvl2;
		break;
	case 3:
		st_level = 3;
		level = 3;
		speed = lvl3;
		break;
	case 4:
		st_level = 4;
		level = 4;
		speed = lvl4;
		break;
	case 5:
		st_level = 5;
		level = 5;
		speed = lvl5;
		break;
	}

	return speed;
}


// 대화 상자 그리기
void draw_dlg_box(int box_width, int box_height, int start_x) {
	
	// 입력 받은 크기 만큼 박스를 그린다.
	for (int i = 0; i < box_height; i++) {
		gotoxy(start_x, i);
		for (int j = 0; j < box_width; j++) {
			if (i == 0 && j == 0) printf("┏");
			else if (i == 0 && j == box_width - 1) printf("┓");
			else if (i == box_height - 1 && j == 0) printf("┗");
			else if (i == box_height - 1 && j == box_width - 1) printf("┛");
			else if (j == 0 || j == box_width - 1) printf("┃");
			else if (i == 0 || i == box_height - 1) printf("━");
			else printf(" ");

		}
		printf("\n");
	}


}


// 입력 예시(힌트) 설정, 입력 받은 좌표에 입력받은 힌트 메시지 표시
void set_hint(int x, int y, char hint[30]) {
	gotoxy(x, y);
	textcolor(DarkGray);
	printf("%s", hint);
	textcolor(White);
}


// 엔터 입력을 기다리는 함수
void wait_enter() {
	while (!(_kbhit() && getchar() == '\n')) {}
}


// 지정한 좌표로 커서를 이동해 " "(공백)를(을) 출력하여 보이는 글자를 지우는 함수
void delete_line(int x, int y) {
	gotoxy(x, y);
	printf("                                                               ");
}




/* 지렁이 몸통 노드 관리 파트 */

// 몸통 생성, 머리 혹은 앞 몸통의 좌표, 보고 있는 방향을 받아 그 다음 위치에 좌표 설정, 바라보는 방향 설정
body* create_body(int x, int y, int direction, int body_no) {

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
	new_body->body_no = body_no;

	return new_body;
}


// 마지막 몸통(꼬리) 탐색
body* find_tail(body* st) {
	body* cur = st;
	while (cur->next != NULL) {
		cur = cur->next;
	}

	return cur;
}


// 생성한 몸통을 가장 마지막에 연결
void insert_body(body* st, body* new_body) {
	body* tail = find_tail(st);
	tail->next = new_body;
}


// 좌표를 받아 해당좌표에 몸통이 있는지 확인하는 함수
int round_snake(body* st, int x, int y) {
	body* cur = st == head ? st->next : st;


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




/* 이벤트 관리 */

// 1 -> 미스테리 박스 점수 변동 기능에 사용, 매개 변수로 받은 change 값을 더 함(change가 음수인 경우에 점수가 0 이하이면 점수 변화 X (2024.11.23 수정)
// 2 -> 폭탄 박스를 위한 기능, 현재 점수와는 상관 없이 무조건 점수를 변화 시킴
void change_score(int num, int change) {

	switch (num) {
	case 1:
		score += ((score + change) >= 0 || change > 0) ? change : 0;
		break;

	case 2:
		score += change;
		break;

	default:
		break;

	}

}


// 1~r 사이의 랜덤한 값을 반환
int random_num(int r) {
	return rand() % r + 1;
}


//  1~5 사이 개수의 몸통 추가
void body_add(int r) {
	for (int i = 0; i < r; i++) {
		body_length++;
		insert_body(head, create_body(head->x, head->y, head->direction, body_length));
	}

}





/* 콘솔 및 하드웨어 컨트롤 파트 */

// 커서 위치 이동
void gotoxy(int x, int y) {

	COORD pos = { x,y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

}


// 글자 색 바꾸기
void textcolor(enum colortype color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


// 한영키 누르기
int KEYINPUT() {// 한영키 누름
	keybd_event(VK_HANGEUL, 0, 0, 0);//누름

	keybd_event(VK_HANGEUL, 0, KEYEVENTF_KEYUP, 0);// 누름 해제

	return 0;
}




/* 게임 시스템 파트 */

// 기본적인 시스템적인 요소를 설정하고 게임 요소를 만드는 함수 
void setup()
{
	// 콘솔 크기 설정
	system("mode con:cols=150 lines=40");

	// 난이도 선택
	speed = select_level();
	system("cls");



	// 커서 세팅
	CONSOLE_CURSOR_INFO cursorInfo = { 0, }; // CONSOLE_CURSOR_INFO 구조체를 선언ㅇ
	cursorInfo.dwSize = 1; // 커서 굵기 (1 ~ 100)
	cursorInfo.bVisible = FALSE; // 커서 Visible TRUE(보임) FALSE(숨김)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo); // 설정 적용


	textcolor(White); // 글자 색깔 변환 (기본 : Gray)


	read_record(); // 최고 기록 읽어와 최고 플레이어, 최고 점수 변수에 저장

	gameover = 0;

	head = (body*)malloc(sizeof(body));

	// Stores height and width 
	// head의 구조체 변수 초기화
	head->x = height / 2;
	head->y = width / 2;
	head->next = NULL;
	head->direction = NULL;
	head->body_no = 1;


	cut_bodies = (body*)malloc(sizeof(body));

	// cut_bodies의 구조체 변수 초기화
	cut_bodies->x = NULL;
	cut_bodies->y = NULL;
	cut_bodies->next = NULL;
	cut_bodies->direction = NULL;
	cut_bodies->body_no = NULL;


	// fruit 위치 설정
	fruitx = 0;
	fruity = 0;

	while (fruitx == 0 || fruity == 0) {
		fruitx = rand() % (height - 1);
		fruity = rand() % (width - 1);
	}

	




	// event 위치 설정
	mysteryx = 0;
	mysteryy = 0;

	while (mysteryx == 0 || mysteryy == 0
		|| (mysteryx == fruitx && mysteryy == fruity)) {
		mysteryx = rand() % (height - 1);
		mysteryy = rand() % (width - 1);
	}





	// trap 위치 설정
	trapx = 0;
	trapy = 0;

	while (trapx == 0 || trapy == 0
		|| (trapx == fruitx && trapy == fruity)
		|| (trapx == mysteryx && trapy == mysteryy)) {
		trapx = rand() % (height - 1);
		trapy = rand() % (width-1);
	}






	// bomb 위치 설정

	for (int i = 0; i < 4; i++) { // bomb초기화
		bomb[i].x = NULL;
		bomb[i].y = NULL;
	}

	bomb[0].x = 0;
	bomb[0].y = 0;
	while (bomb[0].x == 0 || bomb[0].y == 0
		|| (bomb[0].x == fruitx && bomb[0].y == fruity)
		|| (bomb[0].x == mysteryx && bomb[0].y == mysteryy)
		|| (bomb[0].x == trapx && bomb[0].y == trapy)) {

		bomb[0].x = rand() % (height - 1);
		bomb[0].y = rand() % (width -1);
	}




	score = 0;
}


// Function to draw the boundaries 
void draw()
{
	gotoxy(0, 0); // 커서 위치 처음으로 보내기 -> 지우고 다시 작성하므로써 발생했던 깜빡임 제거 (덮어씌우므로써 해결)
	//system("cls");

	for (i = 0; i < height; i++) {
		gotoxy(29, i); // 특정 x위치로 이동 높이(y)는 반복문에 따라 변경) 줄 바꿈을 위함
		for (j = 0; j < width; j++) {
			
			// 테두리
			if (i == 0 && j == 0) printf("┏");
			else if (i == 0 && j == width - 1) printf("┓");
			else if (i == height - 1 && j == 0) printf("┗");
			else if (i == height - 1 && j == width - 1) printf("┛");
			else if (j == 0 || j == width - 1) {
				printf("┃");
			
				// 게임 정보 표시
				if (i == 1 && j == width - 1) printf(" Best player\t: %s\t ", best_player);
				if (i == 2 && j == width - 1) printf(" Highest Score\t: %d\t ", highest_score);

				if (i == 4 && j == width - 1) printf(" Player\t\t: %s\t ", player);
				if (i == 5 && j == width - 1) printf(" Score\t\t: %-d\t ", score);
				if (i == 6 && j == width - 1) printf(" Level\t\t: %d\t ", level);


			}
			else if (i == 0 || i == height - 1) printf("━"); // 테두리 끝
			else { // 요소 그리기 head, 몸통, cut_bodies, fruit, trap, mystery, bomb
				// head
				if (i == head->x && j == head->y) {
					printf("O");
				}
				// 몸통, cut_bodies
				else if (round_snake(head, i, j) == 1 || round_snake(cut_bodies, i, j) == 1) // 영역을 그리고 있는 위치에 몸통이 있는지 확인 -> 존재 O => 'o'출력
				{
					printf("o");
				}
				// fruit
				else if (i == fruitx && j == fruity)
				{
					printf("*");

				}
				// bomb
				else if ((i == bomb[0].x && j == bomb[0].y)
					|| (i == bomb[1].x && j == bomb[1].y)
					|| (i == bomb[2].x && j == bomb[2].y)
					|| (i == bomb[3].x && j == bomb[3].y)) // bomb의 모든 좌표 확인
				{
					printf("▣");

				}
				// mystery
				else if (i == mysteryx && j == mysteryy)
				{
					if (mystery_cnt == 1) {
						printf("?");
					}
					else
					{
						printf(" ");
					}
				}
				//trap
				else if (i == trapx && j == trapy)
				{
					printf("◎");

				}
				//공백
				else
				{
					printf(" ");
				}
			}


		}
		printf("\n");
	}
	

	gotoxy(30, height);
	printf("press X to quit the game\n");
	
}


// Function to take the input 
void input()
{
	if (_kbhit()) {
		switch (getch()) {
		case 'a':
		case 'A':
			flag = 1;
			break;
		case 's':
		case 'S':
			flag = 2;
			break;
		case 'd':
		case 'D':
			flag = 3;
			break;
		case 'w':
		case 'W':
			flag = 4;
			break;
		case 'x':
		case 'X':
			gameover = 1;
			break;
		}
	}
}


// Function for the logic behind 
// each movement 
void logic()
{
	// 점수가 50이 증가할 때마다 레벨 +1
	if (score > 200) {
		if (st_level == 1) level = 5;
	}
	else if (score > 150) {
		if (st_level == 1) level = 4;
		if (st_level == 2) level = 5;
	}
	else if (score > 100) {
		if (st_level == 1) level = 3;
		if (st_level == 2) level = 4;
		if (st_level == 3) level = 5;
	}
	else if (score > 50) {
		if (st_level == 1) level = 2;
		if (st_level == 2) level = 3;
		if (st_level == 3) level = 4;
		if (st_level == 4) level = 5;
	}




	// 레벨에 따라 속도, bomb 개수 설정, level3, level5에서 bomb가 새로 생기면 (x, y 좌표가 NULL일 때) 좌표 설정
	switch (level) {
	default:
		speed = lvl1;
		bomb_cnt = 1;
		break;

	case 2:
		speed = lvl2;
		break;

	case 3:
		speed = lvl3;
		bomb_cnt = 2;
		if (bomb[1].x == NULL && bomb[1].y == NULL) {
			bomb[1].x = 0;
			bomb[1].y = 0;
			while (bomb[1].x == 0 || bomb[1].y == 0
				|| (bomb[1].x == fruitx && bomb[1].y == fruity)
				|| (bomb[1].x == mysteryx && bomb[1].y == mysteryy)
				|| (bomb[1].x == trapx && bomb[1].y == trapy)
				|| (bomb[1].x == bomb[0].x && bomb[1].y == bomb[0].y)
				|| (round_snake(head, bomb[1].x, bomb[1].y))) 
			{

				bomb[1].x = rand() % (height - 1);
				bomb[1].y = rand() % (width - 1);
			}
		}
		break;

	case 4:
		speed = lvl4;
		break;

	case 5:
		speed = lvl5;
		bomb_cnt = 4;


		if (bomb[2].x == NULL && bomb[2].y == NULL) {
			bomb[2].x = 0;
			bomb[2].y = 0;
			while (bomb[2].x == 0 || bomb[2].y == 0
				|| (bomb[2].x == fruitx && bomb[2].y == fruity)
				|| (bomb[2].x == mysteryx && bomb[2].y == mysteryy)
				|| (bomb[2].x == trapx && bomb[2].y == trapy)
				|| (bomb[2].x == bomb[0].x && bomb[2].y == bomb[0].y)
				|| (bomb[2].x == bomb[1].x && bomb[2].y == bomb[1].y)
				|| (round_snake(head, bomb[2].x, bomb[2].y))) 
			{

				bomb[2].x = rand() % (height - 1);
				bomb[2].y = rand() % (width - 1);
			}
		}

		if (bomb[2].x == NULL && bomb[2].y == NULL) {
			bomb[3].x = 0;
			bomb[3].y = 0;
			while (bomb[3].x == 0 || bomb[3].y == 0
				|| (bomb[3].x == fruitx && bomb[3].y == fruity)
				|| (bomb[3].x == mysteryx && bomb[3].y == mysteryy)
				|| (bomb[3].x == trapx && bomb[3].y == trapy)
				|| (bomb[3].x == bomb[0].x && bomb[3].y == bomb[0].y)
				|| (bomb[3].x == bomb[1].x && bomb[3].y == bomb[1].y)
				|| (bomb[3].x == bomb[2].x && bomb[3].y == bomb[2].y)
				|| (round_snake(head, bomb[3].x, bomb[3].y))) 
			{

				bomb[3].x = rand() % (height - 1);
				bomb[3].y = rand() % (width - 1);
			}
		}
		break;

	}




	Sleep(speed); // 속도 조절용 변수 speed (기본값 : lvl1 (300), 레벨에 따라 속도가 더 빨라진다.)
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
	if (head->x <= 0 || head->x >= height - 1
		|| head->y <= 0 || head->y >= width - 1)
		gameover = 1;

	if (round_snake(head, head->x, head->y)) // 몸통에 머리가 닿으면 종료
		gameover = 1;


	// If snake reaches the fruit 
	// then update the score 
	if ((head->x == fruitx && head->y == fruity) /*|| (round_snake(fruitx, fruity) == 1 )*/) {

		fruitx = 0;
		fruity = 0;

		while (fruitx == 0 || fruity == 0
			|| (fruitx == trapx && fruity == trapy)
			|| (fruitx == mysteryx && fruity == mysteryy)
			|| (fruitx == bomb[0].x && fruity == bomb[0].y)
			|| (fruitx == bomb[1].x && fruity == bomb[1].y)
			|| (fruitx == bomb[2].x && fruity == bomb[2].y)
			|| (fruitx == bomb[3].x && fruity == bomb[3].y)
			|| (round_snake(head, fruitx, fruity))) 
		{
			fruitx = rand() % (height - 1);
			fruity = rand() % (width - 1);
		}



		body_length++;
		insert_body(head, create_body(head->x, head->y, head->direction, body_length)); // fruit을 먹었을 때 몸통 추가


		score += 10;
	}



	// snake가 bomb에 닿았을 때
	for (int i = 0; i < bomb_cnt; i++) {
		if ((head->x == bomb[i].x && head->y == bomb[i].y) /*|| (round_snake(bombx, bomby) == 1)*/) {
			change_score(2, -50);

			bomb[i].x = 0;
			bomb[i].y = 0;
			while (bomb[i].x == 0 || bomb[i].y == 0
				|| (bomb[i].x == fruitx && bomb[i].y == fruity)
				|| (bomb[i].x == mysteryx && bomb[i].y == mysteryy)
				|| (bomb[i].x == trapx && bomb[i].y == trapy)
				|| (i != 0 &&(bomb[i].x == bomb[0].x && bomb[i].y == bomb[0].y))
				|| (i != 1 &&(bomb[i].x == bomb[1].x && bomb[i].y == bomb[1].y))
				|| (i != 2 && (bomb[i].x == bomb[2].x && bomb[i].y == bomb[2].y))
				|| (i != 3 && (bomb[i].x == bomb[3].x && bomb[i].y == bomb[3].y))
				|| (round_snake(head, bomb[i].x, bomb[i].y))) 
			{

				bomb[i].x = rand() % (height - 1);
				bomb[i].y = rand() % (width - 1);
			}
		}
	}


	// snake가 event(mystery)에 닿았을 때
	if ((head->x == mysteryx && head->y == mysteryy) /*|| (round_snake(eventx, eventy) == 1)*/) {
		mystery_cnt--;

		// event(mystery)
		mysteryx = 0;
		mysteryy = 0;

		if (rand() % 100 < 10) { // mystery 생성은 10% 확률
			while (mysteryx == 0 || mysteryy == 0
				|| (mysteryx == fruitx && mysteryy == fruity)
				|| (mysteryx == trapx && mysteryy == trapy)
				|| (mysteryx == bomb[0].x && mysteryy == bomb[0].y)
				|| (mysteryx == bomb[1].x && mysteryy == bomb[1].y)
				|| (mysteryx == bomb[2].x && mysteryy == bomb[2].y)
				|| (mysteryx == bomb[3].x && mysteryy == bomb[3].y)
				|| (round_snake(head, mysteryx, mysteryy)))
			{
				mysteryx = rand() % (height - 1);
				mysteryy = rand() % (width - 1);
			}

			mystery_cnt = 1;
		}
			

		int effect = rand() % 3;
		switch (effect) {
		case 0: // 점수 증가
			change_score(1, random_num(50));
			break;

		case 1: // 점수 감소
			change_score(1, -random_num(50));
			break;

		case 2: // 몸통 길이 증가
			body_add(random_num(5));
			break;
		}



	}



	// snake가 trap에 닿았을 때
	if ((head->x == trapx && head->y == trapy)) {
		int cut_num = body_length == 1 ? 0 : random_num(body_length - 1); // 1~(몸통길이-1) 중 랜덤한 수 만큼 절단 (머리 제외)

		body* prev = head;
		body* cur = head->next;

		// trap 좌표 재설정
		trapx = 0;
		trapy = 0;

		while (trapx == 0 || trapy == 0
			|| (trapx == fruitx && trapy == fruity)
			|| (trapx == mysteryx && trapy == mysteryy)
			|| (trapx == bomb[0].x && trapy == bomb[0].y)
			|| (trapx == bomb[1].x && trapy == bomb[1].y)
			|| (trapx == bomb[2].x && trapy == bomb[2].y)
			|| (trapx == bomb[3].x && trapy == bomb[3].y)
			||(round_snake(head, trapx, trapy))) 
		{
			trapx = rand() % (height - 1);
			trapy = rand() % (width - 1);
		}



		while (cur != NULL) {
			if (cur->body_no > body_length - cut_num) {
				prev->next = NULL;
				find_tail(cut_bodies)->next = cur;

				break;
			}

			prev = cur;
			cur = cur->next;
		}


	}



	// snake가 끊어진 몸통에 닿았을 때, 게임 종료
	if (round_snake(cut_bodies, head->x, head->y) == 1) {
		gameover = 1;
	}
}


// 게임 종료시 화면, 기록을 보여주며 하이 스코어가 바뀌는지 알려준다.
void gameover_screen() {
	system("cls");

	draw_dlg_box(27, 27, 29);
	
	gotoxy(35, 2);
	printf("< MY RECORD >");
	gotoxy(33, 4);
	printf("Name\t: %s", player);
	gotoxy(33, 7);
	printf("Score\t: %d", score);

	gotoxy(30, 13);
	printf("=========================");

	gotoxy(34, 15);
	printf("< HIGH RECORD >");
	gotoxy(33, 17);
	printf("Name\t: %s", best_player);
	if (score > highest_score) {
		gotoxy(35, 18);
		printf("\t-> %s", player);
		textcolor(Red);
		printf("  new!");
		textcolor(White);
	}
		
	gotoxy(33, 20);
	printf("Score\t: %d", highest_score);
	if (score > highest_score) {
		gotoxy(35, 21);
		printf("\t-> %d", score);
		textcolor(Red);
		printf("  new!");
		textcolor(White);
	}
	gotoxy(29, height);

}


// record.txt 파일에서 기록을 읽어와 최고 점수와 플레이어를 화면에 띄울 수 있도록 하는 함수
int read_record() {
	FILE* fp = NULL;

	fopen_s(&fp, "record.txt", "rt");

	fscanf_s(fp, "%s : %d ", best_player, 20, &highest_score);

	fclose(fp);

	return 0;
}


// 게임 종료 시점에 현재 플레이어의 점수와 최고 점수를 비교하여 최고 점수를 갱신 혹은 유지하는 함수, record.txt 파일에 점수를 저장한다.
int write_record() {
	if (score > highest_score) {
		FILE* fp = NULL;

		fopen_s(&fp, "record.txt", "wt");

		fprintf(fp, "                                                                                                       \n");
		fprintf(fp, "%s : %d \n", player, score);

		fclose(fp);
	}
}

/// 함수 정의 종료 ///