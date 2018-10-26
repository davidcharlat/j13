#include "ft_btree.h"

void	btree_search_item_test(t_btree *root,
				void *data_ref, int (*cmpf)(void *, void *), void **test)
{
	if (root && !(*test))
	{
		if (root->left)
			btree_search_item_test (root->left, data_ref, cmpf, test);
		if (!cmpf (data_ref, root->item))
			*test = (root->item);
		if (root->right)
			btree_search_item_test (root->right, data_ref, cmpf, test);
	}
}

void	*btree_search_item(t_btree *root,
						 void *data_ref, int (*cmpf)(void *, void *))
{
	void*	test;
	
	test = (void*)0;
	btree_search_item_test(root, data_ref, cmpf, &test);
	return (test);
}
