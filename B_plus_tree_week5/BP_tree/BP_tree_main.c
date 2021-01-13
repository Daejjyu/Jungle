#include "BP_tree.h"

int main() {
	// 1. Input Min_degree
	printf("***************************\n");
	printf("* Enter the min_degree:  ");
	scanf_s("%d", &min_degree);
	printf("* Enter the size of B- Tree:  ");
	scanf_s("%d", &size);

	// 2. Create random_tree
	int* random_arr = (int*)malloc(sizeof(int) * size * 2);
	struct B_tree B_Tree = CreateTree();
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
		InsertKey(&B_Tree, r);
	}

	int mode = 0;

	// UI
	while (1) {
		printf("\n\n*** Current Tree ***\n");
		Visual(B_Tree.root, 0);
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
			InsertKey(&B_Tree, Ins_num);
			printf("\n*****************\n");
			break;
		case 2: // Delete key
			printf("\n*****************\n");
			printf("Delete: Enter int Number:  ");
			int del_num;
			scanf_s("%d", &del_num);
			if (Search(B_Tree.root, del_num)) {
				Delete_key(&B_Tree, B_Tree.root, del_num);
			}
			else {
				printf("\nDelete: Number not found\n");
			}
			printf("\n*****************\n");
			break;
		case 3: // Delete all
			B_Tree = CreateTree();
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