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
 * /

typedef struct node_t *node_t;
typedef struct key_list *key_list;
typedef struct pointer_list *pointer_list;
struct node_t {
	void *data;
	key_list key;
	pointer_list pointer;
}

struct key_list {
	int value;
	void *data;
	struct key_list *next;
}

struct pointer_list {
	node_t node;
	struct pointer_list *next;
}

// new
node_t Node_new (int m);
key_list Key_new (int value);
pointer_list Pointer_new ();

// operations
void Node_insert (node_t node, int value);
void Node_delete (node_t node, int value);
node_t Node_query (node_t node, int value);

