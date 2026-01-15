#include <stdio.h>
#include <stdlib.h>
typedef struct tree {
	struct tree *left;
	struct tree *right;
}tree;

int leaves(tree *t) {
	if(t == NULL) {return 0;}
	if(t->left == NULL && t->right == NULL)	{return 1;} 
	return leaves(t->left) + leaves(t->right);
}


int main(int argc, char** argv){
	
	tree *root = malloc(sizeof(tree));
	root->left = malloc(sizeof(tree));
	root->right = malloc(sizeof(tree));
	root->left->left = NULL;
	root->right->right = NULL;
	root->left->right = NULL;
	root->right->left = NULL;
	printf("Leaves: %d\n",leaves(root));

}
