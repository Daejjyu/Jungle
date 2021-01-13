#ifndef BP_TREE_H
#define BP_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//#define min_degree 2
int min_degree;
int size;
int* database;

struct Node {
	bool leaf;
	int key_len;
	int ptr_len;
	int* key_arr;
	struct node** ptr_arr;
	/*int key_arr[2 * min_degree - 1];
	struct Node* ptr_arr[2 * min_degree];*/
	struct Node* next_node;
	struct Node* prev_node;
};

struct BP_tree {
	struct Node* root;
	struct Node* head;
};

/* functions */

// Create
struct BP_tree CreateTree();
struct Node* CreateNode();

// Search
void Visual(struct Node* node, int level);
void Visual_head(struct BP_tree* tree);
bool Search(struct Node* node, int key);

// Insert
void InsertKey(struct BP_tree* tree, int key);
void NonFull(struct Node* node, int key);
void SplitChild(struct Node* node_x, int index);

// Delete
void Delete_key(struct BP_tree* tree, struct Node* x, int key);
void Borrow_Right(struct Node* x, int idx);
void Borrow_Left(struct Node* x, int idx);
bool Change_Root(struct BP_tree* tree, struct Node* x);
void free_node(struct Node* node);
#endif //BP_TREE_H
