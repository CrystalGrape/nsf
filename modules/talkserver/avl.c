#include "avl.h"
#include <memory.h>
int node_amount = 0;
int del_node_amount = 0;
int del_filedesc[500];
int del_filedesc_amount=0;
//前面大于后面返回1，相等返回0，后面大于前面返回-1
int nsf_avldata_compare(NsfntAvldata front, NsfntAvldata back)
{
	if(strcmp(front.user_id,back.user_id) == 0)
		return 0;
	if (strcmp(front.user_id,back.user_id) > 0)
		return 1;
	if (strcmp(front.user_id,back.user_id) < 0)
		return -1;
}

int nsf_avldata_comparekey(NsfntAvldata front, NsfntAvlkey key)
{
	if(strcmp(front.user_id,key) == 0)
		return 0;
	if (strcmp(front.user_id,key) > 0)
		return 1;
	if (strcmp(front.user_id,key) < 0)
		return -1;
}

void nsf_avltree_rrotate(AVLTree *root)
{
	AVLNode *lc = (*root)->lchild;
	(*root)->lchild = lc->rchild;
	lc->rchild = (*root);
	(*root) = lc;
}

void nsf_avltree_lrotate(AVLTree *root)
{
	AVLNode *rc = (*root)->rchild;
	(*root)->rchild = rc->lchild;
	rc->lchild = (*root);
	(*root) = rc;
}

int nsf_avltree_insert(AVLTree *tree, NsfntAvldata data, int *taller)
{
	if (!(*tree)){
		(*tree) = (AVLTree)malloc(sizeof(AVLNode));
		(*tree)->data = data;
		memcpy(&((*tree)->data), &data, sizeof(data));
		(*tree)->lchild = (*tree)->rchild = NULL;
		(*tree)->bf = EH;
		(*tree)->del_flag = 0;
		(*taller) = 1;
		node_amount++;
	}
	else{
		if (nsf_avldata_compare(data, (*tree)->data) == 0){
			if((*tree)->del_flag == 1)
				del_node_amount--;
			//已有节点，重新更新
			memcpy(&((*tree)->data), &data, sizeof(data));
			(*tree)->del_flag = 0;
			(*taller) = 0;
			return 0;
		}

		if (nsf_avldata_compare(data, (*tree)->data) < 0){
			if (!nsf_avltree_insert(&((*tree)->lchild), data, taller))
				return 0;
			if ((*taller)){
				switch ((*tree)->bf){
				case LH:
					nsf_avltree_leftbalance(tree);
					(*taller) = 0;
					break;
				case EH:
					(*tree)->bf = LH;
					(*taller) = 1;
					break;
				case RH:
					(*tree)->bf = EH;
					(*taller) = 0;
					break;
				}
			}
		}else{
			if (!nsf_avltree_insert(&((*tree)->rchild), data, taller))
				return 0;
			if ((*taller)){
				switch ((*tree)->bf){
				case LH:
					(*tree)->bf = EH;
					(*taller) = 0;
					break;
				case EH:
					(*tree)->bf = RH;
					(*taller) = 1;
					break;
				case RH:
					nsf_avltree_rightbalance(tree);
					(*taller) = 0;
					break;
				}
			}
		}
	}
	return 1;
}

void nsf_avltree_leftbalance(AVLTree *tree)
{
	AVLNode *lc = (*tree)->lchild;
	AVLNode *rd;
	switch (lc->bf){
	case LH:
		lc->bf = EH;
		(*tree)->bf = lc->bf;
		nsf_avltree_rrotate(tree);
		break;
	case RH:
		rd = lc->rchild;
		switch (rd->bf){
		case LH:
			(*tree)->bf = RH;
			lc->bf = EH;
			break;
		case EH:
			lc->bf = EH;
			(*tree)->bf = lc->bf;
			break;
		case RH:
			(*tree)->bf = EH;
			lc->bf = LH;
			break;
		}
		rd->bf = EH;
		nsf_avltree_lrotate(&((*tree)->lchild));
		nsf_avltree_rrotate(tree);
		break;
	}
}

void nsf_avltree_rightbalance(AVLTree *tree)
{
	AVLNode *rc = (*tree)->rchild;
	AVLNode *rl;

	switch (rc->bf){
	case RH:
		rc->bf = EH;
		(*tree)->bf = rc->bf;
		nsf_avltree_lrotate(tree);
		break;
	case LH:
		rl = rc->lchild;
		switch (rl->bf){
		case LH:
			(*tree)->bf = EH;
			rc->bf = RH;
			break;
		case EH:
			rc->bf = EH;
			(*tree)->bf = rc->bf;
			break;
		case RH:
			(*tree)->bf = LH;
			rc->bf = EH;
			break;
		}
		rl->bf = EH;
		nsf_avltree_rrotate(&((*tree)->rchild));
		nsf_avltree_lrotate(tree);
		break;
	}
}

int nsf_avltree_del(AVLTree root, NsfntAvlkey key)
{
	if (root == NULL)
		return 0;
	if(root->del_flag == 1)
		return 0;
	if (nsf_avldata_comparekey(root->data, key) == 0)
	{
		root->del_flag =1;
		del_node_amount++;
		return 1;
	}
	else if (nsf_avldata_comparekey(root->data, key) > 0)
	{
		return nsf_avltree_del(root->lchild, key);
	}
	else
	{
		return nsf_avltree_del(root->rchild, key);
	}
}

NsfntAvldata *nsf_avltree_find(AVLTree root, NsfntAvlkey key)
{
	if (root == NULL)
		return NULL;
	if (nsf_avldata_comparekey(root->data, key) == 0)
		return &(root->data);
	else if (nsf_avldata_comparekey(root->data, key) > 0)
		return nsf_avltree_find(root->lchild, key);
	else
		return nsf_avltree_find(root->rchild, key);
}

void nsf_avltree_rebuild(AVLTree *root, AVLTree *newtree)
{
	int taller = 0; 
	if(NULL != (*root))
	{
		nsf_avltree_rebuild(&(*root)->lchild, newtree);
		nsf_avltree_rebuild(&(*root)->rchild, newtree);
		if((*root)->del_flag == 0)
			nsf_avltree_insert(newtree, (*root)->data, &taller);
		free((*root));
	}
}

void nsf_avltree_rebuilt(AVLTree *root, AVLTree *newtree)
{
	int taller = 0; 
	int i;
	int flag;
	if(NULL != (*root))
	{
		nsf_avltree_rebuilt(&(*root)->lchild, newtree);
		nsf_avltree_rebuilt(&(*root)->rchild, newtree);
		
		flag = 0;
		for(i = 0; i < 500; i++){
			if(del_filedesc[i] == (*root)->data.clt_fd){
				flag = 1;
				break;
			}
		}
		if(flag == 0)
			nsf_avltree_insert(newtree, (*root)->data, &taller);
		free((*root));
	}
}

int nsf_avltree_scale()
{
	if(del_node_amount == 0)
		return 1;
	return (node_amount/del_node_amount);
}

int nsf_avltree_delfd(int fd)
{
	del_filedesc[del_filedesc_amount] = fd;
	del_filedesc_amount++;
	if(del_filedesc_amount == 500)
		return 1;
	return 0;
}
