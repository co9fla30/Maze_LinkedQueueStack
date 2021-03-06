#define _CRT_SECURE_NO_WARNINGS
#include "Link.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <Windows.h>
using namespace std;

class Maze
{
	char** map = NULL;
	int width;
	int height;

	//비교를 위한 맵
	char** stackMap = NULL;
	char** queueMap = NULL;

	//위치 큐,스택 객체 생성
	LinkedQueue locQueue;
	LinkedStack stack;

public:

	Maze() { init(0, 0); }
	~Maze() {
		reset();
	}

	void init(int w, int h) //map 이차원 배열을 동적으로 할당
	{
		map = new char* [h];
		for (int i = 0; i < h; i++)
			map[i] = new char[w];
	}

	void reset() //이차원 배열 동적으로 해제
	{
		for (int i = 0; i < height; i++)
			delete[]map[i];
		delete[]map;
	}

	bool isValidLoc(int r, int c)
	{
		if (r < 0 || c < 0 || r >= height || c >= width) return false;
		else return map[r][c] == '0' || map[r][c] == 'x';
	}

	void roadMap(const char* filename, int num)
	{
		char c;
		ifstream fp(filename);
		if (fp.fail())
			printf("Error: 파일이 존재하지 않습니다.");

		//미로 크기 계산
		FILE* fd = fopen(filename, "r");
		int word = 0;
		height = 1;
		width = 0;
		while ((c = getc(fd)) != EOF) {
			if (c == '\n') height++;
			if (c == '0' || c == '1' || c == 'e' || c == 'x')
			{
				word++;
			}
		}
		width = word / height;

		//map 이차원배열 동적으로 할당
		init(width, height);

		//map 이차원배열에 미로 삽입
		for (int i = 0; i < height; i++)
		{
			for (int z = 0; z < width; z++)
			{
				fp >> c;
				map[i][z] = c;

				//큐, 스택에 입구 위치 삽입
				if (map[i][z] == 'e')
				{
					Location2D entry(i, z);
					if (num == 1)
						stack.push(new Node(i, z));
					else if (num == 2 || num == 5)
						locQueue.enqueue(new Node(i, z));
					else if (num == 4)
					{
						stack.push(new Node(i, z));
						locQueue.enqueue(new Node(i, z));
					}
				}
			}
		}
		fclose(fd);
		fp.close();
	}

	//미로 화면에 출력
	void printMap() {
		//printf("전체 미로의 크기 = %d X %d \n", width, height);
		printf("\n\n  [ Map ]\n\n");
		for (int i = 0; i < height; i++)
		{
			for (int z = 0; z < width; z++)
			{
				if (map[i][z] == '0') //길
				{
					cout << "  ";
				}
				else if (map[i][z] == '1') //벽
				{
					cout << "■";
				}
				else if (map[i][z] == 'e') //입구
				{
					cout << "o ";
				}
				else if (map[i][z] == 'x') //출구
				{
					cout << "x ";
				}
				else if (map[i][z] == '.') //지나온 길
				{
					cout << "* ";
				}

			}
			cout << endl;
		}
		cout << endl;
	}

	int get_Enterkey()
	{
		int key;
		key = _getch();
		if (key == 13)
		{
			return key;
		}
		else if (key == 224)
		{
			return _getch();
		}
		return 0;
	}

	/* 연결리스트 큐로 구현한 미로 찾기 입니다. (이하 queue_SearchExit())
	   사용자가 enter를 입력하면 상단 queue객체를 dequeue한 후 상,하,좌,우 순서로 갈 수 있는 길을 판단하여 enqueue하는 것을 반복합니다.
	   queue는 선입선출이기에 미로의 모든 길을 탐색합니다(너비 우선 탐색).
	   만약 큐의 상단 객체(front)가 출구이면 종료됩니다.
	   count값은 dequeue할 때 마다 증가합니다.
	*/

