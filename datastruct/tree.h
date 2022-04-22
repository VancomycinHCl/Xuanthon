#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dynarray.h"
typedef struct Tree tree;
typedef struct TreeNode treeBody;

/**
 * @file bintree.h
 * @brief The defination of a binary tree for the Abstract Syntax Tree
 * @details No detailed information
 * @author Xuanzhi Liu
 * @version alpha 0.0.1
 * @date Apr 9th, 2022
 * @copyright Xuanzhi Liu
 */

/**
* @brief the structure of a tree Object's node, that contains the actual elements
*/
struct TreeNode
{
	void* contains;				///<mount point of the element
	treeBody* left;				///<the left node of a binary tree
	treeBody* right;			///<the right node of a bin tree 
};

//按照我个人的理解，换成动态数组就可以做到节点任意度数了

/**
* @brief the structure of a tree Object
*/
struct Tree
{
	treeBody* head;				///<mount point of the root in one binary tree 
};

tree* structTreeHead_alloc();									//初始化树元素头
treeBody* structTreeNode_alloc();								//初始化节点
treeBody* structTreeNode_initContain_alloc(void* elementIn);	//初始化节点并分配元素
void ModifyNodeElement(treeBody* targetNode, void* elementIn);	//修改节点内包含元素
void clearTreeNode_free(treeBody* targetNode);					//清除某个节点下的子树
void destructTree_free(tree* targetTree);						//析构一个树
int isLeafNode(treeBody* targetNode);							//是否是叶子结点
int getTreeDepth(tree* targetTree);								//获取树的深度
void traversePreOrder_node(treeBody* targetNode);						//前序遍历，但是接受的是节点
void traverseInOrder_node(treeBody* targetNode);						//中序遍历，但是接受的是节点
void traversePostOrder_node(treeBody* targetNode);						//后序遍历，但是接受的是节点
void traverseLeverOrder_node(treeBody* targetNode);						//层序遍历，但是接受的是节点
listhead* traversePreOrder_node__alloc(treeBody* targetNode); 			//返回链表的前序遍历，但是接受的是节点
listhead* traverseInOrder_node_alloc(treeBody* targetNode);				//返回链表的中序遍历，但是接受的是节点
listhead* traversePostOrder_node_alloc(treeBody* targetNode);			//返回链表的后序遍历，但是接受的是节点
listhead* traverseLeverOrder_node_alloc(treeBody* targetNode);			//返回链表的层序遍历，但是接受的是节点