#include "ft_btree.h"

void	btree_level_count_test(t_btree *root, int *level, int *max)
{
	if (root)
	{
		*level = *level + 1;
		if (*max < *level)
				*max = *level;
		if (root->left)
			btree_level_count_test (root->left, level, max);
		if (root->right)
			btree_level_count_test (root->right, level, max);
		*level = *level - 1;
	}
}

int		btree_level_count(t_btree *root)
{
	int	level;
	int max;
	
	max = 0;
	level = 0;
	btree_level_count_test (root, &level, &max);
	return (max);
}
