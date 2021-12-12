#define _CRT_SECURE_NO_WARNINGS
#include "Maze.h"
#include <time.h>
using namespace std;

class MazeGame {
	char** map = NULL;
	int width;
	int height;
	int ER = 0;
	int EC = 0;
	Maze maze;
	LinkedStack stackMaze;
	LinkedStack enemy;
public:
	void in(int w, int h) //map 이차원 배열을 동적으로 할당
	{
		map = new char* [h];
		for (int i = 0; i < h; i++)
			map[i] = new char[w];
	}
	void ReadFile(const char* filename)	//파일 열기 및 크기 계산
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
		in(width, height);

		//map 이차원배열에 미로 삽입
		for (int i = 0; i < height; i++)
		{
			for (int z = 0; z < width; z++)
			{
				fp >> c;
				map[i][z] = c;
				if (map[i][z] == 'e')
				{
					stackMaze.push(new Node(i, z));
				}
			}
		}
		fclose(fd);
		fp.close();
	}
	void showMap() {
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
				else if (map[i][z] == '2')	//적
				{
					std::cout << "▲";
				}
			}
			cout << endl;
		}
		cout << endl;
	}
	//탐색 성공 시 출력
	void ReGame(int cnt)
	{
		cnt++;
		stackMaze.pop();
		system("cls");
		printf("\n!탐색 성공!\n");
		printf("=====[ Result ]=====\n");
		printf("이동: %d번", cnt);
		showMap();
		return;
	}
	void MakeE()	//적 위치 지정
	{
		for (int i = 1; i < height - 1; i++)
		{
			for (int z = width - 1; z > 0; z--)
			{
				if (map[i][z] == '0')
				{
					map[i][z] = '2';
					ER = z; EC = i;
					printf("%d %d", i, z);
					return;
				}
			}
		}
	}
	//미로 게임 ver2
	void MazeGameStack()
	{
		int num = 0;
		LinkedStack enemy;
		bool ene = false;
		MakeE();
		printf("Enemy is Coming!! Move.");
		//첫 위치 push
		enemy.push(new Node(EC, ER));
		showMap();
		int cnt = 0;

		while (stackMaze.isEmpty() == false)
		{
			Node* here = stackMaze.peek();
			int r = here->row;
			int c = here->col;
			Node* enemyHere = enemy.peek();

			srand((unsigned int)time(NULL));//랜덤 수
			int er = enemyHere->row;
			int ec = enemyHere->col;

			//적 랜덤으로 움직이기
			while (1) {
				int nu = rand() % 4;	//0,1,2,3 랜덤
				map[er][ec] = '0';

				switch (nu)
				{
				case 0:	//상 이동
					if (er - 1 >= 0 && ec >= 0 && er - 1 < height && ec < width && map[er - 1][ec] == '0')//상단이 통로인 경우
					{
						enemy.pop();
						enemy.push(new Node(er - 1, ec));
						map[er - 1][ec] = '2';
						break;
					}
					else if (er - 1 >= 0 && ec >= 0 && er - 1 < height && ec < width && map[er - 1][ec] == '.')	//자신이 적의 상단 위치한 경우 실패
					{
						enemy.pop();
						map[er - 1][ec] = '2';
						printf("==== You Lose!! ====\n");
						showMap();
						return;
					}
					else
						continue;
				case 1:	//하 이동
					if (er + 1 >= 0 && ec >= 0 && er + 1 < height && ec < width && map[er + 1][ec] == '0')//하단이 통로인 경우
					{
						enemy.pop();
						enemy.push(new Node(er + 1, ec));
						map[er + 1][ec] = '2';
						break;
					}
					else if (er + 1 >= 0 && ec >= 0 && er + 1 < height && ec < width && map[er + 1][ec] == '.')	//자신이 적의 하단에 위치한 경우 실패
					{
						enemy.pop();
						map[er + 1][ec] = '2';
						printf("==== You Lose!! ====\n");
						showMap();
						return;
					}
					else
						continue;
				case 2:	//좌 이동
					if (er >= 0 && ec - 1 >= 0 && er < height && ec - 1 < width && map[er][ec - 1] == '0')//좌측이 통로인 경우
					{
						enemy.pop();
						enemy.push(new Node(er, ec - 1));
						map[er][ec - 1] = '2';
						break;
					}
					else if (er >= 0 && ec - 1 >= 0 && er < height && ec - 1 < width && map[er][ec - 1] == '.')	//자신이 적의 좌측에 위치한 경우 실패
					{
						enemy.pop();
						map[er][ec - 1] = '2';
						printf("==== You Lose!! ====\n");
						showMap();
						return;
					}
					else
						continue;
				case 3:	//우 이동
					if (er >= 0 && ec + 1 >= 0 && er < height && ec + 1 < width && map[er][ec + 1] == '0')//우측이 통로인 경우
					{
						enemy.pop();
						enemy.push(new Node(er, ec + 1));
						map[er][ec + 1] = '2';
						break;
					}
					else if (er >= 0 && ec + 1 >= 0 && er < height && ec + 1 < width && map[er][ec + 1] == '.')	//자신이 적의 좌측에 위치한 경우 실패
					{
						enemy.pop();
						map[er][ec + 1] = '2';
						printf("==== You Lose!! ====\n");
						showMap();
						return;
					}
					else
						continue;
				}
				break;
			}

			//직접 움직이기
			printf("피해서 이동하세요.\n");
			printf("enter\n");
			int key;	//이동 방향 키보드 입력
			key = maze.get_Enterkey();

			switch (key)
			{
			case 72:	//상
				if (r - 1 >= 0 && c >= 0 && r - 1 < height && c < width && map[r - 1][c] == 'x') {	//상단 도착점인 경우 완료
					map[r][c] = '0';
					map[r - 1][c] = '.';
					ReGame(cnt); return;
				}
				else if (r - 1 >= 0 && c >= 0 && r - 1 < height && c < width && map[r - 1][c] == '0')	//상단 이동 가능 경우
				{
					map[r][c] = '0';
					map[r - 1][c] = '.';
					stackMaze.pop();
					stackMaze.push(new Node(r - 1, c));
				}
				else if (r - 1 >= 0 && c >= 0 && r - 1 < height && c < width && map[r - 1][c] == '2')	//상단 적이 있는 경우 실패
				{
					map[r][c] = '0';
					map[r - 1][c] = '2';
					system("cls");
					printf("==== You Lose!! ====\n");
					showMap();
					return;
				}
				break;
			case 80:	//하
				if (r + 1 >= 0 && c >= 0 && r + 1 < height && c < width && map[r + 1][c] == 'x') {	//하단 출구 성공
					map[r][c] = '0';
					map[r + 1][c] = '.';
					ReGame(cnt); return;
				}
				else if (r + 1 >= 0 && c >= 0 && r + 1 < height && c < width && map[r + 1][c] == '0')	//하단 통로
				{
					map[r][c] = '0';
					map[r + 1][c] = '.';
					stackMaze.pop();
					stackMaze.push(new Node(r + 1, c));
				}
				else if (r + 1 >= 0 && c >= 0 && r + 1 < height && c < width && map[r + 1][c] == '2')	//하단 적 위치한 경우 실패
				{
					map[r][c] = '0';
					map[r + 1][c] = '2';
					system("cls");
					printf("==== You Lose!! ====\n");
					showMap();
					return;
				}
				break;
			case 75:	//좌
				if (r >= 0 && c - 1 >= 0 && r < height && c - 1 < width && map[r][c - 1] == 'x') {	//좌측 출구 성공
					map[r][c] = '0';
					map[r][c - 1] = '.';
					ReGame(cnt); return;
				}
				else if (r >= 0 && c - 1 >= 0 && r < height && c - 1 < width && map[r][c - 1] == '0')	//좌측 통로
				{
					map[r][c] = '0';
					map[r][c - 1] = '.';
					stackMaze.pop();
					stackMaze.push(new Node(r, c - 1));
				}
				else if (r >= 0 && c - 1 >= 0 && r < height && c - 1 < width && map[r][c - 1] == '2')	//좌측 적 위치 실패
				{
					map[r][c] = '0';
					map[r][c - 1] = '2';
					system("cls");
					printf("==== You Lose!! ====\n");
					showMap();
					return;
				}
				break;
			case 77:	//우
				if (r >= 0 && c + 1 >= 0 && r < height && c + 1 < width && map[r][c + 1] == 'x') {	//우측 출구 성공
					map[r][c] = '0';
					map[r][c + 1] = '.';
					ReGame(cnt); return;
				}
				else if (r >= 0 && c + 1 >= 0 && r < height && c + 1 < width && map[r][c + 1] == '0')	//우측 통로
				{
					map[r][c] = '0';
					map[r][c + 1] = '.';
					stackMaze.pop();
					stackMaze.push(new Node(r, c + 1));
				}
				else if (r >= 0 && c + 1 >= 0 && r < height && c + 1 < width && map[r][c + 1] == '2')	//우측 적 위치 실패
				{
					map[r][c] = '0';
					map[r][c + 1] = '2';
					system("cls");
					printf("==== You Lose!! ====\n");
					showMap();
					return;
				}
				break;
			}
			cnt++;
			system("cls");
			showMap();
		}
	}

	//===========
	//미로3

	void ShowGame()
	{
		printf("\n\n  [ Map ]\n\n");
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (j > width / 2 - 2 && j < width / 2 + 1 && i < height - 1 && i>1)	//미로 가로 크기 / 2 - 2 보다 크고 미로 가로크기/2 +1 보다 작은 경우, 
				{																		//세로크기 -1 보다 작고 크기 1보다 클때 통로, 벽을 ?로 표시 
					if (map[i][j] == '0') //길
					{
						std::cout << "? ";
					}
					else if (map[i][j] == '1')	//벽
					{
						std::cout << "? ";
					}
					else if (map[i][j] == 'e')	//입구
					{
						std::cout << "o ";
					}
					else if (map[i][j] == 'x')	//출구
					{
						std::cout << "x ";
					}
					else if (map[i][j] == '.')	//지나간 곳
					{
						std::cout << "* ";
					}
				}
				else {
					if (map[i][j] == '0') //길
					{
						std::cout << "  ";
					}
					else if (map[i][j] == '1')	//벽
					{
						std::cout << "■";
					}
					else if (map[i][j] == 'e')	//입구
					{
						std::cout << "o ";
					}
					else if (map[i][j] == 'x')	//출구
					{
						std::cout << "x ";
					}
					else if (map[i][j] == '.')	//지나간 곳
					{
						std::cout << "* ";
					}
				}
			}
			std::cout << std::endl;
		}
	}
	void StackGameTwo()
	{
		printf("벽에 부딪히지 않고 움직이세요(기회 3번)\n");
		ShowGame();
		int count = 0;

		while (stackMaze.isEmpty() == false)
		{
			Node* here = stackMaze.peek();
			int r = here->row;
			int c = here->col;

			//직접 움직이기
			int key;
			key = maze.get_Enterkey();

			if (count < 3) {
				switch (key)
				{
				case 72:	//상
					if (r - 1 >= 0 && c >= 0 && r - 1 < height && c < width && map[r - 1][c] == 'x') {
						map[r][c] = '0';
						map[r - 1][c] = '.';
						stackMaze.pop();
						system("cls");
						printf("\n!탐색 성공!\n");
						ShowGame();
						return;
					}
					else if (r - 1 >= 0 && c >= 0 && r - 1 < height && c < width && map[r - 1][c] == '0')	//상단 이동 가능 경우
					{
						system("cls");
						map[r][c] = '0';
						map[r - 1][c] = '.';
						stackMaze.pop();
						stackMaze.push(new Node(r - 1, c));
						printf("실패: %d번 \n", count);
						ShowGame();
					}
					else {
						printf("Can't Move\n");
						count++;
					}
					break;
				case 80:	//하
					if (r + 1 >= 0 && c >= 0 && r + 1 < height && c < width && map[r + 1][c] == 'x') {
						map[r][c] = '0';
						map[r + 1][c] = '.';
						stackMaze.pop();
						system("cls");
						printf("\n!탐색 성공!\n");
						ShowGame();
						return;
					}
					else if (r + 1 >= 0 && c >= 0 && r + 1 < height && c < width && map[r + 1][c] == '0')	//하단 통로
					{
						system("cls");
						map[r][c] = '0';
						map[r + 1][c] = '.';
						stackMaze.pop();
						stackMaze.push(new Node(r + 1, c));
						printf("실패: %d번 \n", count);
						ShowGame();
					}
					else {
						printf("Can't Move\n");
						count++;
					}
					break;
				case 75:	//좌
					if (r >= 0 && c - 1 >= 0 && r < height && c - 1 < width && map[r][c - 1] == 'x') {
						map[r][c] = '0';
						map[r][c - 1] = '.';
						stackMaze.pop();
						system("cls");
						printf("\n!탐색 성공!\n");
						ShowGame();
						return;
					}
					else if (r >= 0 && c - 1 >= 0 && r < height && c - 1 < width && map[r][c - 1] == '0')	//좌측 통로
					{
						system("cls");
						map[r][c] = '0';
						map[r][c - 1] = '.';
						stackMaze.pop();
						stackMaze.push(new Node(r, c - 1));
						printf("실패: %d번 \n", count);
						ShowGame();
					}
					else {
						printf("Can't Move\n");
						count++;
					}
					break;
				case 77:	//우
					if (r >= 0 && c + 1 >= 0 && r < height && c + 1 < width && map[r][c + 1] == 'x') {
						map[r][c] = '0';
						map[r][c + 1] = '.';
						stackMaze.pop();
						system("cls");
						printf("\n!탐색 성공!\n");
						ShowGame();
						return;
					}
					else if (r >= 0 && c + 1 >= 0 && r < height && c + 1 < width && map[r][c + 1] == '0')	//우측 통로
					{
						system("cls");
						map[r][c] = '0';
						map[r][c + 1] = '.';
						stackMaze.pop();
						stackMaze.push(new Node(r, c + 1));
						printf("실패: %d번 \n", count);
						ShowGame();
					}
					else {
						printf("Can't Move\n");
						count++;
					}
					break;
				}
			}
			else
			{
				system("cls");
				printf("실패!!\n");
				ShowGame();
				return;
			}
		}
		return;
	}
};
