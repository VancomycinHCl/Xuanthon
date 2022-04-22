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
		targetArray->top++;															/*栈顶++*/
		(int*)targetArray->fifoArray[targetArray->top] = (int*)targetElementIn;		/*推入栈*/
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
		(int*)targetArray->fifoArray[targetArray->top] = NULL;						/*推入栈*/
		targetArray->top--;															/*栈顶--*/
		return targetArray;
	}
}

void* getLastElement(filoarray* targetArray)										/*查看栈顶*/
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

void ClearStack(filoarray* targetArray)												/*请空栈，由于货仓是void**，所以需要清理指针，把数组强制归零*/
{
	while (targetArray->top>=0)
	{
		targetArray->fifoArray[targetArray->top] = NULL;
		targetArray->top--;
	}
}