	void queue_SearchExit()
	{
		int count = 0;
		while (locQueue.isEmpty() == false) //큐가 비어있지 않는 동안
		{
			printMap();
			if (get_Enterkey() == 13) //엔터 키를 누르면
			{
				system("cls");

				locQueue.display();

				Location2D* here = locQueue.peek(); //큐의 상단 front 객체 복사
				locQueue.dequeue(); //큐 상단 객체 삭제

				count++;
				int r = here->row;
				int c = here->col;

				printf("Now Position: (%d,%d) \n", r, c); //상단 front 객체의 위치가 now position

				if (map[r][c] == 'x') { //출구이면 성공
					while (locQueue.isEmpty() == false)
						locQueue.dequeue();
					system("cls");
					printf("!탐색 성공!\n");
					printf("dequeue %d번 \n", count);
					printf("=====[ Result ]=====\n");
					printMap();
					return;
				}
				else if (map[r][c] != '.') //지나온 길이 아니면
				{
					map[r][c] = '.'; //지나온 길로 표시
					//갈 수 있는 길이라면 큐에 삽입
					if (isValidLoc(r - 1, c)) locQueue.enqueue(new Node(r - 1, c));
					if (isValidLoc(r + 1, c)) locQueue.enqueue(new Node(r + 1, c));
					if (isValidLoc(r, c - 1)) locQueue.enqueue(new Node(r, c - 1));
					if (isValidLoc(r, c + 1)) locQueue.enqueue(new Node(r, c + 1));
				}
			}
		}
		printf("미로 탐색 실패\n");
	}
	void stack_Search()	//스택 부분
	{
		int cnt = 0;
		while (stack.isEmpty() == false) {	//스택 비어있지 않을 경우
			printMap();
			if (get_Enterkey() == 13) {
				system("cls");
				Location2D* here = stack.peek();
				stack.pop();
				cnt++;
				int r = here->row;
				int c = here->col;
				printf("Now Position: (%d,%d) ", r, c);

				if (map[r][c] == 'x')
				{
					system("cls");
					printf("\n!탐색 성공!\n");
					printf("=====[ Result ]=====\n");
					printf("총 횟수 : %d\n", cnt);
					printMap();
					return;
				}
				else {
					map[r][c] = '.';
					if (isValidLoc(r - 1, c)) stack.push(new Node(r - 1, c));
					if (isValidLoc(r + 1, c)) stack.push(new Node(r + 1, c));
					if (isValidLoc(r, c - 1)) stack.push(new Node(r, c - 1));
					if (isValidLoc(r, c + 1)) stack.push(new Node(r, c + 1));
				}
			}
		}
		printf("미로 탐색 실패\n");
	}


	/*연결리스트 큐를 사용해 사용자가 직접 미로를 찾을 수 있는 게임을 만들어 봤습니다. (이하 QueueMazeGame())

	 1. 먼저 입구 위치를 큐에 삽입하고(roadMap()에서 삽입됩니다.)
	 2. 큐가 비어있지 않는 동안
	 -> 1. 큐에 상단 객체를 복사 (이전위치)
		2. 방향키를 입력받고
		3. 해당 방향의 위치가 출구이면 큐의 상단 객체(이전위치)를 삭제한 후 다시 이동한 위치를 큐에 삽입하고 삭제합니다.
		   이전위치는 다시 길로, 이동한 위치는 현재 위치로 표시합니다. 그리고 게임이 종료됩니다. 이 때 queue에는 아무 요소가 남아있지 않습니다.
		4. 해당 방향의 위치가 갈 수 있는 길이고, 벽이 아니면 큐의 맨앞 요소(이전 위치)를 삭제한 후 다시 이동한 위치를 큐에 삽입합니다.
		   이전위치는 다시 길로, 이동한 위치는 현재 위치로 표시합니다.
	 3. count는 dequeue 할 때 마다 증가합니다. (입구~출구까지 사용자가 지나간 모든 위치를 큐에 삽입하기에, 방향키의 입력 횟수와 동일합니다.)
	*/

