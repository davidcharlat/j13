#include "ft_btree_rb.h"
#include <stdlib.h>
	#include <stdio.h>
#define PARENT node->parent
#define GPARENT PARENT->parent

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

void	repare_neg_color (struct s_rb_node *node, struct s_rb_node **root)
{
	if (*root == node)
		node->color = RB_BLACK;
	else if (PARENT->left == node && PARENT->right->color == RB_BLACK)
	{	
		if (PARENT->right->right && PARENT->right->right->color == RB_RED)
		{
			PARENT->right->color = PARENT->color;
			node->color = RB_BLACK;
			PARENT->color = RB_BLACK;
			PARENT->right->right->color = RB_BLACK;
			rotate_tree (node->parent, root, -1);
		}
		else if (PARENT->right->right && PARENT->right->right->color == RB_BLACK
			&& PARENT->right->left->color == RB_RED)
		{
			PARENT->right->left->color = RB_BLACK;
			rotate_tree (PARENT->right, root, 1);
			node->color = RB_BLACK;
			PARENT->color = RB_BLACK;
			rotate_tree (PARENT, root, -1);
		}
		else
		{
			PARENT->right->color = RB_RED;
			PARENT->color--;
			node->color++;
			if (PARENT->color == -1)
				repare_neg_color (PARENT, root);
		}
	}
	else if (PARENT->right == node && PARENT->left->color == RB_BLACK)
	{	
		if (PARENT->left->left && PARENT->left->left->color == RB_RED)
		{
			PARENT->left->color = PARENT->color;
			node->color = RB_BLACK;
			PARENT->color = RB_BLACK;
			PARENT->left->left->color = RB_BLACK;
			rotate_tree (node->parent, root, 1);
		}
		else if (PARENT->left->left && PARENT->left->left->color == RB_BLACK
			&& PARENT->left->right->color == RB_RED)
		{
			PARENT->left->right->color = RB_BLACK;
			rotate_tree (PARENT->left, root, -1);
			node->color = RB_BLACK;
			PARENT->color = RB_BLACK;
			rotate_tree (PARENT, root, 1);
		}
		else
		{
			PARENT->left->color = RB_RED;
			PARENT->color--;
			node->color++;
			if (PARENT->color == -1)
				repare_neg_color (PARENT, root);
		}
	}
	else 
	{
		PARENT->color = RB_RED;
		if (PARENT->left == node)
			PARENT->right->color = RB_BLACK;
		else
			PARENT->left->color = RB_BLACK;
		if (PARENT->left == node)
			rotate_tree (PARENT, root, -1);
		else
			rotate_tree (PARENT, root, 1);
		repare_neg_color (node, root);
	}
}

void	repare_fr_removed_parent (t_rb_node **root, t_rb_node *nodep, int pos)
{
	if (nodep->color == RB_BLACK && ((pos == 1 && nodep->left->color == RB_RED)
					|| (pos == -1 && nodep->right->color == RB_RED)))
	{
		nodep->color == RB_RED;
		if (pos == -1)
			nodep->right->color = RB_BLACK;
		if (pos == 1)
			nodep->left->color = RB_BLACK;
		if (pos == 1 && nodep->left->right)
			nodep->left->right->color = RB_RED;
		if (pos == -1 && nodep->right->left)
			nodep->right->left->color = RB_RED;
		rotate_tree (nodep, root, pos);
	}
	else if (nodep->color == RB_RED)
		rotate_tree (nodep, root, pos);
	else
	{
		nodep->color = RB_RED;
		if (pos == -1)
			nodep->right->color = -1;
		if (pos == 1)
			nodep->left->color = -1;
		rotate_tree (nodep, root, pos);
		repare_neg_color (nodep->parent, root);
	}
}

void	remove_to_delete (t_rb_node **root, t_rb_node *temp,
			t_rb_node *to_delete, void (*freef)(void *))
{
	int	position;
	
	if (to_delete && to_delete->parent && to_delete == to_delete->parent->left)
		position = -1; // -1 = left
	if (to_delete && to_delete->parent && to_delete == to_delete->parent->right)
		position = 1; // 1 = right
	if (to_delete && to_delete != *root)
	{
		if (to_delete->parent->left == to_delete)
			to_delete->parent->left = to_delete->right;
		else
			to_delete->parent->right = to_delete->right;
		if (to_delete->right)
			to_delete->right->parent = to_delete->parent;
		temp->data = to_delete->data;
	}
	else if (to_delete && to_delete != *root && to_delete == temp && temp->left)
	{
		to_delete->parent->left = to_delete->right;
		if (to_delete->right)
			to_delete->right->parent = to_delete->parent;
	}
	else if (*root && to_delete == *root)
	{
		*root = to_delete->right;
		if (to_delete->right)
			to_delete->right->parent = NULL;
	}
	if (to_delete && to_delete->color == RB_BLACK && to_delete->left)
		to_delete->left->color = RB_BLACK;
	else if (to_delete && to_delete->color == RB_BLACK && to_delete->right)
		to_delete->right->color = RB_BLACK;
	else if (to_delete && to_delete->parent && to_delete->color == RB_BLACK)
		repare_fr_removed_parent (root, to_delete->parent, position);
	freef ((void*)to_delete);
}

void	rb_remove(struct s_rb_node **root, void *data,
			int (*cmpf)(void *, void *), void (*freef)(void *))
{
	t_rb_node	*temp;
	t_rb_node	*to_delete;
	
	temp = *root;
	to_delete = NULL;
	while (temp && cmpf(data, temp->data))
	{
		if (cmpf(data, temp->data) < 0)
			temp = temp->left;
		else if (cmpf(data, temp->data) > 0)
			temp = temp->right;
	}
	if (temp && !cmpf(data, temp->data))
	{
		to_delete = temp;
		if (temp->right)
		{
			to_delete = temp->right;
			while (to_delete->left)
				to_delete = to_delete->left;
		}
		else if (temp->left)
			to_delete = temp->left;
	}
	remove_to_delete (root, temp, to_delete, freef);
}

