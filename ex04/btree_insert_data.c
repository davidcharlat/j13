#include "ft_btree.h"

t_btree	*btree_create_node (void *item);

void	btree_insert_data(t_btree **root, void *item, int(*cmpf)(void*, void*))
{
	t_btree *next;
	t_btree *parent;
	
	next = *root;
	while (next)
	{
		parent = next;
		if (cmpf (item, next->item) < 0)
			next = next->left;
		else
			next = next->right;
	}
	if (!*root)
		*root = btree_create_node (item);
	else
	{
		if (cmpf (item, parent->item) < 0)
			parent->left = btree_create_node (item);
		else
			parent->right = btree_create_node (item);
	}
	
}

