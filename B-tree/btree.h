#ifndef BTREE_H
#define BTREE_H

#define DEBUG 0

#define T_DEBUG 0

/*
 *
 * author: CTY
 * time: 2020-08-04
 * type: B树 
 * definition: 
 * 	1. 所有节点关键字递增排列，左小右大，互不相等;
 * 	2. 根节点至少两个子节点;
 * 	3. 非根非叶节点子节点数n: n的max值/2 向上取整 <= n <= n的max值;
 * 	4. 叶子节点处于同一层，可用空指针表示;
 *
 */

typedef struct b_tree b_tree;
typedef struct node_t node_t;
struct b_tree
{
	int m;
	node_t *node;
};

typedef struct key_list key_list;
typedef struct pointer_list pointer_list;
struct node_t {
	int key_num;
	key_list *key;
	pointer_list *pointer;
	struct node_t *last;
	struct node_t *next; // point to next  horizontally
};

struct key_list {
	int value;
	void *data;
	struct key_list *next;
};

struct pointer_list {
	node_t *node;
	struct pointer_list *next;
};

// new
b_tree* B_tree_new (int m);
node_t* Node_new ();
key_list* Key_new (int value);
pointer_list* Pointer_new ();

// operations
void B_tree_insert (b_tree *tree, int value);
void B_tree_delete (b_tree *tree, int value);
node_t* B_tree_query (b_tree *tree, int value);
void B_tree_print (b_tree *tree);

#define NULL_NODE {0,0,0,0,0}

#endif