#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dynarray.h"



listbody* InsertNode_alloc(listhead* targetList,const int position) /*插入节点*/
{
	listbody* returnNode = NULL;									/*返回值*/
	listbody* shadowNode = NULL;									/*当前节点*/
	listbody* lastNode = NULL;										/*上个节点*/
	int count = 1;													/*计数*/

	if (position != 1 && targetList->start == NULL)					/*当空表但是位置非1*/
	{
		return (int)(-1);											/*返回-1错误值*/
	}
	else if (position == 1 && targetList->start==NULL)				/*当空表*/
	{
		returnNode = malloc(sizeof(listbody));						/*实例化节点*/
		returnNode->next = NULL;									/*杀掉野指针*/
		returnNode->contains = NULL;
		returnNode->callTime = 0;									/*调用次数计数置零*/
		targetList->start = returnNode;								/*表头指向这个被实例化的元素*/
		targetList->length++;
		return returnNode;											/*返回节点地址*/
	}
	else if (position == 0)
	{
		return 0;
	}
	else															/*非空表*/
	{
		shadowNode = targetList->start;
		while (shadowNode->next && count < position)				/*遍历，在表内并且小于预定位置则继续*/
		{
			count++;
			lastNode = shadowNode;
			shadowNode = shadowNode->next;
		}
		if (count<position)											/*遍历结束，但是计数小于预定位置，即在表外*/
		{
			return (int)-1;											/*返回错误值-1*/
		}
		else if(position == 1)										/*在表内，但是是第一个*/
		{
			returnNode = malloc(sizeof(listbody));					/*构造节点*/
			returnNode->next = shadowNode;							/*新节点下一个等于源第一个*/
			returnNode->contains = NULL;							/*清除野指针*/
			returnNode->callTime = 0;
			targetList->start = returnNode;							/*修改表头起始节点，指向新节点*/
			targetList->length++;									/*长度++*/
			return returnNode;										/*返回节点地址*/
		}
		else														/*位置在表内*/
		{
			returnNode = malloc(sizeof(listbody));					/*实例化节点并清洗脏内存*/
			returnNode->next = NULL;
			returnNode->contains = NULL;
			returnNode->callTime = 0;								/*调用次数置零*/
			lastNode->next = returnNode;							/*上个节点的下一个指向新建的节点*/
			returnNode->next = shadowNode;							/*本新建节点指向原先的下一个节点*/
			targetList->length++;
			return returnNode;										/*返回节点地址*/
		}

	}
}

listbody* AddNode_alloc(listhead* targetList)								/*Add a node*/
{
	listbody* returnNode = NULL;
	listbody* shadowNode = NULL;

	returnNode = (listbody*)malloc(sizeof(listbody));
	if (returnNode)
	{
		returnNode->callTime = 0;
		returnNode->contains = NULL;
		returnNode->next = NULL;
	}

	shadowNode = targetList->start;

	if (shadowNode == NULL)
	{
		targetList->start = returnNode;
		targetList->length++;
		return returnNode;
	}
	else
	{
		//printf("Start Add Node:\n");
		for (shadowNode; shadowNode->next != NULL; shadowNode = shadowNode->next)/*only for iter the list and print the list in terms of debugging*/
		{
			#ifdef _DEBUG 
			//printf("%p->%p\n",shadowNode, shadowNode->next);
			#endif // 0
		}
		shadowNode->next = returnNode;
		//printf("%p->%p\n", shadowNode->next, returnNode->next);
	}
	targetList->length++;
	return returnNode;
}

listbody* PopNode_free(listhead* targetList)
{
	listbody* shadowNode = targetList->start;
	listbody* lastNode = NULL;
	if (shadowNode)//当节点存在时
	{
		while (shadowNode->next)//shadowNode遍历整个表并且到结尾停止
		{
			lastNode = shadowNode;//并且留下上一个节点的地址
			shadowNode = shadowNode->next;
		}
		if (shadowNode == targetList->start)//当第一个节点就是终止节点
		{
			targetList->start = NULL;
			shadowNode->contains = NULL;//表头设定为NULL
			free(shadowNode);
			targetList->length--;
			return NULL;//返回NULL
		}
		else if(shadowNode != targetList->start)//当第一个节点不是最后一个，则shadowNode是倒一，last是倒二
		{
			lastNode->next = NULL;
			shadowNode->contains = NULL;//倒数第二个的下一跳指针置NULL
			free(shadowNode);//析构倒一节点
			targetList->length--;
			return lastNode;//返回倒数第二个的地址
		}
	}
	else/*本表空*/
	{
		return (int)-1;/*返回-1*/
	}
}

