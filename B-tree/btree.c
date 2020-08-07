#include <stdio.h>
#include "btree.h"
#include "new.h"
/*
* 2020-08-07
* today, I complete insert() and query().
* It looks like that I have saw delete(),
* do it late.
* 
* I create struct variable by malloc, but dosen't
* free it regularly when not used.
*
*/

/*
* 2020-08-05
* code the insert and query function.
* it's dirty and not clean, not debug
*
*/
node_t* Node_new (){
	node_t *p;
	NEW(p);
	p->key_num = 0;
	p->key = 0;
	p->pointer = 0;
	p->last = 0;
	p->next = 0;
	return p;
}

node_t null_node = NULL_NODE; // use as global varible.

b_tree* B_tree_new (int m){
	b_tree *p;
	NEW(p);
	p->m = m;
	p->node = Node_new ();
	return p;
}

key_list* Key_new (int value){
	key_list *p;
	NEW(p);
	p->value = value;
	p->data = 0;
	p->next = 0;
	return p;
}

pointer_list* Pointer_new (){
	pointer_list *p;
	NEW(p);
	p->node = 0;
	p->next = 0;
	return p;
}

void Key_insert (node_t *location, int value, pointer_list *p)
{
	key_list *tmp_key = Key_new (value);
	pointer_list *tmp_p = p;

	key_list *key = location->key;
	pointer_list *pointer = location->pointer;

	if (key == 0)
	{
		location->key = tmp_key;
		location->pointer = p;
		location->key_num++;
		if (DEBUG)
		{
			printf("key_insert : key 1== 0 %d \n",value);
		}
		return;
	}

	key_list *last_key = 0;
	pointer_list *last_p = 0;

	while (1)
	{
		if (value == key->value)
		{
			fprintf(stderr,"value : %d had been stored before!\n", value);
			return;
		}
		if (value < key->value)
		{
			if (DEBUG)
			{
				printf("key_insert : %d < %d \n",value, key->value);
			}
			if (last_key == 0)
			{
				location->key = tmp_key;
				tmp_key->next = key;
				location->pointer = tmp_p;
				tmp_p->next->next = pointer->next;
				location->key_num++;
				return;
			}
			else
			{
				last_key->next = tmp_key;
				tmp_key->next = key;
				last_p->next = tmp_p;
				tmp_p->next->next = pointer->next;
				location->key_num++;
				return;
			}
		}
		last_key = key;
		last_p = pointer;
		key = key->next;
		pointer = pointer->next;
		if (key == 0)
		{
			last_key->next = tmp_key;
			last_p->next = tmp_p;
			location->key_num++;
			return;
		}
	}
}

// operations
void B_tree_insert (b_tree *tree, int value){
	node_t *location = B_tree_query (tree, value);
	location = location->last;

	pointer_list *p1;
	pointer_list *p2;

	p1 = Pointer_new ();
	p2 = Pointer_new ();
	p1->next = p2;
	p2->next = 0;

	while (1)
	{
		if (location->key_num == tree->m - 1)
		{
			Key_insert (location, value, p1);
			if (T_DEBUG)
			{
				B_tree_print (tree);
			}
			node_t *node2 = Node_new ();
			node2->next = location->next;
			location->next = node2;

			location->key_num = tree->m / 2;
			node2->key_num = tree->m / 2 - 1;
			if (tree->m % 2)
			{
				node2->key_num += 1;
			}

			key_list *mid_key = location->key;
			pointer_list *mid_p = location->pointer;

			for (int i=1; i < location->key_num; i++)
			{
				mid_key = mid_key->next;
				mid_p = mid_p->next;
			}

			key_list *tmp_key = mid_key->next;
			mid_key->next = 0;
			mid_key = tmp_key;
			node2->key = mid_key->next;
			mid_key->next = 0;

			pointer_list *tmp_p = mid_p->next->next;
			mid_p->next->next = 0;
			mid_p = tmp_p;
			node2->pointer = mid_p;
			if (T_DEBUG)
			{
				B_tree_print (tree);
			}
			//mid_key
			//node1
			//node2
			//node->last
			p1 = Pointer_new ();
			p2 = Pointer_new ();
			p1->next = p2;
			p1->node = location;
			p2->node = node2;
			if (location->last == 0) // true ==> generate a root node
			{
				node_t *root = Node_new ();
				root->key = mid_key;
				root->pointer = p1;
				root->key_num = 1;
				location->last = node2->last = root;
				tree->node = root;
				if (T_DEBUG)
				{
					printf("last == 0 \n");
					B_tree_print (tree);
				}
				return;
			}
			node2->last = location->last;
			location = location->last;
			value =  mid_key->value;
		}
		else if (location->key_num < tree->m - 1)
		{
			if (DEBUG)
			{
				printf(" < \n");
			}
			Key_insert (location, value, p1);
			if (T_DEBUG)
			{
				printf("last != 0 \n");
				B_tree_print (tree);
			}
			return;
		}
	}
}

