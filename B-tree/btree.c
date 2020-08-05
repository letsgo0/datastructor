#include <stdio.h>
#include "btree.h"
#include "new.h"
/*
* 2020-08-05
* code the insert and query function.
* it's dirty and not clean, not debug
*
*/
node_t null_node = {0,0,0,0}; // use ->last to convey some info

node_t Node_new (){
	node_t p;
	NEW(p);
	p->key_num = 0;
	p->key = 0;
	p->pointer = 0;
	p->last = 0;
	return p;
}

b_tree B_tree_new (int m){
	b_tree p;
	NEW(p);
	p->m = m;
	p->node = Node_new ();
	return p;
}

key_list Key_new (int value){
	key_list p;
	NEW(p);
	p->value = value;
	p->data = 0;
	p->next = 0;
	return p;
}

pointer_list Pointer_new (){
	pointer_list p;
	NEW(p);
	p->node = 0;
	p->next = 0;
	return p;
}

void Key_insert (node_t location, int value, pointer_list p)
{
	key_list tmp_key = Key_new (value);
	if (key == 0)
	{
		location->key = tmp_key;
		location->pointer = p;
		return;
	}

	key_list key = location->key;
	pointer_list pointer = location->pointer;
	key_list last_key = 0;
	pointer_list last_p = 0;
	Pointer_list tmp_p = p;
	while (1)
	{
		if (value == key->value)
		{
			fprintf(stderr,"value : %d had been stored before!\n", value);
			return;
		}
		if (value < key->value)
		{
			if (last_key == 0)
			{
				location->key = tmp_key;
				tmp_key->next = key;
				location->pointer = tmp_p;
				tmp_p->next->next = pointer->next;
				return;
			}
			else
			{
				last_key->next = tmp_key;
				tmp_key->next = key;
				last_p->next = tmp_p;
				tmp_p->next->next = pointer->next;
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
			return;
		}
	}
	location->key_num++;
}

// operations
void B_tree_insert (b_tree tree, int value){
	node_t location = B_tree_query (value);
	location = location->last;

	pointer_list p1;
	pointer_list p2;

	p1 = Pointer_new ();
	p2 = Pointer_new ();
	p1->next = p2;

	while (1)
	{
		if (location->key_num == tree->m - 1)
		{
			Key_insert (location, value, p1);

			node_t node1 = Node_new ();
			node_t node2 = Node_new ();
			node1->key_num = tree->m / 2;
			node2->key_num = tree->m / 2 - 1;
			if (tree->m % 2)
			{
				node2->key_num = tree->m - node1->key_num + 1;
			}
			key_list mid_key = location->key;
			pointer_list mid_p = location->pointer;
			node1->key = location->key;
			node1->pointer = location->pointer;

			for (int i=1; i < node1->key_num; i++)
			{
				mid_key = mid_key->next;
				mid_p = mid_p->next;
			}
			key_list tmp_key = mid_key->next;
			mid_key->next = 0;
			mid_key = tmp_key;
			pointer_list tmp_p = mid_p->next->next;
			mid_p->next->next = 0;
			mid_p = tmp_p;
			node2->key = mid_key->next;
			node2->pointer = mid_p;
			mid_key->next = 0;
			//mid_key
			//node1
			//node2
			//node->last
			p1->node = node1;
			p2->node = node2;
			location = node1->last;
			value =  mid_key->value;
			if (node1->last == 0) // true ==> generate a root node
			{
				node_t root = Node_new ();
				root->key_num = 1;
				root->key = mid_key;
				root->pointer = p1;
				node1->last = node2->last = root;
				return;
			}
		}
		else if (location->key_num < tree->m - 1)
		{
			Key_insert (location, value, p1);
			return;
		}
	}
}

void Node_delete (node_t node, int value){

}

// return
//		node->key_num == 0 ==> not find
//		node->key_num != 0 ==> find it
node_t B_tree_query (b_tree tree, int value){
	node_t cur_node = tree->node;
	if (cur_node->key == 0)
	{
		null_node->last = cur_node;
		return null_node;
	}
	key_list key = cur_node->key;
	pointer_list pointer = cur_node->pointer;
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
			null_node->last = cur_node;
			return null_node;
		}
		cur_node = pointer->node;
		key = cur_node->key;
		pointer = cur_node->pointer;
	}
}