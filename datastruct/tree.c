#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dynarray.h"
#include "tree.h"

tree* structTreeHead_alloc()
{
	tree* returnTree = malloc(sizeof(treeBody));
	return returnTree;
}
treeBody* structTreeNode_alloc()
{
	treeBody* returnNode = malloc(sizeof(treeBody));
	returnNode->contains = NULL;
	returnNode->left = NULL;
	returnNode->right = NULL;
	return returnNode;
}
void traversePreOrder_node(treeBody* targetNode)
{
	if (targetNode)
	{
		printf("%s\n", (char*) targetNode->contains);
		traversePreOrder_node(targetNode->left);
		traversePreOrder_node(targetNode->right);
	}
}

void traverseInOrder_node(treeBody* targetNode)
{
	if (targetNode)
	{
		traversePreOrder_node(targetNode->left);
		printf("%s\n", (char*)targetNode->contains);
		traversePreOrder_node(targetNode->right);
	}
}