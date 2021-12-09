#define _CRT_SECURE_NO_WARNINGS
#include "Link.h"
#include <iostream>
#include <fstream>
#include <conio.h>
using namespace std;

class Maze
{
	char** map = NULL;
	int width;
	int height;

	//��ġ ť,���� ��ü ����
	LinkedQueue locQueue;
	LinkedStack stack;

public:

	Maze() { init(0, 0); }
	~Maze() { reset(); }

	void init(int w, int h) //map ������ �迭�� �������� �Ҵ�
	{
		map = new char* [h];
		for (int i = 0; i < h; i++)
			map[i] = new char[w];
	}

	void reset() //�̷� �� maze�� �������� ����
	{
		for (int i = 0; i < height; i++)
			delete[]map[i];
		delete[]map;
	}

	bool isValidLoc(int r, int c)
	{
		if (r < 0 || c < 0 || r >= height || c >= width) return false;
		else return map[r][c] == '1' || map[r][c] == '9';
	}

	void roadMap(const char* filename, int num)
	{
		char c;
		ifstream fp(filename);
		if (fp.fail())
			printf("Error: ������ �������� �ʽ��ϴ�.");

		//���࿡ �̷� ũ�� �������ֽø�.. p.213ó��
		//fp >> width >> height; 

		//�̷� ũ�� ���
		FILE* fd = fopen(filename, "r");
		int word = 0;
		height = 1;
		width = 0;
		while ((c = getc(fd)) != EOF) {
			if (c == '\n') height++;
			if (c == '0' || c == '1' || c == '5' || c == '9')
			{
				word++;
			}
		}
		width = word / height;

		//map �������迭 �������� �Ҵ�
		init(width, height);

		//map �������迭�� �̷� ����
		for (int i = 0; i < height; i++)
		{
			for (int z = 0; z < width; z++)
			{
				fp >> c;
				map[i][z] = c;

				//ť�� �Ա� ��ġ ����
				if (map[i][z] == '5')
				{
					Location2D entry(i, z);
					if (num == 1)
						stack.push(new Node(i, z)); //���� �Ա�!
					else if (num == 2)
						locQueue.enqueue(new Node(i, z));
				}
			}
		}
		fp.close();
	}

	//�̷� ȭ�鿡 ���
	void printMap() {
		//printf("��ü �̷��� ũ�� = %d X %d \n", width, height);
		printf("\n\n  [ Map ]\n\n");
		for (int i = 0; i < height; i++)
		{
			for (int z = 0; z < width; z++)
			{
				if (map[i][z] == '1') //��
				{
					cout << "  ";
				}
				else if (map[i][z] == '0') //��
				{
					cout << "��";
				}
				else if (map[i][z] == '5') //�Ա�
				{
					cout << "o ";
				}
				else if (map[i][z] == '9') //�ⱸ
				{
					cout << "x ";
				}
				else if (map[i][z] == '.') //������ ��
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
		return 0;
	}

	void queue_SearchExit()
	{
		while (locQueue.isEmpty() == false) //ť�� ������� �ʴ� ����
		{
			if (get_Enterkey() == 13)
			{
				system("cls");

				Location2D* here = locQueue.peek(); //ť�� ��� front ��ü ����
				locQueue.dequeue(); //ť ��� ��ü ����

				int r = here->row;
				int c = here->col;

				if (map[r][c] == '9') { //�ⱸ�̸� ����
					printf("\n!Ž�� ����!\n");
					printf("=====[ Result ]=====\n");
					printMap();

					return;
				}
				else
				{
					map[r][c] = '.';
					if (isValidLoc(r - 1, c)) locQueue.enqueue(new Node(r - 1, c));
					if (isValidLoc(r + 1, c)) locQueue.enqueue(new Node(r + 1, c));
					if (isValidLoc(r, c - 1)) locQueue.enqueue(new Node(r, c - 1));
					if (isValidLoc(r, c + 1)) locQueue.enqueue(new Node(r, c + 1));
					printf("Now Position: (%d,%d) \n", r, c);
					printMap();

				}
			}
		}
		printf("�̷� Ž�� ����\n");
	}

	void stack_Search()	//���� �κ�
	{
		while (stack.isEmpty() == false) {	//���� ������� ���� ���
			printMap();
			if (get_Enterkey() == 13) {
				system("cls");
				Node* here = stack.peek();
				stack.pop();

				int r = here->row;
				int c = here->col;
				printf("Now Position: (%d,%d) ", r, c);

				if (map[r][c] == '9')
				{
					system("cls");
					printf("\n!Ž�� ����!\n");
					printf("=====[ Result ]=====\n");
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
		printf("�̷� Ž�� ����\n");
	}
};
