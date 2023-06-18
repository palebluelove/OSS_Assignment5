#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include<conio.h>
#include<string.h>
#include"block.h"

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACE 32
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

COORD GetCurrentCursorPos()
{
	COORD curPoint;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}//현재 커서 위치 getter

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

	for (x = 1; x < 2 * (GBOARD_WIDTH + 1) - 1; x++) {
		SetCurrentCursorPos(GBOARD_ORIGIN_X + x, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);

		printf("-");
	}
}//콘솔 창에서 그려지는 게임보드 출력

void ShowBlock(char blockInfo[4][4])
{
	int x, y;
	COORD curPos = GetCurrentCursorPos();

	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);
			if (blockInfo[y][x] == 1) {
				printf("■");
			}
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}//전달된 block info 에 맞게 block 출력

void DeleteBlock(char blockInfo[4][4])
{
	int x, y;
	COORD curPos = GetCurrentCursorPos();

	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);

			if (blockInfo[y][x] == 1) {
				printf("  ");
			}
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}//블록의 이동, 회전에 따라 새로운 블록을 그려야 한다. 그 전에 현재 블록 삭제

int BlockDown(COORD* curPos)
{
	int check = DetectCollision(curPos->X, curPos->Y + 1, blockModel[block_id]);

	if (check == 1) {
		DeleteBlock(blockModel[block_id]);
		curPos->Y += 1;
		SetCurrentCursorPos(curPos->X, curPos->Y);
		ShowBlock(blockModel[block_id]);
		return 1;
	}
	return 0;
}//반환값의 이유 : 바닥면과 충돌이 일어났다면 이동을 멈추고 블록을 게임보드에 그려야 한다.

void ShiftRight(COORD* curPos)
{
	if (!DetectCollision(curPos->X + 2, curPos->Y, blockModel[block_id])) {
		return;
	}//오른쪽으로 한 칸 옮겼을 때 충돌이 일어난다면?
	DeleteBlock(blockModel[block_id]);
	curPos->X += 2;
	SetCurrentCursorPos(curPos->X, curPos->Y);
	ShowBlock(blockModel[block_id]);
}

void ShiftLeft(COORD* curPos)
{
	if (!DetectCollision(curPos->X - 2, curPos->Y, blockModel[block_id])) {
		return;
	}
	DeleteBlock(blockModel[block_id]);
	curPos->X -= 2;
	SetCurrentCursorPos(curPos->X, curPos->Y);
	ShowBlock(blockModel[block_id]);
}

void BlockUp(COORD* curPos)
{
	DeleteBlock(blockModel[block_id]);
	curPos->Y -= 1;
	SetCurrentCursorPos(curPos->X, curPos->Y);
	ShowBlock(blockModel[block_id]);
}

void RotateBlock()
{
	int block_senior = block_id - block_id % 4;;
	int block_rotated = block_senior + (block_id + 1) % 4;
	COORD curPos = GetCurrentCursorPos();
	if (!DetectCollision(curPos.X, curPos.Y, blockModel[block_rotated])) {
		return 0;
	}//충돌이 일어난다면 회전 불가
	DeleteBlock(blockModel[block_id]);
	block_id = block_rotated;
	ShowBlock(blockModel[block_id]);
}//블록 회전시키기

void ReverseRotateBlock()
{
	int block_senior = block_id - block_id % 4;

	DeleteBlock(blockModel[block_id]);

	block_id = block_senior + (4 + block_id - 1) % 4;

	ShowBlock(blockModel[block_id]);

}

void SpaceDown(COORD* curPos) {

	while (BlockDown(curPos));

}//블록이 바닥에 충돌할 때까지 BlockDown 함수 실행

void ProcessKeyInput(COORD* curPos)
{

	int key;
	int i;

	for (i = 0; i < 20; i++) {
		if (_kbhit() != 0) {
			key = _getch();
			switch (key) {
			case LEFT:
				ShiftLeft(curPos);
				break;
			case RIGHT:
				ShiftRight(curPos);
				break;
			case UP:
				RotateBlock();
				break;
			case SPACE:
				SpaceDown(curPos);
				break;
			default:
				break;
			}
		}
		Sleep(speed);
	}
}//사용자 입력에 대한 명령어 처리

int DetectCollision(int posX, int posY, char blockModel[4][4])
{
	int x, y;

	int arrX = (posX - GBOARD_ORIGIN_X) / 2;
	int arrY = posY - GBOARD_ORIGIN_Y;
	//배열에 입력한 게임보드와 콘솔창의 게임보드는 출발지점이 다르기 때문에
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			if (blockModel[y][x] == 1 && gameBoardInfo[arrY + y][arrX + x] == 1) {
				return 0;
			}//블록과 게임 보드간 충돌이 발생
		}
	}

	return 1;
}

void AddBlockToBoard(COORD* curPos)
{
	int x, y, arrCurX, arrCurY;

	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			arrCurX = (curPos->X - GBOARD_ORIGIN_X) / 2;
			arrCurY = curPos->Y - GBOARD_ORIGIN_Y;

			if (blockModel[block_id][y][x] == 1) {
				gameBoardInfo[arrCurY + y][arrCurX + x] = 1;
			}
		}
	}
}//게임보드 배열에 새로 생긴 블록 추가

void RedrawBlocks()
{
	int x, y;
	int cursX, cursY;
	for (y = 0; y < GBOARD_HEIGHT; y++) {
		for (x = 1; x < GBOARD_WIDTH + 1; x++) {
			cursX = x * 2 + GBOARD_ORIGIN_X;
			cursY = y + GBOARD_ORIGIN_Y;
			SetCurrentCursorPos(cursX, cursY);
			if (gameBoardInfo[y][x] == 1) {
				printf("■");
			}
			else {
				printf("  ");
			}
		}
	}
}

void RemoveFillUpLine(int* cnt) {
	int line, x, y;

	for (y = GBOARD_HEIGHT - 1; y > 0; y--) {
		for (x = 1; x < GBOARD_WIDTH + 1; x++) {
			if (gameBoardInfo[y][x] != 1) {
				break;
			}
		}
		if (x == GBOARD_WIDTH + 1) {
			for (line = 0; y - line > 0; line++) {
				memcpy(&gameBoardInfo[y - line][1], &gameBoardInfo[y - line - 1][1], GBOARD_WIDTH * sizeof(int));
			}
			y++;
			*cnt += 10;
		}
	}
	RedrawBlocks();

}//한 칸 채우면 삭제

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

		while (1) {
			stop = BlockDown(&curPos);
			if (stop == 0) {
				AddBlockToBoard(&curPos);
				RemoveFillUpLine(&cnt);
				break;
			}
			ProcessKeyInput(&curPos);
		}
		SetCurrentCursorPos(30, 11);
	}
	return 0;
}