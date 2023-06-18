#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include<conio.h>
#include<string.h>
#include"block.h"

#define GBOARD_ORIGIN_X 4
#define GBOARD_ORIGIN_Y 2
#define GBOARD_WIDTH 10
#define GBOARD_HEIGHT 20

int block_id;
int speed = 20;
int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2] = { 0 }; //배열로 나타낸 게임 보드

void SetCurrentCursorPos(int x, int y)
{
	COORD position = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}//현재 커서 위치 setter

void RemoveCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}//반짝 거리는 커서 안보이게

void DrawGameboard()
{
	int x, y;

	for (y = 0; y <= GBOARD_HEIGHT; y++) {
		SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);

		if (y == GBOARD_HEIGHT) {
			printf("└");
		}

		else {
			printf("|");
		}
	}

	for (y = 0; y <= GBOARD_HEIGHT; y++) {
		SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * (GBOARD_WIDTH + 1), GBOARD_ORIGIN_Y + y);

		if (y == GBOARD_HEIGHT) {
			printf("┘");
		}

		else {
			printf("|");
		}
	}

	for (x = 1; x < 2 * (GBOARD_WIDTH + 1); x++) {
		SetCurrentCursorPos(GBOARD_ORIGIN_X + x, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);

		printf("-");
	}
}//콘솔 창에서 그려지는 게임보드 출력

int main()
{
	COORD curPos;

	int x, y;
	int stop;
	int cnt = 0;

	for (y = 0; y < GBOARD_HEIGHT; y++) {
		gameBoardInfo[y][0] = 1;
		gameBoardInfo[y][GBOARD_WIDTH + 1] = 1;
	}

	for (x = 0; x < GBOARD_WIDTH + 2; x++) {
		gameBoardInfo[GBOARD_HEIGHT][x] = 1;
	}
	RemoveCursor();

	DrawGameboard();

	return 0;
}