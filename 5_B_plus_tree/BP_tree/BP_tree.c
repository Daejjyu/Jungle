#include "BP_tree.h"

//Visualization Tree
void Visual(struct Node* node, int level) {
	for (int i = 0; i <= node->key_len; i++) {
		if (node->ptr_len > 0 && !node->leaf) {
			Visual(node->ptr_arr[i], level + 1);
		}
		if (i <= node->key_len - 1) {
			for (int j = 0; j < level; j++) {
				printf("--|");
			}
			printf("%d\n", node->key_arr[i]);
		}
	}
}

void Visual_head(struct BP_tree* tree) {
	struct Node* cur_node = tree->head;

	printf("head : ");
	while (cur_node != NULL) {
		for (int i = 0; i < cur_node->key_len; i++)
		{
			printf("%d ", cur_node->key_arr[i]);
			//printf("ptr_len %d\n", cur_node->ptr_len);
		}
		cur_node = cur_node->next_node;
	}
	printf("\n");
}

int step, debug_num = 0;
void print_debug(struct BP_tree tree) {
	printf("\n step: %d , debug_num: %d\n", step, debug_num++);
	Visual(tree.root, 0);
	printf("\n");
}

struct Node* CreateNode() {
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->leaf = false;
	newNode->key_len = 0;
	newNode->ptr_len = 0;
	newNode->prev_node = NULL;
	newNode->next_node = NULL;
	newNode->key_arr = (int*)malloc(sizeof(int) * (2 * min_degree - 1));
	newNode->ptr_arr = (struct Node**)malloc(sizeof(struct Node*) * (2 * min_degree));

	return newNode;
};

struct BP_tree CreateTree() {
	struct BP_tree tree;
	tree.root = CreateNode();
	tree.root->leaf = true;

	tree.head = tree.root;
	return tree;
}

void InsertKey(struct BP_tree* tree, int key) {
	struct Node* origin_root = tree->root;
	//print_debug(*tree);//debug
	//printf("\n");//debug
	//Visual_head(tree);//debug
	//printf("\n");//debug
	// if origin_root is Full
	if (origin_root->key_len == (2 * min_degree - 1)) {
		struct Node* newNode = CreateNode();
		tree->root = newNode;
		tree->root->leaf = false;
		tree->root->ptr_arr[tree->root->ptr_len] = origin_root;
		tree->root->ptr_len++;

		//Split first, NonFull later
		SplitChild(tree->root, 0);
		NonFull(tree->root, key);
	}
	else {
		NonFull(origin_root, key);
	}
}

