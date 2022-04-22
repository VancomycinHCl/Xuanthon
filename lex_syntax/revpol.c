#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stack.h"
#include "revpol.h"
#include <string.h>
#include <math.h>
#include "dynarray.h"
#include "calMem.h"
#pragma warning(disable:4996)

/**
 * @file revpol.c
 * @brief The core execution for the calculator
 * @details None
 * @author Xuanzhi Liu
 * @version alpha 0.0.1
 * @date Apr 9th, 2022
 * @copyright Xuanzhi Liu
 */


 /**
  * @brief Define the priority of operators
  * @param A char
  * @return int
  * @retval priority in numbers, default is the lowest, 0.
  */

uint8_t SigPower(char* elementIn)
{
	switch (*elementIn)
	{
		case('+'):
		case('-'):
			return 1;
			break;
		case('*'):
		case('/'):
		case('%'):
			return 2;
			break;
		case('^'):
			return 3;
			break;
		default:
			return 0;
	}
}

void printStack(filoarray* stack1)
{
	int count = 0;
	while (count<=stack1->top)
	{
		printf("The element %d in the stack is:%s\n", count+1,stack1->fifoArray[count]);
		count++;
	}
}

char* printStack_char_alloc(filoarray* stack1)
{
	int count = 0;
	char* returnChar = (char*) malloc(stack1->top*sizeof(char)+2);
	while (count <= stack1->top)
	{
		printf("%s", stack1->fifoArray[count]);
		returnChar[count] = *(char*) (stack1->fifoArray[count]);
		count++;
	}
	printf("\n");
	returnChar[stack1->top * sizeof(char)+1] = 0;
	printf("%s\n",returnChar);
	return returnChar;/*不行的*/
}

/*栈->指针数组->指针1            指针2
				|->，字符串1     |->   字符串2*/

void ZeroArray(char* arrayIn, int length)
{
	int count = 0;
	while (count<length)
	{
		arrayIn[count] = 0;
		count++;
	}

}


/**
 * @brief ReversePolandExp_alloc(char* expressionIn) A simple function that accept the string Expression to return a void ptr array, which contains the string elements in RPN order
 * @param A string, such as 1+1-a*b/(c^d)
 * @return Return the stack object(hand-made lib defined), which contains the char ptr array and the length of experssion
 * @retval A struct object
 */

filoarray* ReversePolandExp_alloc(char* ExpressionIn)
{
	filoarray* stack1 = StructStack_alloc(0x20);/*初始化栈*/
	filoarray* stack2 = StructStack_alloc(0x20);
	char numberBuffer[0x20] = { 0 };
	char* currentChar = ExpressionIn;
	printf("Current Char is:%s\n", currentChar);
	char baseSymbol[] = {'#',0};
	uint8_t bufferPtr = 0;

	pushElement(stack2, &baseSymbol);

	printStack(stack2);


	while (*currentChar != 0)														/* 1+1*4+(5/1)-4+aaa => 114*+51/+4-aaa+ ，没遇上结束符*/
	{
		bufferPtr = 0;
		//numberBuffer[32] = { 0 };
		printf("We have processed to: %c\n", *currentChar);
		if (*currentChar >= '0' && *currentChar <= '9' || *currentChar >= 'a' && *currentChar <= 'z' || *currentChar >= 'A' && *currentChar <= 'Z')/*当当前的字符不是算符，才能进行下面的拷贝操作*/
		{
			while (*currentChar >= '0' && *currentChar <= '9' || *currentChar >= 'a' && *currentChar <= 'z' || *currentChar >= 'A' && *currentChar <= 'Z')/*先遍历直到算符*/
			{
				numberBuffer[bufferPtr] = *currentChar;
				bufferPtr++;
				currentChar++;
			}
			printf("We have processed to: %c\n", *currentChar);
			if (CheckArrayState(stack1) != STACK_OVER_FLOW)							/*未溢出*/
			{
				stack1->top++;														/*栈顶涨*/
				stack1->fifoArray[stack1->top] = malloc(strlen(numberBuffer) + 1);	/*栈的钩子分配空间*/
				strcpy(stack1->fifoArray[stack1->top], numberBuffer);				/*把buffer内的字符赋值进新开辟的栈空间*/
				ZeroArray(numberBuffer, 32);
			}
			else
			{
				DeStructStack_free(stack1);											/*上溢，析构，退出*/
				DeStructStack_free(stack2);
				exit(STACK_OVER_FLOW);
			}
		}
		if(*currentChar == '(')														/*检测到括号，入栈2*/
		{
			printf("The '(' met.\n");
			printStack(stack1);
			printStack(stack2);
			pushElement(stack2, currentChar);
			printStack(stack2);
		}
		else if (*currentChar == ')')												/*检测到括号结束*/
		{
			if (getLastElement(stack1) == STACK_OVER_FLOW || getLastElement(stack1) == STACK_UNDER_FLOW)
			{
				DeStructStack_free(stack1);
				DeStructStack_free(stack2);
				exit(-1);															/*先看看有没有溢出*/
			}
			while (*(char*) getLastElement(stack2) != '(')							/*开始把stack2的栈内算符推到stack1中,直到遇见'('*/
			{
				printStack(stack1);
				stack1->top++;
				stack1->fifoArray[stack1->top] = malloc(2);
				*(char*)stack1->fifoArray[stack1->top] = *(char*)getLastElement(stack2);/*弾栈，但是弾栈有bug，这里用的取栈顶*/
				*((char*)stack1->fifoArray[stack1->top] + 1) = 0;
				popElement(stack2);
			}
			popElement(stack2);														 /*把'('符号一并出栈*/
		}
		else if (*currentChar == '+' || *currentChar == '-' || *currentChar == '*' || *currentChar == '/' || *currentChar == '%' || *currentChar == '^')
		{
			while (SigPower(currentChar) <= SigPower((char*)getLastElement(stack2)))/*当前算符优先级等于或小于栈顶算符优先级*/
			{
				stack1->top++;														 /*把当前stack2栈顶算符弹出，直到遇到比栈顶大的停止*/
				stack1->fifoArray[stack1->top] = malloc(sizeof(char)*2);
				printf("%c\n",*(char*)getLastElement(stack2));
				*(char*)stack1->fifoArray[stack1->top] =  *(char*)getLastElement(stack2) ;
				*((char*)stack1->fifoArray[stack1->top]+1) = 0;
				popElement(stack2);
			}
			pushElement(stack2, currentChar);										 /*将当前算符推入栈中*/
			//printStack(stack2);
			//printStack(stack1);
		}
		if (*currentChar == 0)
		{
			break;
		}
		currentChar++;
	}
	while (*(char*)getLastElement(stack2)!='#')										/*当遇到结束符时，把栈内所有算符弹出，只剩初始化的#算符*/
	{
		stack1->top++;																/*stack2栈顶算符弹出*/
		stack1->fifoArray[stack1->top] = malloc(2);
		*(char*)stack1->fifoArray[stack1->top] = *(char*)getLastElement(stack2);
		*((char*)stack1->fifoArray[stack1->top] + 1) = 0;
		popElement(stack2);
	}

	DeStructStack_free(stack2);														/*析构stack2工具栈*/
	return stack1;
}


