#include "ft_btree_rb.h"
#include <stdlib.h>

#define PARENT node->parent
#define GPARENT PARENT->parent

t_rb_node	*rb_tree_create_node (void *item)
{
	t_rb_node	*ptr;
	
	ptr = (t_rb_node*)malloc(sizeof(t_rb_node));
	if (ptr)
	{
		ptr->data = item;
		ptr->left = NULL;
		ptr->right = NULL;
		ptr->parent = NULL;
		ptr->color = RB_RED;
	}
	return (ptr);	
}

void	insert_node (struct s_rb_node **root, struct s_rb_node *node, int (*cmpf)(void *, void *))
{
	t_rb_node	*next;
	t_rb_node	*parent;

	next = *root;
	parent = next;
	while (next)
	{
		parent = next;
		if (cmpf (node->data, next->data) < 0)
			next = next->left;
		else
			next = next->right;
	}
	if (!*root)
		*root = node;
	else
	{
		if (cmpf (node->data, parent->data) < 0)
			parent->left = node;
		else
			parent->right = node;
	}
	PARENT = parent;
}

void	rotate_tree (struct s_rb_node *node, struct s_rb_node **root, int dir)
{
	t_rb_node	*temp;
	
	if (dir == 1)
	{
		if (!PARENT)
			*root = node->left;
		else if (PARENT->right == node)
			PARENT->right = node->left;
		else 
			PARENT->left = node->left;
		temp = node->left->right;
		node->left->parent = PARENT;
		if (node->left->right)
			node->left->right->parent = node;
		node->left->right = node;
		PARENT = node->left;
		node->left = temp;
	}
	if (dir == -1)
	{
		if (!PARENT)
			*root = node->right;
		else if (PARENT->right == node)
			PARENT->right = node->right;
		else 
			PARENT->left = node->right;
		temp = node->right->left;
		node->right->parent = PARENT;
		if (node->right->left)
			node->right->left->parent = node;
		node->right->left = node;
		PARENT = node->right;
		node->right = temp;
	}
}

void	repare_tree (struct s_rb_node *node, struct s_rb_node **root)
{
	if (PARENT == NULL)
    	node->color = RB_BLACK;
    else if (PARENT->color == RB_RED && GPARENT->left && GPARENT->right
    		 && GPARENT->left->color == GPARENT->right->color)
    {
		GPARENT->left->color = RB_BLACK;
		GPARENT->right->color = RB_BLACK;
		GPARENT->color = RB_RED;
		repare_tree (GPARENT, root);
	}
	else if (PARENT->color == RB_RED)
	{
		if (GPARENT->left && GPARENT->left->right == node)
		{
			rotate_tree (PARENT, root, -1); // -1 = left
			node = node->left;
		}
		else if (GPARENT->right && GPARENT->right->left == node)
		{
			rotate_tree (PARENT, root, 1); // 1 = right
			node = node->right;
		}
		GPARENT->color = RB_RED;
		if (node == PARENT->left)
			rotate_tree (GPARENT, root, 1);
		else
			rotate_tree (GPARENT, root, -1);
		PARENT->color = RB_BLACK;
    }
}

void	rb_insert(struct s_rb_node **root, void *data, int (*cmpf)(void *, void *))
{
	t_rb_node	*node;
	
	node = rb_tree_create_node (data);
	insert_node (root, node, cmpf);
	repare_tree (node, root);
}
