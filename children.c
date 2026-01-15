#include <stdio.h>
#include <stdlib.h>
typedef struct tree {
	struct tree *left;
	struct tree *right;
}tree;

int children(tree *t) {
	if(t == NULL) {return 0;}
	int leftchild = 0, rightchild=0;
	if(t->left != NULL){ leftchild = 1+ children(t->left);}
	if(t->right != NULL){rightchild = 1 +children(t->right);}
	int max = (leftchild > rightchild)? leftchild : rightchild;
	return max;
}


int main(int argc, char** argv){
	tree *root = malloc(sizeof(tree));
    	tree *A    = malloc(sizeof(tree));
   	 tree *B    = malloc(sizeof(tree));
    	tree *C    = malloc(sizeof(tree));
    	tree *D    = malloc(sizeof(tree));
    	tree *E    = malloc(sizeof(tree));
    	tree *F    = malloc(sizeof(tree));
	root->left = A;
   	root->right = B;
    A->left = C;
    A->right = NULL;

    C->left = NULL;
    C->right = NULL;

    B->left = D;
    B->right = E;

    E->left = NULL;
    E->right = NULL;

    D->left = F;
    D->right = NULL;

    F->left = NULL;
    F->right = NULL;

    printf("Max chain (edges): %d\n", children(root));  

    return 0;
}
