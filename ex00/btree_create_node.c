#include <stdlib.h>
#include "ft_btree.h"

t_btree	*btree_create_node (void *item)
{
	t_btree	*ptr;
	
	ptr = NULL;
	ptr = (t_btree*)malloc(sizeof(t_btree));
	if (ptr)
	{
		ptr->item = item;
		ptr->left = NULL;
		ptr->right = NULL;
	}
	return (ptr);	
}
