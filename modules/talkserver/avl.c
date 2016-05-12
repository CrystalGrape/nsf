#include <stdio.h>
#include <stdlib.h>
#define LH	1
#define EH	0
#define RH -1

typedef int NsfntAvldata;
typedef int NsfntAvlkey;

typedef struct avl_tree_node
{
	NsfntAvldata data;
	int bf;
	struct avl_tree_node *lchild,*rchild;
}AVLNode,*AVLTree;

int nsf_avldata_compare(NsfntAvldata front, NsfntAvldata back);
void nsf_avltree_rrotate(AVLTree *root);
void nsf_avltree_lrotate(AVLTree *root);
int nsf_avltree_insert(AVLTree *tree, NsfntAvldata data, int *taller);
void nsf_avltree_leftbalance(AVLTree *tree);
void nsf_avltree_rightbalance(AVLTree *tree);
NsfntAvldata *nsf_avltree_find(AVLTree root, NsfntAvldata key);
//前面大于后面返回1，相等返回0，后面大于前面返回-1
int nsf_avldata_compare(NsfntAvldata front, NsfntAvldata back)
{
	if(front > back)
		return 1;
	if(front == back)
		return 0;
	if(front < back)
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
	if(!(*tree)){
		(*tree) = (AVLTree)malloc(sizeof(AVLNode));
		(*tree)->lchild = (*tree)->rchild = NULL;
		(*tree)->bf = EH;
		(*taller) = 1;
	}else{
		if(nsf_avldata_compare(data, (*tree)->data) == 0){
			(*taller) = 0;
			return 0;
		}
		
		if(nsf_avldata_compare(data, (*tree)->data) < 0){
			if(!nsf_avltree_insert(&((*tree)->lchild), data, taller))
				return 0;
			if((*taller)){
				switch((*tree)->bf){
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
			if(!nsf_avltree_insert(&((*tree)->rchild), data, taller))
				return 0;
			if((*taller)){
				switch((*tree)->bf){
				case LH:
					(*tree)->bf = EH;
					(*taller) = 0;
					break;
				case EH:
					(*tree)->bf = RH;
					(*taller) =1;
					break;
				case RH:
					nsf_avltree_rightbalance(tree);
					break;
				}
			}
		}
	}
	return 1;
}

void nsf_avltree_leftbalance(AVLTree *tree)
{
	AVLNode *lc =  (*tree)->lchild;
	AVLNode *rd;
	switch(lc->bf){
	case LH:
		lc->bf = EH;
		(*tree)->bf = lc->bf;
		nsf_avltree_rrotate(tree);
		break;
	case RH:
		rd = lc->rchild;
		switch(rd->bf){
		case LH:
			(*tree)->bf = RH;
			lc->bf = EH;
			break;
		case EH:
			lc->bf = EH;
			(*tree)->bf = lc->bf;
			break;
		case RH:
			(*tree)->bf =EH;
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
	
	switch(rc->bf){
	case RH:
		rc->bf = EH;
		(*tree)->bf = rc->bf;
		nsf_avltree_lrotate(tree);
		break;
	case LH:
		rl = rc->lchild;
		switch(rl->bf){
		case LH:
			(*tree)->bf=EH;
			rc->bf = RH;
			break;
		case EH:
			rc->bf = EH;
			(*tree)->bf = rc->bf;
			break;
		case RH:
			(*tree)->bf=LH;
			rc->bf = EH;
			break;
		}
		rl->bf = EH;
		nsf_avltree_rrotate(&((*tree)->rchild));
		nsf_avltree_lrotate(tree);
		break;
	}
}
int times;
NsfntAvldata *nsf_avltree_find(AVLTree root, NsfntAvldata key)
{
	times++;
    if (root == NULL)   
    {
        return NULL;  
    } 
      
    if (root->data == key)   
    {  
    	printf("121312\n");
        return &(root->data);  
    }  
    else if (key < root->data)   
    {  
        return nsf_avltree_find(root->lchild, key);  
    }   
    else   
    {  
        return nsf_avltree_find(root->rchild, key);  
    }  
}

int main()
{
	int i;
	AVLTree tree = NULL;
	int taller = 0;
	NsfntAvldata dt;
	NsfntAvldata *ret;
	
	for(i = 0; i < 1000; i++){
		nsf_avltree_insert(&tree, i, &taller);
	}
	printf("可以开始查找了\n");
	while(1)
	{
		times = 0;
		scanf("%d", &dt);
		ret = nsf_avltree_find(tree, dt);
		if(ret == NULL)
			printf("未找到\n");
		else
			printf("找到了%d\n", *ret);
		printf("查找次数%d\n", times);
	}
}	
