#ifndef B_TREE_H
#define B_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int min_degree;
int size;
//#define min_degree = 2  //const

/* structures */

struct Node {
	bool leaf;
	int key_len;
	int child_len;
	int* key_arr;
	struct Node** child_arr;

	////use this when use const array
	//int key_arr[2 * min_degree -1];
	//struct Node* child_arr[2 * min_degree -1];
};

struct B_tree {
	struct Node* root;
};

struct Searched {
	struct Node* node;
	int index;
};

/* functions */

// Create
struct B_tree CreateTree();
struct Node* CreateNode();

// Search
void Visual(struct Node* node, int level);
bool Search(struct Node* node, int key);

// Insert
void InsertKey(struct B_tree* tree, int key);
void NonFull(struct Node* node, int key);
void SplitChild(struct Node* node_x, int index);

// Delete
void Delete_key(struct B_tree* tree, struct Node* x, int key);
void Borrow_Right(struct Node* x, int idx);
void Borrow_Left(struct Node* x, int idx);
bool Change_Root(struct B_tree* tree, struct Node* x);
void merge(struct Node* x, int idx);
int Succ(struct Node* child_x);
int Pred(struct Node* child_x);

#endif //B_TREE_H