	void QueueMazeGame()
	{
		int count = 0;
		while (locQueue.isEmpty() == false) //큐가 비어있지 않는 동안
		{
			Location2D* here = locQueue.peek(); //큐의 상단 front 객체 복사
			int r = here->row;
			int c = here->col;

			int key = get_Enterkey();

			switch (key)
			{
			case 72: //상
				if (map[r - 1][c] == 'x') //다음 위치가 출구이면 성공
				{
					map[r][c] = '0'; //이전 위치 길로 변경
					map[r - 1][c] = '.';  //이동한 위치
					locQueue.dequeue(); //큐 상단 객체 삭제
					locQueue.enqueue(new Node(r - 1, c)); //이동한 위치 큐에 삽입
					locQueue.dequeue(); //큐 상단 객체 삭제
					count++;
					system("cls");
					printf("\n!탐색 성공!\n");
					printf("=====[ Result ]=====\n");
					printf("이동 %d번 ", count);
					printMap();
					return;
				}
				else if (isValidLoc(r - 1, c) && map[r - 1][c] != '1') //갈 수 있는 위치이고, 벽이 아니면
				{
					system("cls");
					map[r][c] = '0'; //이전 위치 길로 변경
					map[r - 1][c] = '.';  //이동한 위치
					locQueue.dequeue(); //큐 상단 객체 삭제
					locQueue.enqueue(new Node(r - 1, c)); //이동한 위치 큐에 삽입
					printMap();
					count++;
				}
				break;
			case 80: //하
				if (map[r + 1][c] == 'x')
				{
					map[r][c] = '0';
					map[r + 1][c] = '.';
					locQueue.dequeue();
					locQueue.enqueue(new Node(r + 1, c));
					locQueue.dequeue();
					count++;
					system("cls");
					printf("\n!탐색 성공!\n");
					printf("=====[ Result ]=====\n");
					printf("이동 %d번 ", count);
					printMap();
					return;
				}
				else if (isValidLoc(r + 1, c) && map[r + 1][c] != '1')
				{
					system("cls");
					map[r][c] = '0';
					map[r + 1][c] = '.';
					locQueue.dequeue();
					locQueue.enqueue(new Node(r + 1, c));
					printMap();
					count++;
				}
				break;
			case 75: //좌
				if (map[r][c - 1] == 'x')
				{
					map[r][c] = '0';
					map[r][c - 1] = '.';
					locQueue.dequeue();
					locQueue.enqueue(new Node(r, c - 1));
					locQueue.dequeue();
					count++;
					system("cls");
					printf("\n!탐색 성공!\n");
					printf("=====[ Result ]=====\n");
					printf("이동 %d번 ", count);
					printMap();
					return;
				}
				else if (isValidLoc(r, c - 1) && map[r][c - 1] != '1')
				{
					system("cls");
					map[r][c] = '0';
					map[r][c - 1] = '.';
					locQueue.dequeue();
					locQueue.enqueue(new Node(r, c - 1));
					printMap();
					count++;
				}
				break;
			case 77: //우
				if (map[r][c + 1] == 'x')
				{
					map[r][c] = '0';
					map[r][c + 1] = '.';
					locQueue.dequeue();
					locQueue.enqueue(new Node(r, c + 1));
					locQueue.dequeue();
					count++;
					system("cls");
					printf("\n!탐색 성공!\n");
					printf("=====[ Result ]=====\n");
					printf("이동 %d번 ", count);
					printMap();
					return;
				}
				else if (isValidLoc(r, c + 1) && map[r][c + 1] != '1')
				{
					system("cls");
					map[r][c] = '0';
					map[r][c + 1] = '.';
					locQueue.dequeue();
					locQueue.enqueue(new Node(r, c + 1));
					printMap();
					count++;
				}
				break;
			default:
				printf("방향키만 입력해주세요.\n");
				break;
			}
		}
	}

	/*
	기본미로와 다른 흐름으로 진행됩니다.
	기본 미로에서는 empty 상태가 아니어도 출구를 만난다면 탐색이 종료되었지만
    여기서는 출구를 만나면 empty상태가 될 때 까지 pop, dequeue 하고, 스택과 큐 모두가 empty상태가 되면 탐색이 종료됩니다.

	스택과 큐는 각각 map정보를 따로 가지며
	스택과 큐가 비어있지 않은동안 pop, dequeue를 하고
	상,하,좌,우의 길이 갈 수 있는 길이며, 지나 온 길이 아니라면 그 위치를 push, enqueue 합니다.
	이 때 만약 출구를 만난다면 스택과 큐가 공백 상태 일 때까지 pop, dequeue 합니다.
	*/

