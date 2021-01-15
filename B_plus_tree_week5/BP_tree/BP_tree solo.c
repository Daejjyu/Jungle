//#include<stdio.h>
//#include <stdlib.h>
//#include <stdbool.h>
//
//#define min_degree 2
//
//int* input_arr;
//int* db_arr;
//
////struct
//struct Node {
//	bool leaf;
//	int key_len;
//	struct Node* prev;
//	struct Node* next;
//	int key_arr[2 * min_degree - 1];
//	struct Node* ptr_arr[2 * min_degree - 1];
//};
//struct BP_Tree {
//	struct Node* root;
//	struct Node* head;
//};
//struct Node* CreateNode() {
//	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
//	newNode->leaf = false;
//	newNode->key_len = 0;
//	newNode->next = NULL;
//	newNode->prev = NULL;
//	return newNode;
//}
//struct BP_Tree CreateTree() {
//	struct BP_Tree newTree;
//	newTree.root = CreateNode();
//	newTree.root->leaf = true;
//	newTree.head = newTree.root;
//	return newTree;
//}
//
//void Visual(struct Node* x, int level) {
//	for (int i = 0; i < x->key_len + 1; i++) {
//		if (!x->leaf) {
//			Visual(x->ptr_arr[i], level + 1);
//		}
//		if (i != x->key_len) {
//			for (int j = 0; j < level; j++)
//			{
//				printf("-----!");
//			}
//			printf("%d\n", x->key_arr[i]);
//		}
//	}
//}
//
//void VisualHead(struct Node* x) {
//	struct Node* nextNode = x;
//	if (x->key_len == 0) return;
//	printf("HEAD : ");
//	do {
//		for (int i = 0; i < nextNode->key_len; i++)
//		{
//			printf("%d ", nextNode->key_arr[i]);
//		}
//		nextNode = nextNode->next;
//	} while (nextNode != NULL);
//	printf("\n");
//}
//
//void SplitChild(struct Node* x, int idx) {
//	struct Node* y = x->ptr_arr[idx];
//	struct Node* z = CreateNode();
//	z->leaf = y->leaf;
//	//link
//	if (y->leaf) {
//		if (y->next != NULL) {
//			z->next = y->next;
//			y->next->prev = z;
//		}
//		y->next = z;
//		z->prev = y;
//	}
//
//	if (y->leaf) {
//		//key move y to z (t-1개)
//		for (int i = 0; i < min_degree - 1; i++)
//		{
//			z->key_arr[i] = y->key_arr[min_degree + i];
//		}
//		//ptr move y to z (t-1개)
//		for (int i = 0; i < min_degree - 1; i++)
//		{
//			z->ptr_arr[i] = y->ptr_arr[min_degree + i];
//		}
//		//move x
//		for (int i = x->key_len - 1; i >= idx; i--)
//		{
//			x->key_arr[i + 1] = x->key_arr[i];
//		}
//		//move y to x (1개)
//		x->key_arr[idx] = y->key_arr[min_degree - 1];
//		x->key_len++;
//		//y key ptr len t개
//		y->key_len = min_degree;
//
//		//z key len 은 t-1개
//		//z ptr len 은 t개
//		z->key_len = min_degree - 1;
//	}
//	else {
//		//move y to z (t-1개)
//		for (int i = 0; i < min_degree - 1; i++)
//		{
//			z->key_arr[i] = y->key_arr[min_degree + i]; // index t ~ 2t-2
//		}
//		//ptr move y to z (t개)
//		for (int i = 0; i <= min_degree - 1; i++)
//		{
//			z->ptr_arr[i] = y->ptr_arr[min_degree + i];
//		}
//
//		for (int i = x->key_len - 1; i >= idx; i--)
//		{
//			x->key_arr[i + 1] = x->key_arr[i];
//		}
//		//move y to x (1개)
//		x->key_arr[idx] = y->key_arr[min_degree - 1];
//		x->key_len++;
//
//		//y, z key len은 t-1개
//		y->key_len = min_degree - 1;
//		z->key_len = min_degree - 1;
//	}
//	for (int i = x->key_len - 1; i > idx; i--)
//	{
//		x->ptr_arr[i + 1] = x->ptr_arr[i];
//	}
//	x->ptr_arr[idx + 1] = z;
//}
//
//void NonFull(struct Node* x, int key, int data) {
//	int ptr_idx = 0;
//	while (ptr_idx < x->key_len && key > x->key_arr[ptr_idx])
//	{
//		ptr_idx++;
//	}
//
//	if (x->leaf) {
//		//중복되는 값이 없어 ptr_idx는 key가 들어갈 위치다.
//		//key 넣을 공간 만들기
//		x->key_len++;
//		for (int i = x->key_len - 2; i >= ptr_idx; i--)
//		{
//			x->key_arr[i + 1] = x->key_arr[i];
//		}
//
//		//ptr move
//		for (int i = x->key_len - 1; i >= ptr_idx; i--)
//		{
//			x->ptr_arr[i + 1] = x->ptr_arr[i];
//		}
//
//		x->key_arr[ptr_idx] = key;
//		x->ptr_arr[ptr_idx - 1] = db_arr;
//		x->ptr_arr[ptr_idx] = db_arr;
//		x->ptr_arr[ptr_idx + 1] = db_arr;
//	}
//	else {
//		if (x->ptr_arr[ptr_idx]->key_len >= 2 * min_degree - 1) {
//			SplitChild(x, ptr_idx);
//			NonFull(x, key, data);
//			return;
//		}
//
//		NonFull(x->ptr_arr[ptr_idx], key, data);
//	}
//}
//
////Insert
//void Insert(struct BP_Tree* tree, int key, int data) {
//	int ptr_idx = 0;
//	while (ptr_idx < tree->root->key_len && key > tree->root->key_arr[ptr_idx])
//	{
//		ptr_idx++;
//	}
//
//	//root is full
//	if (tree->root->key_len >= 2 * min_degree - 1) {
//		struct Node* newRoot = CreateNode();
//		newRoot->ptr_arr[0] = tree->root;
//		tree->root = newRoot;
//		SplitChild(tree->root, 0);
//		NonFull(tree->root, key, data);
//	}
//	else {
//		if (!tree->root->leaf && tree->root->ptr_arr[ptr_idx]->key_len >= 2 * min_degree - 1) {
//			SplitChild(tree->root, ptr_idx);
//		}
//		NonFull(tree->root, key, data);
//	}
//}
//
//bool Traverse(struct Node* x, int key) {
//	bool ret = 0;
//	int ptr_idx = 0;
//	while (ptr_idx < x->key_len && key > x->key_arr[ptr_idx])
//	{
//		ptr_idx++;
//	}
//	if (!x->leaf) {
//		ret = Traverse(x->ptr_arr[ptr_idx], key);
//	}
//	else {
//		for (int i = 0; i <= ptr_idx; i++)
//		{
//			if (x->key_arr[i] == key) {
//				return ret = 1;
//				printf("key %d, DB %d", x->key_arr[i], x->ptr_arr[i]);
//			}
//		}
//	}
//	return ret;
//}
//
//void BorrowRight(struct Node* x, int idx) {
//	struct Node* y = x->ptr_arr[idx];
//	struct Node* z = x->ptr_arr[idx + 1];
//
//	if (y->leaf) {
//		//move z to y
//		y->key_arr[y->key_len] = z->key_arr[0];
//
//		x->key_arr[idx] = z->key_arr[0];
//	}
//	else {
//		//move x to y
//		y->key_arr[y->key_len] = x->key_arr[idx];
//
//		//move z to x
//		x->key_arr[idx] = z->key_arr[0];
//	}
//
//	y->key_len++;
//	y->ptr_arr[y->key_len] = z->ptr_arr[0];
//
//	//move z
//	for (int i = 0; i < z->key_len - 1; i++)
//	{
//		z->key_arr[i] = z->key_arr[i + 1];
//	}
//	for (int i = 0; i < z->key_len + 1; i++)
//	{
//		z->ptr_arr[i] = z->ptr_arr[i + 1];
//	}
//	z->key_len--;
//}
//
//void BorrowLeft(struct Node* x, int idx) {
//	struct Node* y = x->ptr_arr[idx - 1];
//	struct Node* z = x->ptr_arr[idx];
//
//	//move z
//	for (int i = z->key_len - 1; i >= 0; i--)
//	{
//		z->key_arr[i + 1] = z->key_arr[i];
//	}
//	for (int i = z->key_len + 1; i >= 0; i--)
//	{
//		z->ptr_arr[i + 1] = z->ptr_arr[i];
//	}
//	z->key_len++;
//	z->ptr_arr[0] = y->ptr_arr[y->key_len];
//
//	if (y->leaf) {
//		//move y to z
//		z->key_arr[0] = y->key_arr[y->key_len - 1];
//
//		//move y-1 to x
//		x->key_arr[idx - 1] = y->key_arr[y->key_len - 2];
//	}
//	else {
//		//move x to z
//		z->key_arr[0] = x->key_arr[idx - 1];
//
//		//move y to x
//		x->key_arr[idx - 1] = y->key_arr[y->key_len - 1];
//	}
//
//	//move y
//	y->key_len--;
//}
//
//void merge(struct BP_Tree* tree, struct Node* x, int idx) {
//	struct Node* y = x->ptr_arr[idx];
//	struct Node* z = x->ptr_arr[idx + 1];
//	if (y->leaf) {
//		y->next = NULL;//debug
//		if (z->next != NULL) {
//			y->next = z->next;
//			z->next->prev = y;
//		}
//		//move z to y
//		for (int i = 0; i < z->key_len; i++)
//		{
//			y->key_arr[y->key_len + i] = z->key_arr[i];
//		}
//
//		for (int i = 0; i < z->key_len + 1; i++)
//		{
//			y->ptr_arr[y->key_len + i] = z->ptr_arr[i];//debug
//		}
//		y->key_len += z->key_len;//debug
//		//move x
//		for (int i = idx; i < x->key_len - 1; i++)
//		{
//			x->key_arr[i] = x->key_arr[i + 1];
//		}
//		for (int i = idx + 1; i < x->key_len + 1; i++)
//		{
//			x->ptr_arr[i] = x->ptr_arr[i + 1];
//		}
//		x->key_len--;
//	}
//	else {
//		//move x,z to y
//		y->key_arr[y->key_len] = x->key_arr[idx];
//		y->key_len++;
//		for (int i = 0; i < z->key_len; i++)
//		{
//			y->key_arr[y->key_len + i] = z->key_arr[i];
//		}
//		for (int i = 0; i < z->key_len + 2; i++)
//		{
//			y->ptr_arr[y->key_len + i] = z->ptr_arr[i];//debug
//		}
//		y->key_len += z->key_len;//debug
//
//		//move x
//		for (int i = idx; i < x->key_len; i++)
//		{
//			x->key_arr[i] = x->key_arr[i + 1];
//		}
//		for (int i = idx + 1; i < x->key_len + 2; i++)
//		{
//			x->ptr_arr[i] = x->ptr_arr[i + 1];
//		}
//		x->key_len--;
//	}
//	//free(z);
//}
//
//void Delete(struct BP_Tree* tree, struct Node* x, int key) {
//	int ptr_idx = 0;
//	while (ptr_idx < x->key_len && key > x->key_arr[ptr_idx])
//	{
//		ptr_idx++;
//	}
//
//	//case 1 cur==leaf
//	if (x->leaf) {
//		//x key
//		for (int i = ptr_idx; i <= x->key_len; i++)
//		{
//			x->key_arr[i] = x->key_arr[i + 1];
//		}
//
//		//x ptr
//		for (int i = ptr_idx; i <= x->key_len + 1; i++)
//		{
//			x->ptr_arr[i] = x->ptr_arr[i + 1];
//		}
//		x->key_len--;
//		return;
//	}
//	else {
//		//case 3-0
//		if (x->ptr_arr[ptr_idx]->key_len > min_degree - 1) {
//			Delete(tree, x->ptr_arr[ptr_idx], key);
//			return;
//		}
//		else {
//			//case 3-1-1 borrow right
//			if (ptr_idx != x->key_len) {
//				if (x->ptr_arr[ptr_idx + 1]->key_len > min_degree - 1) {
//					BorrowRight(x, ptr_idx);
//					Delete(tree, x, key);
//					return;
//				}
//				//case 3-2 merge
//				merge(tree, x, ptr_idx);
//
//				//root change
//				if (tree->root->key_len == 0) {
//					tree->root = x->ptr_arr[ptr_idx];
//					//free(x);
//					Delete(tree, x, key);
//				}
//				else {
//					Delete(tree, x, key);
//					return;
//				}
//			}
//			else {
//				//case 3-1-2 borrow left
//				if (x->ptr_arr[ptr_idx - 1]->key_len > min_degree - 1) {
//					BorrowLeft(x, ptr_idx);
//					Delete(tree, x, key);
//					return;
//				}
//				//case 3-2 merge
//				merge(tree, x, ptr_idx - 1);
//
//				//root change
//				if (tree->root->key_len == 0) {
//					tree->root = x->ptr_arr[ptr_idx - 1];
//					//free(x);
//					Delete(tree, x, key);
//				}
//				else {
//					Delete(tree, x, key);
//					return;
//				}
//			}
//		}
//	}
//}
//
//#define TEST { 10,1,3,63,82,6,31,8,2,16,11,77,96,14,92,21,47,23,24,78,26,97,15,4,30,69,37,38,76,90,17,87,53,44,45,46,9,41,54,43,22,84,58,39,65,28,42,59,99,70,71,72,29,74,73,68,13,60,79,80,81,85,83,64,94,86,66,88,93,40,91,62,25,20,36,95,19,52,55,100 }
//
//// 출력(DFS)
//void print_for_exam(struct Node* x) {
//	for (int i = 0; i < x->key_len + 1; i++) {
//		int cnt = 0;
//		if (!x->leaf) {
//			print_for_exam(x->ptr_arr[i]);
//			if (i != x->key_len) printf("[%5d]\n", x->key_arr[i]);
//		}
//		else {
//			if (i != x->key_len) {
//				int* db = x->ptr_arr[i];
//				printf("[%5d, %5d]\n", x->key_arr[i], *(db + x->key_arr[i]));
//			}
//		}
//	}
//}
//
//// 테스트
//int main()
//{
//	//int size;
//	//printf("* Enter the size of B+ Tree:  ");
//	//scanf_s("%d", &size);
//	//// Random TEST
//	//int* random_arr = (int*)malloc(sizeof(int) * size);
//	//struct BP_Tree bp_tree = CreateTree();
//
//	//for (int i = 0; i < size; i++) {
//	//	random_arr[i] = i;
//	//}
//	//for (int i = 0; i < size; i++)
//	//{
//	//	int j = i + rand() / (RAND_MAX / (size - i) + 1);
//	//	int t = random_arr[j];
//	//	random_arr[j] = random_arr[i];
//	//	random_arr[i] = t;
//	//}
//	//for (int i = 0; i < size; i++) {
//	//	int r = random_arr[i];
//	//	printf("%d ", r);
//	//	Insert(&bp_tree, r, r * 1000);
//	//}
//
//	//printf("---- TREE0 ----\n");
//	//Visual(bp_tree.root, 0);
//	//printf("\n\n\n");
//
//	//TEST
//	int arr[80] = TEST;
//	db_arr = (int*)malloc(sizeof(int) * 300);
//	for (int i = 0; i < 300; i++)
//	{
//		db_arr[i] = (i) * 1000;
//	}
//	struct Node* flag = CreateNode();
//	struct BP_Tree BTree = CreateTree();
//	flag->next = BTree.root;
//
//	// TEST 1 CASE
//	Insert(&BTree, 4, 4 * 1000);
//	Insert(&BTree, 1, 1 * 1000);
//	Insert(&BTree, 3, 3 * 1000);
//	Insert(&BTree, 2, 2 * 1000);
//
//	Delete(&BTree, BTree.root, 4);
//	Delete(&BTree, BTree.root, 2);
//	Delete(&BTree, BTree.root, 3);
//
//	printf("---- TREE1 ----\n");
//	Visual(BTree.root, 0);
//	printf("\n---- TEST1 ----\n");
//	print_for_exam(BTree.root);
//
//	// TEST 2 CASE
//	for (int i = 2; i <= 100; i++) {
//		Insert(&BTree, i, i * 1000);
//	}
//	for (int i = 50; i <= 70; i++) {
//		Delete(&BTree, BTree.root, i);
//	}
//
//	printf("\n\n\n\n\n\n");
//	printf("---- TREE2 ----\n");
//	Visual(BTree.root, 0);
//	printf("\n---- TEST2 ----\n");
//	print_for_exam(BTree.root);
//
//	// TEST3 CASE
//	for (int i = 50; i <= 70; i++) {
//		Insert(&BTree, i, i * 1000);
//	}
//
//	for (int i = 0; i < 80; i++) {
//		Delete(&BTree, BTree.root, arr[i]);
//	}
//
//	printf("\n\n\n\n\n\n");
//	printf("---- TREE3 ----\n");
//	Visual(BTree.root, 0);
//	printf("\n---- TEST3 ----\n");
//	print_for_exam(BTree.root);
//
//	printf("프로그램이 정상적으로 종료 되었음.");
//	return 0;
//}