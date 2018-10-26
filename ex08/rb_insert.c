#include "ft_btree_rb.h"
#include <stdlib.h>
	#include <stdio.h>

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
//	t_rb_node	*gparent;
	
//	gparent = NULL;
//printf("*root :%p",*root);
	next = *root;
	parent = next;
	while (next)
	{
//printf ("next = %p\n", next);
//		gparent = parent;
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
//printf("parent :%p\n", parent);
		if (cmpf (node->data, parent->data) < 0)
			parent->left = node;
		else
			parent->right = node;
	}
//printf("parent :%p\n", parent);
//printf("nodeparent :%p\n", node->parent);
	node->parent = parent;
//printf("nodeparent :%p\n", node->parent);
//printf ("node color :%d\n", node->color);
//if (parent){printf("parentcolor%d\n", node->parent->color);}
}

void	rotate_tree (struct s_rb_node *node, struct s_rb_node **root, int dir) //dir -1 = left, 1=right
{
	t_rb_node	*temp;
//	t_rb_node	*cpy;
	
//	cpy = *node;
//printf ("start rotate tree %d: %s  &parent: %p\n", dir, (char*)node->data, node->parent);
//printf ("%p ",node->parent);
	if (node->parent == NULL && dir == 1)
		*root = node->left;
	else if (!node->parent && dir == -1)
		*root = node->right;
	else if (dir == 1)
	{
		if (node->parent->right == node)
			node->parent->right = node->left;
		else 
			node->parent->left = node->left;
	}
	else if (dir == -1)
	{
		if (node->parent->right == node)
			node->parent->right = node->right;
		else 
			node->parent->left = node->right;
	}
	if (dir == 1)
	{
		temp = node->left->right;
		node->left->parent = node->parent;
		if (node->left->right)
			node->left->right->parent = node;
		node->left->right = node;
		node->parent = node->left;
		node->left = temp;
		
//		cpy = (cpy)->left;
	}
	if (dir == -1)
	{
		temp = node->right->left;
		node->right->parent = node->parent;
		if (node->right->left)
			node->right->left->parent = node;
		node->right->left = node;
		node->parent = node->right;
		node->right = temp;
/*	
		
printf ("node %p, *node %p, ->parent %p, ->left %p, ->right %p, ->data %s",node, *node, (*node)->parent, (*node)->left, (*node)->right, (char*)(*node)->data);
printf("start left %p (val: %s, parent: %p, left: %p, right: %p)\n", cpy, (char*)(cpy)->data, (cpy)->parent, (cpy)->left, (cpy)->right);
		temp = (cpy)->right;
printf("futur parent du sommet :%p\n",(cpy)->parent);
printf("temp :%p\n",temp);
		(*node)->right->parent = (cpy)->parent;
		(*node)->right->left = cpy;
printf("fils du sommet :%p\n",cpy);
printf("node :%p",cpy);
		if (temp->left)
			(*node)->right->left->parent = temp;
printf("parent du droit du fils :%p\n",temp);
printf("parent du sommet :%p\n",(cpy)->parent);
		*node = (cpy)->right;
printf("nouveau sommet affecté");
		
		(*node)->parent = (cpy)->right;
printf("parent du fils :%p\n",(cpy)->right);
		(*node)->right = temp->left;
printf("droit du fils :%p\n",temp->left);
*/	
	
	}
//printf ("end rotate tree %d\n", dir);
}

void	repare_tree (struct s_rb_node *node, struct s_rb_node **root)
{

	if (node->parent == NULL)
//{printf ("repare tree case 1\n");
    	node->color = RB_BLACK;
//}
    else if (node->parent->color == RB_RED && node->parent->parent->left && node->parent->parent->right
    		 && node->parent->parent->left->color == node->parent->parent->right->color)
    {
//printf ("repare tree case 3\n");
		node->parent->parent->left->color = RB_BLACK;
		node->parent->parent->right->color = RB_BLACK;
		node->parent->parent->color = RB_RED;
		repare_tree (node->parent->parent, root);
	}
	else if (node->parent->color == RB_RED)
	{
//printf ("repare tree case 4\n");
		if (node->parent->parent->left && node->parent->parent->left->right == node)
		{
//printf ("*root %p\n", *root);
//printf("left, nodeparent :%p\n", node->parent);
//printf("&node :%p\n", &(node->parent));
			rotate_tree (node->parent, root, -1); //-1 = left
			node = node->left;
//printf ("done *root %p\n", *root);
		}
		else if (node->parent->parent->right && node->parent->parent->right->left == node)
		{
//printf ("*root %p\n", *root);
//printf("left, nodeparent :%p\n", node->parent);
//printf("&nodeparent :%p\n", &(node->parent));
			rotate_tree (node->parent, root, 1); //1 = right
			node = node->right;
//printf ("done; *root %p\n", *root);
		}
//printf ("*root %p\n", *root);
//printf( "nodeparent :%p\n", node->parent);
//printf("&nodeparent :%p\n", &(node->parent));
		node->parent->parent->color = RB_RED;
//printf( "nodeparent left :%p\n", node->parent->left);
//printf( "nodeparent right :%p\n", node->parent->right);
		if (node == node->parent->left)
			rotate_tree (node->parent->parent, root, 1);
		else
			rotate_tree (node->parent->parent, root, -1);
//printf ("done, *root %p\n", *root);
		node->parent->color = RB_BLACK;  //attention à ces deux derniers changement de couleur
    }
//else printf ("repare tree case 2\n");
}

void	rb_insert(struct s_rb_node **root, void *data, int (*cmpf)(void *, void *))
{
	t_rb_node	*node;
//printf("start rb insert: data = %s\n", (char*)data);
	
	node = rb_tree_create_node (data);
//printf("end create node\n");
	insert_node (root, node, cmpf);
//printf("end insert node\n");	
	repare_tree (node, root);
//printf("end repare tree\n");
}
