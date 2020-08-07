#include <stdio.h>
#include "btree.h"

#define N 20

int main(int argc, char *argv[])
{
	printf("Hello, world\n");
	b_tree*  tree = B_tree_new (5);
	int array[N] = {1,2,6,7,
		11,4,8,13,
		10,5,17,9,
		16,20,3,12,
		14,18,19,15};
	for (int i = 0; i < N ; i++)
	{
		B_tree_insert (tree, array[i]);
	}
	B_tree_print (tree);
	return 0;
}