	void compare_init(int w, int h)
	{
		stackMap = new char* [h];
		queueMap = new char* [h];
		for (int i = 0; i < h; i++)
		{
			stackMap[i] = new char[w];
			queueMap[i] = new char[w];
		}
	}

	void compare_reset()
	{
		for (int i = 0; i < height; i++)
		{
			delete[]stackMap[i];
			delete[]queueMap[i];
		}
		delete[]stackMap;
		delete[]queueMap;
	}

	bool stack_isValidLoc(int sr, int sc)
	{
		if (sr < 0 || sc < 0 || sr >= height || sc >= width) return false;
		else return stackMap[sr][sc] == '0' || stackMap[sr][sc] == 'x';
	}

	bool queue_isValidLoc(int qr, int qc)
	{
		if (qr < 0 || qc < 0 || qr >= height || qc >= width) return false;
		else return queueMap[qr][qc] == '0' || queueMap[qr][qc] == 'x';
	}

	void compare_roadMap(const char* filename)
	{
		char c;
		ifstream fp(filename);
		if (fp.fail())
			printf("Error: 파일이 존재하지 않습니다.");

		//미로 크기 계산
		FILE* fd = fopen(filename, "r");
		int word = 0;
		height = 1;
		width = 0;
		while ((c = getc(fd)) != EOF) {
			if (c == '\n') height++;
			if (c == '0' || c == '1' || c == 'e' || c == 'x')
			{
				word++;
			}
		}
		width = word / height;

		compare_init(width, height);

		for (int i = 0; i < height; i++)
		{
			for (int z = 0; z < width; z++)
			{
				fp >> c;
				queueMap[i][z] = c;
				stackMap[i][z] = c;

				if (queueMap[i][z] == 'e') //입구 위치
				{
					stack.push(new Node(i, z));
					locQueue.enqueue(new Node(i, z));
				}
			}
		}
		fp.close();
	}
	void comparePrintMap() {
		printf("\n\n  [ StackMap ]\n\n");
		for (int i = 0; i < height; i++)
		{
			for (int z = 0; z < width; z++)
			{
				if (stackMap[i][z] == '0') //길
				{
					cout << "  ";
				}
				else if (stackMap[i][z] == '1') //벽
				{
					cout << "■";
				}
				else if (stackMap[i][z] == 'e') //입구
				{
					cout << "o ";
				}
				else if (stackMap[i][z] == 'x') //출구
				{
					cout << "x ";
				}
				else if (stackMap[i][z] == '.') //지나온 길
				{
					cout << "* ";
				}

			}
			cout << endl;
		}
		cout << endl;
		printf("\n\n  [ QueueMap ]\n\n");
		for (int i = 0; i < height; i++)
		{
			for (int z = 0; z < width; z++)
			{
				if (queueMap[i][z] == '0') //길
				{
					cout << "  ";
				}
				else if (queueMap[i][z] == '1') //벽
				{
					cout << "■";
				}
				else if (queueMap[i][z] == 'e') //입구
				{
					cout << "o ";
				}
				else if (queueMap[i][z] == 'x') //출구
				{
					cout << "x ";
				}
				else if (queueMap[i][z] == '.') //지나온 길
				{
					cout << "* ";
				}

			}
			cout << endl;
		}
		cout << endl;
	}

