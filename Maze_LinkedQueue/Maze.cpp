#include "Maze.h"
void main()
{
	int num;
	Maze maze;

	while (1) {
		printf("=================================\n");
		printf("1. ���Ḯ��Ʈ �������� �̷�ã��\n");
		printf("2. ���Ḯ��Ʈ ť�� �̷�ã��\n");
		printf("=================================\n");
		printf("��ȣ �Է�, ��ȣ �� �� ���� : ");
		scanf("%d", &num);
		maze.roadMap("Maze.txt", num);
		if (num == 1)
		{
			system("cls");
			printf("<Linked Stack Maze> Start!!\n-> Enter and Show Miro Program");
			maze.stack_Search();
		}
		else if(num==2)
		{
			system("cls");
			printf("<LinkedQueueMaze> Start!!\n-> Enter and Show Miro Program");
			maze.queue_SearchExit();
		}
		else if (num == 3)
		{
			system("cls");
			printf("<LinkedQueueMaze> Start!!\n-> Enter and Show Miro Program");
			maze.printMap();
			maze.QueueMazeGame();
		}
		else
		{
			printf("���α׷��� �����մϴ�.\n");
			break;
		}
	}
	
}