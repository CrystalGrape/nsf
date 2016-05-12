#ifndef avl_h
#define avl_h
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#define LH	1
#define EH	0
#define RH -1

typedef struct nsf_user_map NsfntAvldata;
typedef char * NsfntAvlkey;

typedef struct avl_tree_node
{
	NsfntAvldata data;
	int bf;
	int del_flag;
	struct avl_tree_node *lchild, *rchild;
}AVLNode, *AVLTree;

int nsf_avldata_compare(NsfntAvldata front, NsfntAvldata back);
int nsf_avldata_comparekey(NsfntAvldata front, NsfntAvlkey key);
void nsf_avltree_rrotate(AVLTree *root);
void nsf_avltree_lrotate(AVLTree *root);
int nsf_avltree_insert(AVLTree *tree, NsfntAvldata data, int *taller);
void nsf_avltree_leftbalance(AVLTree *tree);
void nsf_avltree_rightbalance(AVLTree *tree);
NsfntAvldata *nsf_avltree_find(AVLTree root, NsfntAvlkey key);
int nsf_avltree_del(AVLTree root, NsfntAvlkey key);
#endif