/**
 * @brief Give a integer result to the expression by utilizing the extra stack object
 * @param The RPN stack object
 * @return integer result
 * @retval int
 */

int calculateStack(filoarray* stack1,listhead* varListIn)/*简单作业用，只支持整数运算，不支持变量计算与浮点数计算*/
{
	/**
	* @brief filoarray* calculateStack Newing a stack object for calculation, the default length is 0x20
	*/
	filoarray* calculateStack = StructStack_alloc(0x20);
	
	/**
	* @brief int copyOrigTop One redundant pointer of the inputted stack's contained array, as the indecator of the pushing the char ptr to the calculate stack 
	*/
	int copyOrigTop = 0;  
	/*复制一个原始栈的栈顶进来*/

	/**
	* @brief int calBuffer[3] The buffer for calculation, 1st and 2nd place for the operand, and the last for the result
	*/
	int calBuffer[3] = { 0 };        /*加数被加数缓存*/
	
	///char tempRestChar_alloc[100] A char buffer, incharge of receive the converted result, and send the string-integer value to the dynamic array
	char tempRestChar_alloc[100] = {0};						/*字符串指针，接收计算结果转成的字符串后的malloc指针*/
	char* convertedVarPtr = NULL;
	/**
	* @brief listhead* tempResultList_alloc Structure a dynamic array for carrying the strncpyed char from the char buffer, and link its body's contained string (The estimated result) for the calculate stack
	*/
	listhead* tempResultList_alloc = NULL;

	/**
	* @brief char* tmpTr Contains the temproary base address of the string in last element of the dynamic array
	*/
	char* tmpTr = NULL;								/*存放操作GetLastElement_List(tempResultList_alloc)->contains产生的基址指针，用来给堆中数组封0*/
	tempResultList_alloc = StructList_alloc(0x00);	/*给定一个动态数组，存放算好的数*/


	//int tempRest = 0;
	while (copyOrigTop<=stack1->top && stack1->top != -1)/*当原始栈没有下溢出并且复制的栈顶指针小于原始指针*/
	{
		pushElement(calculateStack, stack1->fifoArray[copyOrigTop]);
		printf("入了栈的东西：%c\n",*(char*)getLastElement(calculateStack));
		printStack(calculateStack);
		if (*(char*) getLastElement(calculateStack) == '+' || *(char*)getLastElement(calculateStack) == '-' ||
			*(char*) getLastElement(calculateStack) == '*' || *(char*)getLastElement(calculateStack) == '/' ||
			*(char*) getLastElement(calculateStack) == '%' || *(char*)getLastElement(calculateStack) == '^'
			)
		{
			switch (*(char*)getLastElement(calculateStack))
			{
				case '+':
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn,(char*)getLastElement(calculateStack));
					//calBuffer[0] = atoi((char*)getLastElement(calculateStack));/*计算用栈顶出栈1个,并且转换成整数*/
					calBuffer[0] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[1] = atoi((char*)getLastElement(calculateStack));
					calBuffer[1] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					calBuffer[2] = calBuffer[1] + calBuffer[0];
					itoa(calBuffer[2], tempRestChar_alloc,10);			/*转换回字符串指针*/
					//printf("%s\n", tempRestChar_alloc);
					AddNode_alloc(tempResultList_alloc);
					(GetLastElement_List(tempResultList_alloc))->contains = malloc(strnlen(tempRestChar_alloc,100)+1);/*新建存放计算结果的动态数组节点分配空间*/
					strncpy(GetLastElement_List(tempResultList_alloc)->contains, tempRestChar_alloc, strnlen(tempRestChar_alloc, 100));
					/*新建节点内空间拷贝原有字符串进新空间*/
					tmpTr = GetLastElement_List(tempResultList_alloc)->contains;
					tmpTr[strnlen(tempRestChar_alloc, 100)] = 0;
					/*尾段封口*/

					//printf("%s\n", GetLastElement_List(tempResultList_alloc)->contains);
					//pushElement(calculateStack, tempRestChar_alloc);

					pushElement(calculateStack, (GetLastElement_List(tempResultList_alloc))->contains);	/*把字符串从动态数组压进栈里*/
					//printStack(calculateStack);
					break;
				case '-':
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[0] = atoi((char*)getLastElement(calculateStack));/*计算用栈顶出栈1个,并且转换成整数*/
					calBuffer[0] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[1] = atoi((char*)getLastElement(calculateStack));
					calBuffer[1] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					calBuffer[2] = calBuffer[1] - calBuffer[0];
					itoa(calBuffer[2], tempRestChar_alloc, 10);			/*转换回字符串指针*/
					AddNode_alloc(tempResultList_alloc);
					(GetLastElement_List(tempResultList_alloc))->contains = malloc(strnlen(tempRestChar_alloc, 100) + 1);/*新建存放计算结果的动态数组节点分配空间*/
					strncpy(GetLastElement_List(tempResultList_alloc)->contains, tempRestChar_alloc, strnlen(tempRestChar_alloc, 100));
					/*新建节点内空间拷贝原有字符串进新空间*/
					tmpTr = GetLastElement_List(tempResultList_alloc)->contains;
					tmpTr[strnlen(tempRestChar_alloc, 100)] = 0;
					/*尾段封口*/

					//printf("%s\n", GetLastElement_List(tempResultList_alloc)->contains);
					//pushElement(calculateStack, tempRestChar_alloc);

					pushElement(calculateStack, (GetLastElement_List(tempResultList_alloc))->contains);	/*把字符串从动态数组压进栈里*/
					//printStack(calculateStack);
					break;
				case '*':
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[0] = atoi((char*)getLastElement(calculateStack));/*计算用栈顶出栈1个,并且转换成整数*/
					calBuffer[0] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[1] = atoi((char*)getLastElement(calculateStack));
					calBuffer[1] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					calBuffer[2] = calBuffer[1] * calBuffer[0];
					itoa(calBuffer[2], tempRestChar_alloc, 10);			/*转换回字符串指针*/
					AddNode_alloc(tempResultList_alloc);
					(GetLastElement_List(tempResultList_alloc))->contains = malloc(strnlen(tempRestChar_alloc, 100) + 1);/*新建存放计算结果的动态数组节点分配空间*/
					strncpy(GetLastElement_List(tempResultList_alloc)->contains, tempRestChar_alloc, strnlen(tempRestChar_alloc, 100));
					/*新建节点内空间拷贝原有字符串进新空间*/
					tmpTr = GetLastElement_List(tempResultList_alloc)->contains;
					tmpTr[strnlen(tempRestChar_alloc, 100)] = 0;
					/*尾段封口*/

					//printf("%s\n", GetLastElement_List(tempResultList_alloc)->contains);
					//pushElement(calculateStack, tempRestChar_alloc);

					pushElement(calculateStack, (GetLastElement_List(tempResultList_alloc))->contains);	/*把字符串从动态数组压进栈里*/
					//printStack(calculateStack);
					break;
				case '/':
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[0] = atoi((char*)getLastElement(calculateStack));/*计算用栈顶出栈1个,并且转换成整数*/
					calBuffer[0] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[1] = atoi((char*)getLastElement(calculateStack));
					calBuffer[1] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					if (calBuffer[0]!=0)
					{
						calBuffer[2] = calBuffer[1] / calBuffer[0];
					}
					else
					{
						calBuffer[2] = calBuffer[1] / 1;
						printf("ZeroDevisionError");
					}
					itoa(calBuffer[2], tempRestChar_alloc, 10);			/*转换回字符串指针*/
					AddNode_alloc(tempResultList_alloc);
					(GetLastElement_List(tempResultList_alloc))->contains = malloc(strnlen(tempRestChar_alloc, 100) + 1);/*新建存放计算结果的动态数组节点分配空间*/
					strncpy(GetLastElement_List(tempResultList_alloc)->contains, tempRestChar_alloc, strnlen(tempRestChar_alloc, 100));
					/*新建节点内空间拷贝原有字符串进新空间*/
					tmpTr = GetLastElement_List(tempResultList_alloc)->contains;
					tmpTr[strnlen(tempRestChar_alloc, 100)] = 0;
					/*尾段封口*/

					//printf("%s\n", GetLastElement_List(tempResultList_alloc)->contains);
					//pushElement(calculateStack, tempRestChar_alloc);

					pushElement(calculateStack, (GetLastElement_List(tempResultList_alloc))->contains);	/*把字符串从动态数组压进栈里*/
					//printStack(calculateStack);
					break;
				case '%':
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[0] = atoi((char*)getLastElement(calculateStack));/*计算用栈顶出栈1个,并且转换成整数*/
					calBuffer[0] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[1] = atoi((char*)getLastElement(calculateStack));
					calBuffer[1] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					if (calBuffer[0] != 0)
					{
						calBuffer[2] = calBuffer[1] / calBuffer[0];
					}
					else
					{
						calBuffer[2] = calBuffer[1] / 1;
						printf("ZeroDevisionError");
					}
					itoa(calBuffer[2], tempRestChar_alloc, 10);			/*转换回字符串指针*/
					AddNode_alloc(tempResultList_alloc);
					(GetLastElement_List(tempResultList_alloc))->contains = malloc(strnlen(tempRestChar_alloc, 100) + 1);/*新建存放计算结果的动态数组节点分配空间*/
					strncpy(GetLastElement_List(tempResultList_alloc)->contains, tempRestChar_alloc, strnlen(tempRestChar_alloc, 100));
					/*新建节点内空间拷贝原有字符串进新空间*/
					tmpTr = GetLastElement_List(tempResultList_alloc)->contains;
					tmpTr[strnlen(tempRestChar_alloc, 100)] = 0;
					/*尾段封口*/

					//printf("%s\n", GetLastElement_List(tempResultList_alloc)->contains);
					//pushElement(calculateStack, tempRestChar_alloc);

					pushElement(calculateStack, (GetLastElement_List(tempResultList_alloc))->contains);	/*把字符串从动态数组压进栈里*/
					//printStack(calculateStack);
					break;
				case '^':
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[0] = atoi((char*)getLastElement(calculateStack));/*计算用栈顶出栈1个,并且转换成整数*/
					calBuffer[0] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[1] = atoi((char*)getLastElement(calculateStack));
					calBuffer[1] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					calBuffer[2] = (int) pow(calBuffer[1],calBuffer[0]);
					itoa(calBuffer[2], tempRestChar_alloc, 10);			/*转换回字符串指针*/
					AddNode_alloc(tempResultList_alloc);
					(GetLastElement_List(tempResultList_alloc))->contains = malloc(strnlen(tempRestChar_alloc, 100) + 1);/*新建存放计算结果的动态数组节点分配空间*/
					strncpy(GetLastElement_List(tempResultList_alloc)->contains, tempRestChar_alloc, strnlen(tempRestChar_alloc, 100));
					/*新建节点内空间拷贝原有字符串进新空间*/
					tmpTr = GetLastElement_List(tempResultList_alloc)->contains;
					tmpTr[strnlen(tempRestChar_alloc, 100)] = 0;
					/*尾段封口*/

					//printf("%s\n", GetLastElement_List(tempResultList_alloc)->contains);
					//pushElement(calculateStack, tempRestChar_alloc);

					pushElement(calculateStack, (GetLastElement_List(tempResultList_alloc))->contains);	/*把字符串从动态数组压进栈里*/
					//printStack(calculateStack);
					break;
			}
			copyOrigTop++;/*原始栈的复制栈顶进1*/
		}
		else
		{
			copyOrigTop++;/*原始栈的复制栈顶进1*/
		}
		/*应该是没有什么内存泄漏的问题了*/
	}
	printStack(calculateStack);
	calBuffer[2] = atoi((char*)getLastElement(calculateStack));
	DeStructStack_free(calculateStack);
	DestructList_free(tempResultList_alloc);
	return calBuffer[2];
}

