#include "B_tree.h"

//Visualization Tree
void Visual(struct Node* node, int level) {
	for (int i = 0; i <= node->key_len; i++) {
		if (node->child_len > 0 && !node->leaf) {
			Visual(node->child_arr[i], level + 1);
		}
		if (i <= node->key_len - 1) {
			for (int j = 0; j < level; j++) {
				printf("--|");
			}
			printf("%d\n", node->key_arr[i]);
			//printf("%d  ---- child_len: %d\n", node->key_arr[i], node->child_len);
		}
	}
}

void free_node(struct Node* node) {
	free(node->child_arr);
	free(node->key_arr);
	free(node);
	printf("Free Complete\n");
}

struct Node* CreateNode() {
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->leaf = false;
	newNode->key_len = 0;
	newNode->child_len = 0;
	newNode->key_arr = (int*)malloc(sizeof(int) * (2 * min_degree - 1));
	newNode->child_arr = (struct Node**)malloc(sizeof(struct Node*) * (2 * min_degree));

	return newNode;
};

struct B_tree CreateTree() {
	struct B_tree tree;
	tree.root = CreateNode(min_degree);
	tree.root->leaf = true;
	return tree;
}

void InsertKey(struct B_tree* tree, int key) {
	struct Node* origin_root = tree->root;

	// if origin_root is Full
	if (origin_root->key_len == (2 * min_degree - 1)) {
		struct Node* newNode = CreateNode(min_degree);
		tree->root = newNode;
		tree->root->leaf = false;
		tree->root->child_arr[tree->root->child_len] = origin_root;
		tree->root->child_len++;

		//Split first, NonFull later
		SplitChild(tree->root, 0);
		NonFull(tree->root, key);
	}
	else {
		NonFull(origin_root, key);
	}
}

//only insert at leaf node
void NonFull(struct Node* node, int key) {
	//last index of node
	int i = node->key_len - 1;

	//if node == leaf
	if (node->leaf) {
		// find position and key index move
		while (i >= 0 && key < node->key_arr[i]) {
			node->key_arr[i + 1] = node->key_arr[i];
			i--;
		}
		i++;
		node->key_arr[i] = key;
		node->key_len++;
	}
	else {
		// find position
		while (i >= 0 && key < node->key_arr[i]) {
			i--;
		}
		i++;
		// if child arr is full
		if (node->child_arr[i]->key_len == (2 * min_degree - 1)) {
			SplitChild(node, i);
			NonFull(node, key);
		}
		else {
			NonFull(node->child_arr[i], key);
		}
	}
}

void SplitChild(struct Node* node_x, int index) {
	struct Node* node_y = node_x->child_arr[index];
	struct Node* node_z = CreateNode();

	// 1. Arragne z node's env & Move y node's key & child to z node
	node_z->leaf = node_y->leaf; // if y is leaf, than z is leaf
	node_z->key_len = min_degree - 1; // z key is always (t-1, min_key num)

	//move y node's keys to z
	for (int j = 0; j < min_degree - 1; j++) {
		node_z->key_arr[j] = node_y->key_arr[j + min_degree];
	}
	node_y->key_len = min_degree - 1;

	//if not leaf, move child
	if (!node_y->leaf) {
		node_z->child_len = min_degree;
		node_y->child_len = min_degree;
		for (int j = 0; j < min_degree; j++) {
			node_z->child_arr[j] = node_y->child_arr[j + min_degree];
		}
	}

	// 2. Rearrange x node's env
	// x key
	for (int j = node_x->key_len - 1; j >= index; j--)
	{
		node_x->key_arr[j + 1] = node_x->key_arr[j];
	}
	node_x->key_arr[index] = node_y->key_arr[min_degree - 1];
	node_x->key_len++;

	// x child
	for (int j = node_x->child_len - 1; j > index; j--)
	{
		node_x->child_arr[j + 1] = node_x->child_arr[j];
	}
	node_x->child_arr[index + 1] = node_z;
	node_x->child_len++;
}

bool Search(struct Node* node, int key) {
	int i = 0;
	bool result = false;
	//find next position
	for (; i < node->key_len; ++i) {
		printf("%d ", i);
		//if find idx
		if (key == node->key_arr[i]) {
			printf("Search Success : %d\n", key);
			return true;
		}
		if (key < node->key_arr[i]) {
			break;
		}
	}
	//if next node exist go
	if (node->child_len > 0) {
		result = Search(node->child_arr[i], key);
	}
	else {
		printf("Search Failed %d\n", key);
	}
	return result;
}

