#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stack.h"


filoarray* StructStack_alloc(const int MAXSIZE)
{
	filoarray* returnArray = malloc(sizeof(filoarray));
	returnArray->fifoArray = malloc(sizeof(void*) * MAXSIZE);
	returnArray->top = -1;
	returnArray->size = MAXSIZE;
	return returnArray;
}
filoarray* DeStructStack_free(filoarray* targetArray)
{
	free(targetArray->fifoArray);
	targetArray->fifoArray = NULL;
	free(targetArray);
	return NULL;
}

int CheckArrayState(filoarray* targetArry)
{
	if (targetArry->top == -1)
	{
		return STACK_UNDER_FLOW;
	}
	else if(targetArry->top == targetArry->size-1)
	{
		return STACK_OVER_FLOW;
	}
	else
	{
		return 0;
	}
}

void* pushElement(filoarray* targetArray, void* targetElementIn)
{
	int status = CheckArrayState(targetArray);
	if (status == STACK_OVER_FLOW)
	{
		return STACK_OVER_FLOW;
	}
	else
	{
		targetArray->top++;															/*ջ��++*/
		(int*)targetArray->fifoArray[targetArray->top] = (int*)targetElementIn;		/*����ջ*/
		return targetArray;
	}
}

void* popElement(filoarray* targetArray)
{
	int status = CheckArrayState(targetArray);
	if (status == STACK_UNDER_FLOW)
	{
		return STACK_UNDER_FLOW;
	}
	else
	{
		(int*)targetArray->fifoArray[targetArray->top] = NULL;						/*����ջ*/
		targetArray->top--;															/*ջ��--*/
		return targetArray;
	}
}

void* getLastElement(filoarray* targetArray)										/*�鿴ջ��*/
{
	int status = CheckArrayState(targetArray);
	if (status == STACK_UNDER_FLOW)
	{
		return STACK_UNDER_FLOW;
	}
	else
	{
		return targetArray->fifoArray[targetArray->top];
	}
}

void ClearStack(filoarray* targetArray)												/*���ջ�����ڻ�����void**��������Ҫ����ָ�룬������ǿ�ƹ���*/
{
	while (targetArray->top>=0)
	{
		targetArray->fifoArray[targetArray->top] = NULL;
		targetArray->top--;
	}
}