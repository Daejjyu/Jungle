#include "BP_tree.h"
#include<time.h>
int main() {
	// 1. Input Min_degree
	printf("******* B+ Tree **********\n");
	printf("* Enter the min_degree:  ");
	scanf_s("%d", &min_degree);
	printf("* Enter the size of B- Tree:  ");
	scanf_s("%d", &size);

	// 2. Create random_tree
	int* random_arr = (int*)malloc(sizeof(int) * size);
	struct BP_tree BP_tree = CreateTree();
	printf("\n**********Init Tree**********\n");

	for (int i = 0; i < size; i++) {
		random_arr[i] = i;
	}
	for (int i = 0; i < size; i++)
	{
		int j = i + rand() / (RAND_MAX / (size - i) + 1);
		int t = random_arr[j];
		random_arr[j] = random_arr[i];
		random_arr[i] = t;
	}
	for (int i = 0; i < size; i++) {
		int r = random_arr[i];
		printf("%d ", r);
		InsertKey(&BP_tree, r);
	}

	int mode = 0;

	// UI
	while (1) {
		printf("\n\n*** Current Tree ***\n");
		Visual(BP_tree.root, 0);
		printf("*****************\n");
		printf("** Select ** \n");
		printf("1. Insert num\n");
		printf("2. Delete num\n");
		printf("3. Delete all\n");
		printf("4. EXIT\n");
		printf("*****************\n");
		printf("Input: ");
		scanf_s("%d", &mode);

		switch (mode)
		{
		case 1: // Insert
			printf("\n*****************\n");
			printf("Insert: Enter int Number:  ");
			int Ins_num;
			scanf_s("%d", &Ins_num);
			InsertKey(&BP_tree, Ins_num);
			printf("\n*****************\n");
			break;
		case 2: // Delete key
			printf("\n*****************\n");
			printf("Delete: Enter int Number:  ");
			int del_num;
			scanf_s("%d", &del_num);
			if (Search(BP_tree.root, del_num)) {
				Delete_key(&BP_tree, BP_tree.root, del_num);
			}
			else {
				printf("\nDelete: Number not found\n");
			}
			printf("\n*****************\n");
			break;
		case 3: // Delete all
			BP_tree = CreateTree();
			printf("** Delete all");
			break;
		case 4:
			free(random_arr);
			printf("\n\n\nEXIT\n\n\n");
			return 0;
			break;
		}
	}
	return 0;
}