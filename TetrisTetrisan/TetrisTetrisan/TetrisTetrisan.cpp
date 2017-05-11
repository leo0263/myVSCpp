#define _CRT_SECURE_NO_WARNINGS	// Ignore build errors when using unsafe functions in Visual Studio.
#include <stdio.h>

// debug
#include <iostream>
using namespace std;

#define BOARD_MAX_WIDTH     10
#define BLOCK_MAX_SIZE      3
#define CMD_ROTATE          100
#define CMD_MOVE            200

extern void init(int width);
extern void newBlock(int block[BLOCK_MAX_SIZE][BLOCK_MAX_SIZE], int width, int height);
extern void rotate(int angle);
extern void move(int distance);
extern int land();

// debug
extern void printBlock();

int main(void)
{
	setbuf(stdout, NULL);

	freopen("input.txt", "r", stdin);

	int testCnt;
	double totalScore = 0.0;
	int block[BLOCK_MAX_SIZE][BLOCK_MAX_SIZE];

	scanf("%d", &testCnt);
	for (int tc = 1; tc <= testCnt; ++tc)
	{
		int boardWidth, blockCnt;
		scanf("%d %d", &boardWidth, &blockCnt);

		int score = blockCnt;

		init(boardWidth);

		for (int i = 0; i < blockCnt; ++i) {
			for (int h = 0; h < BLOCK_MAX_SIZE; ++h) {
				for (int w = 0; w < BLOCK_MAX_SIZE; ++w) {
					block[h][w] = 0;
				}
			}

			int width, height;
			scanf("%d %d", &width, &height);

			for (int h = 0; h < height; ++h) {
				for (int w = 0; w < width; ++w) {
					scanf("%d", &block[h][w]);
				}
			}

			newBlock(block, width, height);

			int cmdCnt, cmd, option;
			scanf("%d", &cmdCnt);
			for (int j = 0; j < cmdCnt; ++j) {
				scanf("%d %d", &cmd, &option);

				switch (cmd) {
				case CMD_ROTATE:
					rotate(option);
					break;
				case CMD_MOVE:
					move(option);
					break;
				default:
					printf("cmd error\n");
					break;
				}
			}

			//printBlock();

			int answer, result;
			scanf("%d", &answer);
			result = land();
			if (answer != result) {
				--score; // wrong answer
			}
		}

		printf("#%d %d\n", tc, score);
		totalScore += (double)score / blockCnt * 100;
	}

	printf("Total Score : %.3lf\n", totalScore / testCnt);

	return 0;
}

#define BOARD_MAX_HEIGHT    200
#define BOARD_MAX_WIDTH     10
#define BLOCK_MAX_SIZE      3
#define ROTATE_90           1
#define ROTATE_180          2
#define ROTATE_270          3
#define ROTATE_360          4

int board[BOARD_MAX_HEIGHT][BOARD_MAX_WIDTH];
int nBlock[BLOCK_MAX_SIZE][BLOCK_MAX_SIZE];
int nBlockW, nBlockH, nBlockD;
int bWidth;

//void printBlock() {
//	for (int y = 0; y < nBlockH; y++) {
//		for (int x = 0; x < nBlockW; x++) {
//			cout << nBlock[y][x];
//		}
//		cout << endl;
//	}
//}

void init(int width) {
	for (int y = 0; y < BOARD_MAX_HEIGHT; y++)
		for (int x = 0; x < width; x++)
			board[y][x] = 0;

	bWidth = width;

	//// debug
	//cout << "initing board [" << width << "]" << endl;
}

void newBlock(int block[BLOCK_MAX_SIZE][BLOCK_MAX_SIZE], int width, int height) {
	for (int y = 0; y < BLOCK_MAX_SIZE; y++)
		for (int x = 0; x < BLOCK_MAX_SIZE; x++)
			nBlock[y][x] = 0;

	nBlockW = width;
	nBlockH = height;
	nBlockD = 0;

	for (int y = 0; y < nBlockH; y++)
		for (int x = 0; x < nBlockW; x++)
			nBlock[y][x] = block[y][x];

	//// debug
	//cout << "creating new block : " << endl;
	//printBlock();
}

