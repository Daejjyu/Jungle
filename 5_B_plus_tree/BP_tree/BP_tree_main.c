#include "BP_tree.h"
#include<time.h>

#define TEST { 10,1,3,63,82,6,31,8,2,16,11,77,96,14,92,21,47,23,24,78,26,97,15,4,30,69,37,38,76,90,17,87,53,44,45,46,9,41,54,43,22,84,58,39,65,28,42,59,99,70,71,72,29,74,73,68,13,60,79,80,81,85,83,64,94,86,66,88,93,40,91,62,25,20,36,95,19,52,55,100 }

// Ãâ·Â(DFS)
void print_for_exam(struct Node* x) {
	for (int i = 0; i < x->key_len + 1; i++) {
		int cnt = 0;
		if (!x->leaf) {
			print_for_exam(x->ptr_arr[i]);
			if (i != x->key_len) printf("[%5d]\n", x->key_arr[i]);
		}
		else {
			if (i != x->key_len) {
				printf("[%5d]\n", x->key_arr[i]);
				/*int* db = x->ptr_arr[i];
				printf("[%5d, %5d]\n", x->key_arr[i], *(db + x->key_arr[i]));*/
			}
		}
	}
}

int main() {
	// 1. Input Min_degree
	printf("******* B+ Tree **********\n");
	printf("* Enter the min_degree:  ");
	scanf_s("%d", &min_degree);
	printf("* Enter the size of B+ Tree:  ");
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
		print_for_exam(BP_tree.root);
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