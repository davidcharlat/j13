#include "ft_btree.h"
#include <stdlib.h>
	#include <stdio.h>

typedef	struct	s_list
{
	struct	s_list	*next;
	void			*data;
	int				level;
}					t_list;

#define CUR_NODE_ADD ((t_btree*)(cursor->data))

t_list	*ft_create_elem (void *data, int current_level)
{
	t_list	*ptr;
	
	ptr = (t_list*)malloc(sizeof(t_list));
	if (ptr)
	{
		ptr->data = data;
		ptr->next = NULL;
		ptr->level = current_level;
	}
	return (ptr);	
}

void	ft_list_add_node(t_list **begin_list, void *ptr, int current_level)
{
	t_list *temp;
	t_list *addr_next;
	
	temp = ft_create_elem (ptr, current_level);
	if (*begin_list)
	{
		addr_next = *begin_list;
		while (addr_next->next)
			addr_next = addr_next->next;
		if (temp)
			addr_next->next = temp;
	}
	else
	{
		if (temp)
			*begin_list = temp;
	}
}


void	btree_apply_by_level(t_btree *root, void (*applyf)(void *item,
							int current_level, int is_first_elem))
{
	t_list	*nodes;
	t_list	*cursor;
	int		prec_level;
	
	if (root)
	{
		nodes = NULL;
		prec_level = 0;
		if (root->left)
			ft_list_add_node (&nodes, root->left, 1);
		if (root->right)
			ft_list_add_node (&nodes, root->right, 1);
		applyf (root->item, 0, 1);
		cursor = nodes;	
		while (cursor)
		{
			if (CUR_NODE_ADD->left)
				ft_list_add_node (&nodes, CUR_NODE_ADD->left, cursor->level + 1);
			if (CUR_NODE_ADD->right)
				ft_list_add_node (&nodes, CUR_NODE_ADD->right, cursor->level + 1);
			applyf (CUR_NODE_ADD->item, cursor->level, cursor->level - prec_level);
			if (cursor->next)
				prec_level = cursor->level;
			cursor = cursor->next;
		}
		while (nodes)
		{
			cursor = nodes;
			nodes = nodes->next;
			free (cursor);
		}
	}
}