void SplitChild(struct Node* node_x, int index) {
	struct Node* node_y = node_x->ptr_arr[index];
	struct Node* node_z = CreateNode();

	if (node_y->leaf) {
		// B+ tree
		// 1. Arragne z node's env & Move y node's key & ptr to z node
		node_z->leaf = node_y->leaf; // if y is leaf, than z is leaf
		node_z->key_len = min_degree; // z key is always (t, min_key+1 num)
		// set prev next
		if (node_y->next_node != NULL) {
			node_z->next_node = node_y->next_node;
			node_y->next_node->prev_node = node_z;
		}
		node_y->next_node = node_z;
		node_z->prev_node = node_y;

		//move y node's keys to z
		for (int j = 0; j < min_degree; j++) {
			node_z->key_arr[j] = node_y->key_arr[j + min_degree - 1];
		}
		node_y->key_len = min_degree - 1;

		//move ptr
		node_y->ptr_len = min_degree - 1;
		node_z->ptr_len = min_degree;
		for (int j = 0; j < min_degree + 1; j++) {
			node_z->ptr_arr[j] = node_y->ptr_arr[j + min_degree - 1];
		}
	}
	else {
		// B- tree
		// 1. Arragne z node's env & Move y node's key & ptr to z node
		node_z->leaf = node_y->leaf; // if y is leaf, than z is leaf
		node_z->key_len = min_degree - 1; // z key is always (t-1, min_key num)
		// set prev next
		node_y->next_node = node_z;
		node_z->prev_node = node_y;

		//move y node's keys to z
		for (int j = 0; j < min_degree - 1; j++) {
			node_z->key_arr[j] = node_y->key_arr[j + min_degree];
		}
		node_y->key_len = min_degree - 1;

		//if not leaf, move ptr
		node_y->ptr_len = min_degree;
		node_z->ptr_len = min_degree;
		for (int j = 0; j < min_degree; j++) {
			node_z->ptr_arr[j] = node_y->ptr_arr[j + min_degree];
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

	// x ptr
	for (int j = node_x->ptr_len - 1; j > index; j--)
	{
		node_x->ptr_arr[j + 1] = node_x->ptr_arr[j];
	}
	node_x->ptr_arr[index + 1] = node_z;
	node_x->ptr_len++;
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
		node->ptr_arr[i] = (struct Node*)database;
		node->ptr_len++;
	}
	else {
		// find position
		while (i >= 0 && key < node->key_arr[i]) {
			i--;
		}
		i++;
		// if ptr arr is full
		struct Node* ptr_temp = node->ptr_arr[i];
		if (ptr_temp->key_len == (2 * min_degree - 1)) {
			SplitChild(node, i);
			NonFull(node, key);
		}
		else {
			NonFull(node->ptr_arr[i], key);
		}
	}
}

bool Search(struct Node* node, int key) {
	int idx = 0;
	bool result = false;

	//find next position
	for (; idx < node->key_len; ++idx) {
		//if find idx
		if (key <= node->key_arr[idx]) break;
	}

	if (node->leaf && key == node->key_arr[idx]) {
		//printf("Search Success : %d\n", key);//debug
		return true;
	}

	int key_idx = idx;
	if (idx == node->key_len) key_idx--;

	if (key == node->key_arr[key_idx]) idx++;

	//if cur node is not leaf node
	if (!node->leaf) {
		result = Search(node->ptr_arr[idx], key);
	}
	else {
		//printf("Search Failed %d\n", key);//debug
	}
	return result;
}

//void merge(struct Node* x, int idx) {
//
//bool Change_Root(struct BP_tree* tree, struct Node* x) {
//
void Borrow_Left(struct Node* x, int idx) {
	struct Node* y = x->ptr_arr[idx - 1];
	struct Node* z = x->ptr_arr[idx];

	if (y->leaf) {
		//printf("Borrow_left_1\n");//debug
		// 1. Make z node's extra space(Z[0]) for Y[-1]
		for (int i = 0; i < z->key_len; i++) {
			z->key_arr[z->key_len - i] = z->key_arr[z->key_len - i - 1];
		}
		z->key_len++;

		// 2. Rotate keys (y[-1] - > x[idx] -> z[0] )
		z->key_arr[0] = y->key_arr[y->key_len - 1];
		x->key_arr[idx - 1] = y->key_arr[y->key_len - 1];
		y->key_len--;
	}
	else {
		//printf("Borrow_left_2\n");//debug
		// 1. Make z node's extra space(Z[0]) for X[idx]
		for (int i = 0; i < z->key_len; i++) {
			z->key_arr[z->key_len - i] = z->key_arr[z->key_len - i - 1];
		}

		// 2. Rotate keys (y[-1] - > x[idx] -> z[0] )
		z->key_len++;
		z->key_arr[0] = x->key_arr[idx - 1];
		x->key_arr[idx - 1] = y->key_arr[y->key_len - 1];
		y->key_len--;
	}
	// 3. move ptr
	for (int i = 0; i < z->ptr_len; i++) {
		z->ptr_arr[z->ptr_len - i] = z->ptr_arr[z->ptr_len - i - 1];
	}
	z->ptr_len++;
	z->ptr_arr[0] = y->ptr_arr[y->ptr_len - 1];
	y->ptr_len--;
}

void Borrow_Right(struct Node* x, int idx) {
	struct Node* y = x->ptr_arr[idx];
	struct Node* z = x->ptr_arr[idx + 1];

	if (y->leaf) {
		//printf("Borrow_right_1\n");//debug
		// 1. Make y node's extra space(Y[-1]) for Z[0]
		y->key_len++;

		// 2. Rotate keys (y[-1] < x[idx] < z[0] )
		y->key_arr[y->key_len - 1] = z->key_arr[0];
		x->key_arr[idx] = z->key_arr[1];
		for (int i = 0; i < z->key_len - 1; i++) {
			z->key_arr[i] = z->key_arr[i + 1];
		}
		z->key_len--;
	}
	else {
		//printf("Borrow_right_2\n");//debug
		// 1. Make y node's extra space(z[-1]) for x[idx]
		y->key_len++;

		// 2. Rotate keys (y[-1] < x[idx] < z[0] )
		y->key_arr[y->key_len - 1] = x->key_arr[idx];
		x->key_arr[idx] = z->key_arr[0];
		for (int i = 0; i < z->key_len - 1; i++) {
			z->key_arr[i] = z->key_arr[i + 1];
		}
		z->key_len--;
	}

	// 3. move ptr
	y->ptr_arr[y->ptr_len] = z->ptr_arr[0];
	y->ptr_len++;
	for (int i = 0; i < z->ptr_len - 1; i++) {
		z->ptr_arr[i] = z->ptr_arr[i + 1];
	}
	z->ptr_len--;
}

void free_node(struct Node* node) {
	/*free(node->ptr_arr);
	free(node->key_arr);*/
	free(node);
	//printf("Free Complete\n");//debug
}

bool Change_Root(struct BP_tree* tree, struct Node* x) {
	bool result = false;
	if (x->key_len == 0) {
		//printf("Root Changed\n");//debug
		tree->root = x->ptr_arr[0];

		free_node(x);
		result = true;
		return result;
	}
	//printf("Root_not Changed\n");//dubug
	return result;
}

void Delete_key(struct BP_tree* tree, struct Node* x, int key) {
	//////debug
	//debug_num = 0;
	//step++;
	//print_debug(*tree);

	int idx = 0; // idx of ptr

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
	if (!x->leaf && key == x->key_arr[key_idx]) idx++;

	if (x->leaf && key == x->key_arr[key_idx]) {
		// case 1, find key and node is leaf at node x
		//printf("case 1 \n");//debug
		for (int i = idx; i < x->key_len - 1; i++) {
			x->key_arr[i] = x->key_arr[i + 1];
		}
		x->key_len--;
		for (int i = idx; i < x->ptr_len - 1; i++) {
			x->ptr_arr[i] = x->ptr_arr[i + 1];
		}
		x->ptr_len--;
		return;
	}
	// case 3, key not found at node x
	else {
		//printf("idx = %d ", idx);//debug
		// case 3-0, if (ptr[idx] len > min_degree -1), go
		struct Node* ptr_x_temp1 = x->ptr_arr[idx];//debug ptr
		struct Node* ptr_x_temp2 = x->ptr_arr[idx - 1];
		struct Node* ptr_x_temp3 = x->ptr_arr[idx + 1];
		if (ptr_x_temp1->key_len > min_degree - 1) {
			//printf("case 3-0 \n");// debug
			Delete_key(tree, x->ptr_arr[idx], key);
			return;
		}
		else {
			struct Node* a = x->ptr_arr + (idx + 1);
			// case 3-A-1, if (left exist) and (ptr[left] len > min_degree -1), borrow
			if (idx != 0 && ptr_x_temp2->key_len > min_degree - 1) {
				//printf("case 3-A-1 \n");// debug
				Borrow_Left(x, idx);
				Delete_key(tree, x, key);
				return;
			}
			// case 3-A-2, if (right exist) and (ptr[right] len > min_degree -1), borrow
			else if (idx != x->ptr_len - 1 && ptr_x_temp3->key_len > min_degree - 1) {
				//printf("case 3-A-2 \n");// debug
				Borrow_Right(x, idx);
				Delete_key(tree, x, key);
				return;
			}
			// case 3-B, if if (left and right ptr len) < t, merge
			else {
				//printf("case 3-B \n");// debug
				struct Node* y = x->ptr_arr[key_idx];
				struct Node* z = x->ptr_arr[key_idx + 1];
				if (z->next_node != NULL) {
					y->next_node = z->next_node;
					z->next_node->prev_node = y;
				}
				else {
					y->next_node = NULL;
				}
				// Merge all x, y, z node's keys and ptrren into y node
				if (!y->leaf) {
					//printf("case 3-B is not leaf \n");// debug
					// merge x to y
					y->key_arr[y->key_len] = x->key_arr[key_idx];
					y->key_len++;
				}

				// merge z to y
				for (int i = 0; i < z->key_len; i++) {
					y->key_arr[y->key_len] = z->key_arr[i];
					y->key_len++;
				}
				// if y is not leaf node than ptr move

				for (int i = 0; i < z->ptr_len; i++) {
					y->ptr_arr[y->ptr_len] = z->ptr_arr[i];
					y->ptr_len++;
				}

				// Free z node
				free_node(z);

				// x key move
				for (int i = key_idx; i < x->key_len - 1; i++) {
					x->key_arr[i] = x->key_arr[i + 1];
				}
				x->key_len--;

				// x ptr move
				for (int i = key_idx + 1; i < x->ptr_len - 1; i++) {
					x->ptr_arr[i] = x->ptr_arr[i + 1];
				}
				x->ptr_len--;

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