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

//�����Ҹ��˵���⣬���ɶ�̬����Ϳ��������ڵ����������

/**
* @brief the structure of a tree Object
*/
struct Tree
{
	treeBody* head;				///<mount point of the root in one binary tree 
};

tree* structTreeHead_alloc();									//��ʼ����Ԫ��ͷ
treeBody* structTreeNode_alloc();								//��ʼ���ڵ�
treeBody* structTreeNode_initContain_alloc(void* elementIn);	//��ʼ���ڵ㲢����Ԫ��
void ModifyNodeElement(treeBody* targetNode, void* elementIn);	//�޸Ľڵ��ڰ���Ԫ��
void clearTreeNode_free(treeBody* targetNode);					//���ĳ���ڵ��µ�����
void destructTree_free(tree* targetTree);						//����һ����
int isLeafNode(treeBody* targetNode);							//�Ƿ���Ҷ�ӽ��
int getTreeDepth(tree* targetTree);								//��ȡ�������
void traversePreOrder_node(treeBody* targetNode);						//ǰ����������ǽ��ܵ��ǽڵ�
void traverseInOrder_node(treeBody* targetNode);						//������������ǽ��ܵ��ǽڵ�
void traversePostOrder_node(treeBody* targetNode);						//������������ǽ��ܵ��ǽڵ�
void traverseLeverOrder_node(treeBody* targetNode);						//������������ǽ��ܵ��ǽڵ�
listhead* traversePreOrder_node__alloc(treeBody* targetNode); 			//���������ǰ����������ǽ��ܵ��ǽڵ�
listhead* traverseInOrder_node_alloc(treeBody* targetNode);				//���������������������ǽ��ܵ��ǽڵ�
listhead* traversePostOrder_node_alloc(treeBody* targetNode);			//��������ĺ�����������ǽ��ܵ��ǽڵ�
listhead* traverseLeverOrder_node_alloc(treeBody* targetNode);			//��������Ĳ�����������ǽ��ܵ��ǽڵ