listbody* DeleteNode_free(listhead* targrtList, const int position)
{
	listbody* shadowNode;
	listbody* lastNode = NULL;
	shadowNode = targrtList->start;
	int counter = 1;


	while (shadowNode->next != NULL && counter < position)
	{
		lastNode = shadowNode;
		shadowNode = shadowNode->next;
		counter++;
	}

	if (counter < position)
	{
		return (int)-1;
	}
	else if (lastNode && position != 1)
	{
		lastNode->next = shadowNode->next;
		shadowNode->next = NULL;
		shadowNode->contains = NULL;
		shadowNode->callTime = 0;
		free(shadowNode);
		targrtList->length--;
		return(lastNode->next);
	}

	else
	{
		targrtList->start = NULL;
		free(shadowNode);
		targrtList->length--;
		return(int)-1;
	}
}

listhead* StructList_alloc(uint8_t listLength)			/*整表的构造函数，输入节点数目即可完成自动构造*/
{
	listhead* returnList = NULL;						/*给定一个指针变量*/
	returnList = malloc(sizeof(listhead));				/*构造表头*/
	returnList->start = NULL;
	returnList->length = 0;
	for (int i = 0; i < listLength; i++){				/*构造表身并链接*/
		AddNode_alloc(returnList);
	}
	return returnList;									/*造好的表*/
}

listhead* DestructList_free(listhead* targetList)		/*整表的析构函数*/
{
	if (targetList->length){							/*当length>0时，即非空表*/
		for (int i = targetList->length; i > 0; i--)
		{
			DeleteNode_free(targetList, i);				/*挨个节点析构*/
		}
		targetList->start = NULL;
		free(targetList);								/*析构表头*/
		return NULL;
	}
	else if (targetList->start == NULL){				/*当length==0，并且开始节点为空，即真空*/
		free(targetList);
		return NULL;									/*析构表头*/
	}
	else{
		return (int)-1;									/*返回错误值-1*/
	}
	
}

listbody* IterList(listhead* targetList,const int position)		/*遍历器，输入位置返回对应位置指针*/
{
	int count = 1;												/*序数指针*/
	listbody* shadowNode = targetList->start;					/*当前节点*/
	if (targetList->start && position>0)							/*列表非空并指针为正数*/
	{
		while (shadowNode->next && count<position)				/*当在列表内并小于给定位置*/
		{
			shadowNode = shadowNode->next;						/*继续*/
			count++;
		}
		if (count<position)										/*结束后检查是否在对应位置*/
		{
			return (int)-1;										/*不再，就返回错误值-1，即位置在表外*/
		}
		else
		{
			return shadowNode;									/*在，就返回目标位置指针*/
		}
	}
	else
	{
		return NULL;											/*其余情况返回NULL*/
	}
}

uint8_t GetLength(listhead* targetList)
{
	int count = 1;
	listbody* shadowNode = targetList->start;
	if (targetList->start)
	{
		while (shadowNode->next)
		{
			count++;
			shadowNode = shadowNode->next;
		}
		return count;
	}
	else
	{
		return 0;
	}
}

listbody* ModifyElement(listhead* targetList, const int position, void* targetElement) /*编辑给定位置的元素*/
{
	listbody* shadowNode = IterList(targetList, position);		/*根据输入地点寻找节点地址*/
	if (shadowNode != NULL && (int) shadowNode != -1)			/*当节点不是错误码时*/
	{
		shadowNode->contains = (int*)targetElement;		/*输入指针（targetElement）强制转换为int*，然后赋值进对应节点内   */
		shadowNode->callTime++;									/*访问次数+1*/
		return shadowNode;										/*返回这个地址*/
	}
	else
	{
		return shadowNode;										/*返回错误值*/
	}

}

listbody* GetLastElement_List(listhead* targetList)
{
	listbody* shadowNode = targetList->start;
	if (targetList->start == NULL)
	{
		return -1;
	}
	else
	{
		while (shadowNode->next)
		{
			shadowNode = shadowNode->next;
		}
		return shadowNode;
	}
}

#ifdef _DEBUG
void PrintList(listhead* targetList)/*This is for iterate the whole list so that to check the node.*/
{
	listbody* shadowNode = targetList->start;
	int counter = 1;
	printf("The address of this dynamic array object is: %p.\n", targetList);
	if (targetList->start)
	{
		printf("The initical address of the array's first element is:%p.\n", targetList->start);
		printf("Length of dynamic array is: %d\n", targetList->length);
		while (shadowNode->next)
		{
			printf("addr of node %d is: %p.\t Next node is:%p.\n", counter, shadowNode, shadowNode->next);
			shadowNode = shadowNode->next;
			counter++;
		}
		printf("addr of node %d is: %p.\t Next node is:%p.\n\n", counter, shadowNode, shadowNode->next);
	}

	else
	{
		printf("Length of dynamic array is: %d\n", targetList->length);
		printf("There is No node in the list\n\n");
	}

}
#endif
