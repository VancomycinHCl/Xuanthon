#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dynarray.h"



listbody* InsertNode_alloc(listhead* targetList,const int position) /*����ڵ�*/
{
	listbody* returnNode = NULL;									/*����ֵ*/
	listbody* shadowNode = NULL;									/*��ǰ�ڵ�*/
	listbody* lastNode = NULL;										/*�ϸ��ڵ�*/
	int count = 1;													/*����*/

	if (position != 1 && targetList->start == NULL)					/*���ձ���λ�÷�1*/
	{
		return (int)(-1);											/*����-1����ֵ*/
	}
	else if (position == 1 && targetList->start==NULL)				/*���ձ�*/
	{
		returnNode = malloc(sizeof(listbody));						/*ʵ�����ڵ�*/
		returnNode->next = NULL;									/*ɱ��Ұָ��*/
		returnNode->contains = NULL;
		returnNode->callTime = 0;									/*���ô�����������*/
		targetList->start = returnNode;								/*��ͷָ�������ʵ������Ԫ��*/
		targetList->length++;
		return returnNode;											/*���ؽڵ��ַ*/
	}
	else if (position == 0)
	{
		return 0;
	}
	else															/*�ǿձ�*/
	{
		shadowNode = targetList->start;
		while (shadowNode->next && count < position)				/*�������ڱ��ڲ���С��Ԥ��λ�������*/
		{
			count++;
			lastNode = shadowNode;
			shadowNode = shadowNode->next;
		}
		if (count<position)											/*�������������Ǽ���С��Ԥ��λ�ã����ڱ���*/
		{
			return (int)-1;											/*���ش���ֵ-1*/
		}
		else if(position == 1)										/*�ڱ��ڣ������ǵ�һ��*/
		{
			returnNode = malloc(sizeof(listbody));					/*����ڵ�*/
			returnNode->next = shadowNode;							/*�½ڵ���һ������Դ��һ��*/
			returnNode->contains = NULL;							/*���Ұָ��*/
			returnNode->callTime = 0;
			targetList->start = returnNode;							/*�޸ı�ͷ��ʼ�ڵ㣬ָ���½ڵ�*/
			targetList->length++;									/*����++*/
			return returnNode;										/*���ؽڵ��ַ*/
		}
		else														/*λ���ڱ���*/
		{
			returnNode = malloc(sizeof(listbody));					/*ʵ�����ڵ㲢��ϴ���ڴ�*/
			returnNode->next = NULL;
			returnNode->contains = NULL;
			returnNode->callTime = 0;								/*���ô�������*/
			lastNode->next = returnNode;							/*�ϸ��ڵ����һ��ָ���½��Ľڵ�*/
			returnNode->next = shadowNode;							/*���½��ڵ�ָ��ԭ�ȵ���һ���ڵ�*/
			targetList->length++;
			return returnNode;										/*���ؽڵ��ַ*/
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
	if (shadowNode)//���ڵ����ʱ
	{
		while (shadowNode->next)//shadowNode�����������ҵ���βֹͣ
		{
			lastNode = shadowNode;//����������һ���ڵ�ĵ�ַ
			shadowNode = shadowNode->next;
		}
		if (shadowNode == targetList->start)//����һ���ڵ������ֹ�ڵ�
		{
			targetList->start = NULL;
			shadowNode->contains = NULL;//��ͷ�趨ΪNULL
			free(shadowNode);
			targetList->length--;
			return NULL;//����NULL
		}
		else if(shadowNode != targetList->start)//����һ���ڵ㲻�����һ������shadowNode�ǵ�һ��last�ǵ���
		{
			lastNode->next = NULL;
			shadowNode->contains = NULL;//�����ڶ�������һ��ָ����NULL
			free(shadowNode);//������һ�ڵ�
			targetList->length--;
			return lastNode;//���ص����ڶ����ĵ�ַ
		}
	}
	else/*�����*/
	{
		return (int)-1;/*����-1*/
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

listhead* StructList_alloc(uint8_t listLength)			/*����Ĺ��캯��������ڵ���Ŀ��������Զ�����*/
{
	listhead* returnList = NULL;						/*����һ��ָ�����*/
	returnList = malloc(sizeof(listhead));				/*�����ͷ*/
	returnList->start = NULL;
	returnList->length = 0;
	for (int i = 0; i < listLength; i++){				/*�����������*/
		AddNode_alloc(returnList);
	}
	return returnList;									/*��õı�*/
}

listhead* DestructList_free(listhead* targetList)		/*�������������*/
{
	if (targetList->length){							/*��length>0ʱ�����ǿձ�*/
		for (int i = targetList->length; i > 0; i--)
		{
			DeleteNode_free(targetList, i);				/*�����ڵ�����*/
		}
		targetList->start = NULL;
		free(targetList);								/*������ͷ*/
		return NULL;
	}
	else if (targetList->start == NULL){				/*��length==0�����ҿ�ʼ�ڵ�Ϊ�գ������*/
		free(targetList);
		return NULL;									/*������ͷ*/
	}
	else{
		return (int)-1;									/*���ش���ֵ-1*/
	}
	
}

listbody* IterList(listhead* targetList,const int position)		/*������������λ�÷��ض�Ӧλ��ָ��*/
{
	int count = 1;												/*����ָ��*/
	listbody* shadowNode = targetList->start;					/*��ǰ�ڵ�*/
	if (targetList->start && position>0)							/*�б�ǿղ�ָ��Ϊ����*/
	{
		while (shadowNode->next && count<position)				/*�����б��ڲ�С�ڸ���λ��*/
		{
			shadowNode = shadowNode->next;						/*����*/
			count++;
		}
		if (count<position)										/*���������Ƿ��ڶ�Ӧλ��*/
		{
			return (int)-1;										/*���٣��ͷ��ش���ֵ-1����λ���ڱ���*/
		}
		else
		{
			return shadowNode;									/*�ڣ��ͷ���Ŀ��λ��ָ��*/
		}
	}
	else
	{
		return NULL;											/*�����������NULL*/
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

listbody* ModifyElement(listhead* targetList, const int position, void* targetElement) /*�༭����λ�õ�Ԫ��*/
{
	listbody* shadowNode = IterList(targetList, position);		/*��������ص�Ѱ�ҽڵ��ַ*/
	if (shadowNode != NULL && (int) shadowNode != -1)			/*���ڵ㲻�Ǵ�����ʱ*/
	{
		shadowNode->contains = (int*)targetElement;		/*����ָ�루targetElement��ǿ��ת��Ϊint*��Ȼ��ֵ����Ӧ�ڵ���   */
		shadowNode->callTime++;									/*���ʴ���+1*/
		return shadowNode;										/*���������ַ*/
	}
	else
	{
		return shadowNode;										/*���ش���ֵ*/
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