// not end node : replace with the most close right keyword until end
// end node : if keywords is enough, just delete it directly;
//			  else if it's too few to keep a node,
//					1. barrow from brother-node if can
//					2. merge upward until if can 
void B_tree_delete (b_tree *tree, int value){
	node_t *location = B_tree_query (tree, value);
	int min = (tree->m % 2 == 0) ? tree->m / 2 - 1 : tree->m / 2;
	if (location->key_num == 0)
	{
		fprintf(stderr, "Oh, this keyword is not stored in the tree \n");
		return;
	}
	// it's a end node
	if (location->pointer->node == 0)
	{
		if (location->key_num  >= min) // enough to delete directly
		{
			key_list *last_key = 0;
			pointer_list *last_p = 0;
			key_list *key = location->key;
			pointer_list *pointer = location->pointer;
			while (key)
			{
				if (value == key->value)
				{
					break;
				}
				last_key = key;
				last_p = pointer;
				key = key->next;
				pointer = pointer->next;
			}
			if (last_key == 0)
			{
				loaction->key = key->next;
				LOSE(key);
				location->pointer = pointer->next;
				LOSE(pointer);
			}
			else
			{
				last->key->next = key->next;
				last->p->next = pointer->next;
				LOSE(key);
				LOSE(pointer);
			}
		}
		else
		{
			// TODO 
		}
	}
}

// return
//		node->key_num == 0 ==> not find
//		node->key_num != 0 ==> find it
node_t* B_tree_query (b_tree *tree, int value){
	node_t *cur_node = tree->node;
	if (cur_node->key == 0)
	{
		null_node.last = cur_node;
		return &null_node;
	}
	key_list *key = cur_node->key;
	pointer_list *pointer = cur_node->pointer;
	while (1)
	{
		if (value == key->value)
		{
			return cur_node;
		}
		if (value > key->value)
		{
			key = key->next;
			pointer = pointer->next;
			if ( key )
			{
				continue;
			}
		}
		// value < key->value
		if (pointer->node == 0)		 // or key == 0
		{
			null_node.last = cur_node;
			return &null_node;
		}
		cur_node = pointer->node;
		key = cur_node->key;
		pointer = cur_node->pointer;
	}
}

void B_tree_print (b_tree *tree){
	node_t *queue_head;
	queue_head = tree->node;
	
	int count = 1;

	while (queue_head != 0 && queue_head->key_num != 0)
	{
		printf("LAY %d : \n",count++);

		node_t *node = queue_head;
		while (node != 0)
		{
			key_list *key = node->key;
			while (key)
			{
				printf("%d  ", key->value);
				key = key->next;
			}
			printf(" ** ");
			node = node->next;
		}
		printf("\n\n");
		queue_head = queue_head->pointer->node;
	}
}