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
int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2] = { 0 }; //�迭�� ��Ÿ�� ���� ����

void SetCurrentCursorPos(int x, int y)
{
	COORD position = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}//���� Ŀ�� ��ġ setter

COORD GetCurrentCursorPos()
{
	COORD curPoint;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}//���� Ŀ�� ��ġ getter

void RemoveCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}//��¦ �Ÿ��� Ŀ�� �Ⱥ��̰�

void DrawGameboard()
{
	int x, y;

	for (y = 0; y <= GBOARD_HEIGHT; y++) {
		SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);

		if (y == GBOARD_HEIGHT) {
			printf("��");
		}

		else {
			printf("|");
		}
	}

	for (y = 0; y <= GBOARD_HEIGHT; y++) {
		SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * (GBOARD_WIDTH + 1), GBOARD_ORIGIN_Y + y);

		if (y == GBOARD_HEIGHT) {
			printf("��");
		}

		else {
			printf("|");
		}
	}

	for (x = 1; x < 2 * (GBOARD_WIDTH + 1); x++) {
		SetCurrentCursorPos(GBOARD_ORIGIN_X + x, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);

		printf("-");
	}
}//�ܼ� â���� �׷����� ���Ӻ��� ���

void ShowBlock(char blockInfo[4][4])
{
	int x, y;
	COORD curPos = GetCurrentCursorPos();

	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);
			if (blockInfo[y][x] == 1) {
				printf("��");
			}
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}//���޵� block info �� �°� block ���

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
	while (1) {
		SetCurrentCursorPos(12, 2);
		curPos = GetCurrentCursorPos();
		srand((unsigned int)time(NULL));
		block_id = (rand() % 7) * 4;
		
		ShowBlock(blockModel[block_id]);
	}
	return 0;
}