void rotate(int angle) {  // 1: ROTATE_90, 2: ROTATE_180, 3: ROTATE_270, 4: ROTATE_360
	int tBlock[BLOCK_MAX_SIZE][BLOCK_MAX_SIZE];
	for (int y = 0; y < nBlockH; y++)
		for (int x = 0; x < nBlockW; x++)
			tBlock[y][x] = 0;

	int temp;
	bool rotated = false;

	switch (angle) {
	case 1:
		//cout << "rotating 90" << endl;
		for (int y = 0; y < nBlockH; y++)
			for (int x = 0; x < nBlockW; x++) {
				tBlock[x][y] = nBlock[nBlockH - y - 1][x];
			}
		temp = nBlockH;
		nBlockH = nBlockW;
		nBlockW = temp;
		rotated = true;

		break;

	case 2:
		//cout << "rotating 180" << endl;
		for (int y = 0; y < nBlockH; y++)
			for (int x = 0; x < nBlockW; x++)
				tBlock[y][x] = nBlock[nBlockH - y - 1][nBlockW - x - 1];
		rotated = true;
		break;

	case 3:
		//cout << "rotating 270" << endl;
		for (int y = 0; y < BLOCK_MAX_SIZE; y++)
			for (int x = 0; x < nBlockW; x++)
				tBlock[y][x] = nBlock[x][nBlockW - y - 1];
		temp = nBlockH;
		nBlockH = nBlockW;
		nBlockW = temp;
		rotated = true;
		break;

	default:
		break;
	}

	if (rotated) {
		for (int y = 0; y < BLOCK_MAX_SIZE; y++)
			for (int x = 0; x < BLOCK_MAX_SIZE; x++)
				nBlock[y][x] = tBlock[y][x];
	}
	//printBlock();
}

void move(int distance) {
	nBlockD += distance;
}

bool checkHit(int posY, int posX) {
	bool isHit = false;
	for (int y = 0; y < nBlockH; y++) {
		for (int x = 0; x < nBlockW; x++) {
			if (posY + y <= 0) continue;
			if (posY + y >= BOARD_MAX_HEIGHT - 1) {
				isHit = true;
				break;
			}
			if (nBlock[y][x] == 1)
				if (board[posY + y + 1][posX + x] == 1) {
					isHit = true;
					break;
				}
		}
		if (isHit) break;
	}

	return isHit;
}

int land() {
	int answer = -1;

	// simulate fall
	int posX = nBlockD;
	int posY = 0 - nBlockH;
	bool hit = false;

	do {
		posY++;
		hit = checkHit(posY, posX);
	} while (!hit);
	//posY--;

	// place the block on board
	for (int y = 0; y < nBlockH; y++)
		for (int x = 0; x < nBlockW; x++) {
			if (posY + y < 0) continue;
			board[posY + y][posX + x] = nBlock[y][x] || board[posY + y][posX + x];
		}


	// remove any full line
	for (int y = BOARD_MAX_HEIGHT - 1; y >= 0; y--) {
		int isFull = true;
		for (int x = 0; x < bWidth; x++) if (board[y][x] == 0) isFull = false;
		if (isFull) {
			for (int nY = y; nY >= 1; nY--)
				for (int nX = 0; nX < bWidth; nX++)
					board[nY][nX] = board[nY - 1][nX];

			for (int nX = 0; nX < bWidth; nX++) board[0][nX] = 0;
			y++;
		}
	}

	////debug : print the bottom board
	//cout << "the block : shift <" << nBlockD << ">" << endl;
	//printBlock();
	//cout << "the board : " << endl;
	//for (int y = 0; y < 10; y++) {
	//	for (int x = 0; x < bWidth; x++) {
	//		cout << board[BOARD_MAX_HEIGHT - 10 + y][x];
	//	}
	//	cout << endl;
	//}

	for (int y = BOARD_MAX_HEIGHT - 1; y >= 0; y--) {
		int isEmpty = true;
		for (int x = 0; x < bWidth; x++) if (board[y][x] == 1) isEmpty = false;
		if (isEmpty) {
			answer = BOARD_MAX_HEIGHT - 1 - y;
			break;
		}
	}

	//// debug 
	//cout << "i got answer : " << answer << endl;

	return answer;
}
