#include "Maze.h"
void main()
{
	int num;
	Maze maze;

	while (1) {
		printf("=================================\n");
		printf("1. ���Ḯ��Ʈ �������� �̷�ã��\n");
		printf("2. ���Ḯ��Ʈ ť�� �̷�ã��\n");
		printf("=================================\PrintMazeMenu");
		printf("��ȣ �Է�, ��ȣ �� �� ���� : ");
		scanf("%d", &num);

		if (num == 1)
		{
			maze.roadMap("Maze.txt", num);
			system("cls");
			printf("<Linked Stack Maze> Start!!\n-> Enter and Show Miro Program");
			maze.stack_Search();
		}
		else if(num==2)
		{
			maze.roadMap("Maze.txt", num);
			system("cls");
			printf("<LinkedQueue Maze> Start!!\n-> Enter and Show Miro Program");
			maze.printMap();
			maze.queue_SearchExit();
		}
		else
		{
			printf("���α׷��� �����մϴ�.\n");
			break;
		}
	}
	
}