	void compare()
	{
		int stackCount = 0;
		int queueCount = 0;

		while (1)
		{
			comparePrintMap();
			Sleep(300);
			system("cls");

			if (locQueue.isEmpty() == true && stack.isEmpty() == true)
			{
				printf("\n!탐색 성공!\n");
				printf("=====[ Result ]=====\n");
				printf("스택 탐색 %d회, 큐 탐색 %d회 \n", stackCount, queueCount);
				if (stackCount > queueCount)
					printf("큐(너비 우선 탐색)가 더 빠릅니다.\n");
				else if (stackCount < queueCount)
					printf("스택(깊이 우선 탐색)이 더 빠릅니다.\n");
				else if (stackCount == queueCount)
					printf("스택과 큐의 탐색 횟수가 동일합니다.\n");

				comparePrintMap();
				return;
			}

			if (stack.isEmpty() == false)
			{
				Node* hereStack = stack.peek();
				stack.pop();
				int sr = hereStack->row;
				int sc = hereStack->col;
				stackCount++;
				printf("Stack Now Position: (%d,%d) \n", sr, sc);

				if (stackMap[sr][sc] == 'x')
				{
					while (stack.isEmpty() == false)
						stack.pop();
				}
				else if (stackMap[sr][sc] != '.')
				{
					stackMap[sr][sc] = '.';
					if (stack_isValidLoc(sr - 1, sc)) stack.push(new Node(sr - 1, sc));
					if (stack_isValidLoc(sr + 1, sc)) stack.push(new Node(sr + 1, sc));
					if (stack_isValidLoc(sr, sc - 1)) stack.push(new Node(sr, sc - 1));
					if (stack_isValidLoc(sr, sc + 1)) stack.push(new Node(sr, sc + 1));
				}
			}
			if (locQueue.isEmpty() == false)
			{
				Location2D* hereQueue = locQueue.peek();
				locQueue.dequeue();
				int qr = hereQueue->row;
				int qc = hereQueue->col;
				queueCount++;
				printf("Queue Now Position: (%d,%d) \n", qr, qc);

				if (queueMap[qr][qc] == 'x')
				{
					while (locQueue.isEmpty() == false)
						locQueue.dequeue();
				}
				else if (queueMap[qr][qc] != '.')
				{
					queueMap[qr][qc] = '.';
					if (queue_isValidLoc(qr - 1, qc)) locQueue.enqueue(new Node(qr - 1, qc));
					if (queue_isValidLoc(qr + 1, qc)) locQueue.enqueue(new Node(qr + 1, qc));
					if (queue_isValidLoc(qr, qc - 1)) locQueue.enqueue(new Node(qr, qc - 1));
					if (queue_isValidLoc(qr, qc + 1)) locQueue.enqueue(new Node(qr, qc + 1));
				}
			}
		}
		printf("미로 탐색 실패\n");
	}

	void resultSatckWay()
	{
		printf("[ Stack 경로 ]\n");
		while (stack.isEmpty() == false) {
			Location2D* here = stack.peek();
			stack.pop();

			int r = here->row;
			int c = here->col;

			printf("(%d,%d) ", r, c);

			if (map[r][c] == 'x')
			{
				printf("\n!탐색 성공!\n");
				return;
			}
			else {
				map[r][c] = '.';
				if (isValidLoc(r - 1, c)) stack.push(new Node(r - 1, c));
				if (isValidLoc(r + 1, c)) stack.push(new Node(r + 1, c));
				if (isValidLoc(r, c - 1)) stack.push(new Node(r, c - 1));
				if (isValidLoc(r, c + 1)) stack.push(new Node(r, c + 1));
			}
		}
		printf("미로 탐색 실패\n");
	}
	void resultQueueWay()
	{
		printf("\n[ Queue 경로 ]\n");
		while (locQueue.isEmpty() == false)
		{
			Location2D* here = locQueue.peek();
			locQueue.dequeue();
			int r = here->row;
			int c = here->col;

			printf("(%d,%d) ", r, c);

			if (map[r][c] == 'x') {
				while (locQueue.isEmpty() == false)
					locQueue.dequeue();
				printf("\n!탐색 성공!\n");
				return;
			}
			else if (map[r][c] != '.')
			{
				map[r][c] = '.';
				if (isValidLoc(r - 1, c)) locQueue.enqueue(new Node(r - 1, c));
				if (isValidLoc(r + 1, c)) locQueue.enqueue(new Node(r + 1, c));
				if (isValidLoc(r, c - 1)) locQueue.enqueue(new Node(r, c - 1));
				if (isValidLoc(r, c + 1)) locQueue.enqueue(new Node(r, c + 1));
			}
		}
		printf("미로 탐색 실패\n\n");
	}
};