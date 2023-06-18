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

	for (x = 1; x < 2 * (GBOARD_WIDTH + 1) - 1; x++) {
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
}//����� �̵�, ȸ���� ���� ���ο� ����� �׷��� �Ѵ�. �� ���� ���� ��� ����

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
}//��ȯ���� ���� : �ٴڸ�� �浹�� �Ͼ�ٸ� �̵��� ���߰� ����� ���Ӻ��忡 �׷��� �Ѵ�.

int DetectCollision(int posX, int posY, char blockModel[4][4])
{
	int x, y;

	int arrX = (posX - GBOARD_ORIGIN_X) / 2;
	int arrY = posY - GBOARD_ORIGIN_Y;
	//�迭�� �Է��� ���Ӻ���� �ܼ�â�� ���Ӻ���� ��������� �ٸ��� ������
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			if (blockModel[y][x] == 1 && gameBoardInfo[arrY + y][arrX + x] == 1) {
				return 0;
			}//��ϰ� ���� ���尣 �浹�� �߻�
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
}//���Ӻ��� �迭�� ���� ���� ��� �߰�

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
				printf("��");
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

}//�� ĭ ä��� ����

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
		}
	}
	return 0;
}