// Find x node's predecessor key
int Pred(struct Node* child_x) {
	int ret = -1;
	if (!child_x->leaf) {
		ret = Pred(child_x->child_arr[child_x->child_len - 1]);
	}
	else {
		return ret = child_x->key_arr[child_x->key_len - 1];
	}
	return ret;
}

// Find x node's successor key
int Succ(struct Node* child_x) {
	int ret = -1;
	if (!child_x->leaf) {
		ret = Succ(child_x->child_arr[0]);
	}
	else {
		return ret = child_x->key_arr[0];
	}
	return ret;
}

void merge(struct Node* x, int idx) {
	struct Node* y = x->child_arr[idx];
	struct Node* z = x->child_arr[idx + 1];

	// 1. Move all z node's keys and children to y node.
	y->key_arr[y->key_len] = x->key_arr[idx];
	y->key_len++;
	for (int i = 0; i < z->key_len; i++) {
		y->key_arr[y->key_len + i] = z->key_arr[i];
	}
	for (int i = 0; i < z->child_len; i++) {
		y->child_arr[y->child_len + i] = z->child_arr[i];
	}

	if (!y->leaf)y->child_len = (2 * min_degree);
	y->key_len = (2 * min_degree) - 1;

	// 2. Add count number of z node's keys and children to y node.
	for (int i = idx; i < x->key_len; i++) {
		x->key_arr[i] = x->key_arr[i + 1];
	}
	x->key_len--;

	for (int i = idx + 1; i < x->child_len - 1; i++) {
		x->child_arr[i] = x->child_arr[i + 1];
	}
	x->child_len--;

	// 3. Free z node
	free_node(z);
}

bool Change_Root(struct B_tree* tree, struct Node* x) {
	bool result = false;
	if (x->key_len == 0) {
		printf("Root Changed\n");
		tree->root = x->child_arr[0];

		free_node(x);
		result = true;
		return result;
	}
	printf("Root_not Changed\n");
	return result;
}

void Borrow_Left(struct Node* x, int idx) {
	struct Node* y = x->child_arr[idx - 1];
	struct Node* z = x->child_arr[idx];

	// 1. Make z node's extra space(Z[0]) for X[idx]
	for (int i = 0; i < z->key_len; i++) {
		z->key_arr[z->key_len - i] = z->key_arr[z->key_len - i - 1];
	}

	// 2. Rotate keys (y[-1] - > x[idx] -> z[0] )
	z->key_len++;
	z->key_arr[0] = x->key_arr[idx - 1];
	x->key_arr[idx - 1] = y->key_arr[y->key_len - 1];
	y->key_len--;

	// 3. if not leaf than move child
	if (!y->leaf) {
		for (int i = 0; i < z->child_len; i++) {
			z->child_arr[z->child_len - i] = z->child_arr[z->child_len - i - 1];
		}
		z->child_len++;
		z->child_arr[0] = y->child_arr[y->child_len - 1];
		y->child_len--;
	}
}

void Borrow_Right(struct Node* x, int idx) {
	struct Node* y = x->child_arr[idx];
	struct Node* z = x->child_arr[idx + 1];

	// 1. Make y node's extra space(z[-1]) for x[idx]
	y->key_len++;

	// 2. Rotate keys (y[-1] < x[idx] < z[0] )
	y->key_arr[y->key_len - 1] = x->key_arr[idx];
	x->key_arr[idx] = z->key_arr[0];
	for (int i = 0; i < z->key_len - 1; i++) {
		z->key_arr[i] = z->key_arr[i + 1];
	}
	z->key_len--;

	// 3. if not leaf than move child
	if (!y->leaf) {
		y->child_arr[y->child_len] = z->child_arr[0];
		y->child_len++;
		for (int i = 0; i < z->child_len - 1; i++) {
			z->child_arr[i] = z->child_arr[i + 1];
		}
		z->child_len--;
	}
}

int step, debug_num = 0;
void print_debug(struct B_tree* tree) {
	//printf("\n step: %d , debug_num: %d\n", step, debug_num++);
	Visual(tree->root, 0);
	printf("\n");
}

void Delete_key(struct B_tree* tree, struct Node* x, int key) {
	////debug
	//debug_num = 0;
	//step++;
	//print_debug(tree);

	int idx = 0; // idx of child

	// find index at node x
	for (; idx < x->key_len; idx++) {
		if (key <= x->key_arr[idx]) {
			break;
		}
	}
	int key_idx = idx;
	if (key_idx == x->key_len) {
		key_idx--;
	}

	if (key == x->key_arr[key_idx]) {
		// case 1, find key and node is leaf at node x
		if (x->leaf) {
			//printf("case 1 \n");//debug
			for (int i = idx; i < x->key_len - 1; i++) {
				x->key_arr[i] = x->key_arr[i + 1];
			}
			x->key_len--;
			return;
		}
		// case 2, find key and node is not leaf at node x
		else {
			// case 2-A, if (pre-child len > min_degree -1), go
			if (x->child_arr[idx]->key_len > min_degree - 1) {
				//printf("case 2-A \n");//debug
				int k_prime = Pred(x->child_arr[idx]);
				Delete_key(tree, x->child_arr[idx], k_prime);
				x->key_arr[idx] = k_prime;
				return;
			}
			// case 2-B, if (succ-child len > min_degree -1), go
			else if (x->child_arr[idx + 1]->key_len > min_degree - 1) {
				//printf("case 2-B \n");// debug
				int k_prime = Succ(x->child_arr[idx + 1]);
				Delete_key(tree, x->child_arr[idx + 1], k_prime);
				x->key_arr[idx] = k_prime;
				return;
			}
			else {
				// case 2-C, if (pre and succ child len) < t, merge
				//printf("case 2-C \n");// debug
				merge(x, idx);
				if (Change_Root(tree, x)) {
					Delete_key(tree, tree->root, key);
					return;
				}
				else {
					Delete_key(tree, x, key);
					return;
				}
			}
		}
	}
	// case 3, key not found at node x
	else {
		// case 3-0, if (child[idx] len > min_degree -1), go
		if (x->child_arr[idx]->key_len > min_degree - 1) {
			//printf("case 3-0 \n");// debug
			Delete_key(tree, x->child_arr[idx], key);
			return;
		}
		else {
			// case 3-A-1, if (left exist) and (child[left] len > min_degree -1), borrow
			if (idx != 0 && x->child_arr[idx - 1]->key_len > min_degree - 1) {
				//printf("case 3-A-1 \n");// debug

				Borrow_Left(x, idx);
				Delete_key(tree, x, key);
				return;
			}
			// case 3-A-2, if (right exist) and (child[right] len > min_degree -1), borrow
			else if (idx != x->child_len - 1 && x->child_arr[idx + 1]->key_len > min_degree - 1) {
				//printf("case 3-A-2 \n");// debug

				Borrow_Right(x, idx);
				Delete_key(tree, x, key);
				return;
			}
			// case 3-B, if if (left and right child len) < t, merge
			else {
				//printf("case 3-B \n");// debug
				struct Node* y = x->child_arr[key_idx];
				struct Node* z = x->child_arr[key_idx + 1];

				// Merge all x, y, z node's keys and children into y node
				// merge x to y
				y->key_arr[y->key_len] = x->key_arr[key_idx];
				y->key_len++;
				// merge z to y
				for (int i = 0; i < z->key_len; i++) {
					y->key_arr[y->key_len] = z->key_arr[i];
					y->key_len++;
				}
				// if y is not leaf node than child move
				if (!y->leaf) {
					for (int i = 0; i < z->child_len; i++) {
						y->child_arr[y->child_len] = z->child_arr[i];
						y->child_len++;
					}
				}
				// Free z node
				free_node(z);

				// x key move
				for (int i = key_idx; i < x->key_len - 1; i++) {
					x->key_arr[i] = x->key_arr[i + 1];
				}
				x->key_len--;

				// x child move
				for (int i = key_idx + 1; i < x->child_len - 1; i++) {
					x->child_arr[i] = x->child_arr[i + 1];
				}
				x->child_len--;

				// check root change
				if (Change_Root(tree, x)) {
					Delete_key(tree, tree->root, key);
					return;
				}
				else {
					Delete_key(tree, x, key);
					return;
				}
			}
		}
	}
}