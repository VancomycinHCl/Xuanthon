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
	return returnChar;/*���е�*/
}

/*ջ->ָ������->ָ��1            ָ��2
				|->���ַ���1     |->   �ַ���2*/

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
	filoarray* stack1 = StructStack_alloc(0x20);/*��ʼ��ջ*/
	filoarray* stack2 = StructStack_alloc(0x20);
	char numberBuffer[0x20] = { 0 };
	char* currentChar = ExpressionIn;
	printf("Current Char is:%s\n", currentChar);
	char baseSymbol[] = {'#',0};
	uint8_t bufferPtr = 0;

	pushElement(stack2, &baseSymbol);

	printStack(stack2);


	while (*currentChar != 0)														/* 1+1*4+(5/1)-4+aaa => 114*+51/+4-aaa+ ��û���Ͻ�����*/
	{
		bufferPtr = 0;
		//numberBuffer[32] = { 0 };
		printf("We have processed to: %c\n", *currentChar);
		if (*currentChar >= '0' && *currentChar <= '9' || *currentChar >= 'a' && *currentChar <= 'z' || *currentChar >= 'A' && *currentChar <= 'Z')/*����ǰ���ַ�������������ܽ�������Ŀ�������*/
		{
			while (*currentChar >= '0' && *currentChar <= '9' || *currentChar >= 'a' && *currentChar <= 'z' || *currentChar >= 'A' && *currentChar <= 'Z')/*�ȱ���ֱ�����*/
			{
				numberBuffer[bufferPtr] = *currentChar;
				bufferPtr++;
				currentChar++;
			}
			printf("We have processed to: %c\n", *currentChar);
			if (CheckArrayState(stack1) != STACK_OVER_FLOW)							/*δ���*/
			{
				stack1->top++;														/*ջ����*/
				stack1->fifoArray[stack1->top] = malloc(strlen(numberBuffer) + 1);	/*ջ�Ĺ��ӷ���ռ�*/
				strcpy(stack1->fifoArray[stack1->top], numberBuffer);				/*��buffer�ڵ��ַ���ֵ���¿��ٵ�ջ�ռ�*/
				ZeroArray(numberBuffer, 32);
			}
			else
			{
				DeStructStack_free(stack1);											/*���磬�������˳�*/
				DeStructStack_free(stack2);
				exit(STACK_OVER_FLOW);
			}
		}
		if(*currentChar == '(')														/*��⵽���ţ���ջ2*/
		{
			printf("The '(' met.\n");
			printStack(stack1);
			printStack(stack2);
			pushElement(stack2, currentChar);
			printStack(stack2);
		}
		else if (*currentChar == ')')												/*��⵽���Ž���*/
		{
			if (getLastElement(stack1) == STACK_OVER_FLOW || getLastElement(stack1) == STACK_UNDER_FLOW)
			{
				DeStructStack_free(stack1);
				DeStructStack_free(stack2);
				exit(-1);															/*�ȿ�����û�����*/
			}
			while (*(char*) getLastElement(stack2) != '(')							/*��ʼ��stack2��ջ������Ƶ�stack1��,ֱ������'('*/
			{
				printStack(stack1);
				stack1->top++;
				stack1->fifoArray[stack1->top] = malloc(2);
				*(char*)stack1->fifoArray[stack1->top] = *(char*)getLastElement(stack2);/*��ջ�����Ǐ�ջ��bug�������õ�ȡջ��*/
				*((char*)stack1->fifoArray[stack1->top] + 1) = 0;
				popElement(stack2);
			}
			popElement(stack2);														 /*��'('����һ����ջ*/
		}
		else if (*currentChar == '+' || *currentChar == '-' || *currentChar == '*' || *currentChar == '/' || *currentChar == '%' || *currentChar == '^')
		{
			while (SigPower(currentChar) <= SigPower((char*)getLastElement(stack2)))/*��ǰ������ȼ����ڻ�С��ջ��������ȼ�*/
			{
				stack1->top++;														 /*�ѵ�ǰstack2ջ�����������ֱ��������ջ�����ֹͣ*/
				stack1->fifoArray[stack1->top] = malloc(sizeof(char)*2);
				printf("%c\n",*(char*)getLastElement(stack2));
				*(char*)stack1->fifoArray[stack1->top] =  *(char*)getLastElement(stack2) ;
				*((char*)stack1->fifoArray[stack1->top]+1) = 0;
				popElement(stack2);
			}
			pushElement(stack2, currentChar);										 /*����ǰ�������ջ��*/
			//printStack(stack2);
			//printStack(stack1);
		}
		if (*currentChar == 0)
		{
			break;
		}
		currentChar++;
	}
	while (*(char*)getLastElement(stack2)!='#')										/*������������ʱ����ջ���������������ֻʣ��ʼ����#���*/
	{
		stack1->top++;																/*stack2ջ���������*/
		stack1->fifoArray[stack1->top] = malloc(2);
		*(char*)stack1->fifoArray[stack1->top] = *(char*)getLastElement(stack2);
		*((char*)stack1->fifoArray[stack1->top] + 1) = 0;
		popElement(stack2);
	}

	DeStructStack_free(stack2);														/*����stack2����ջ*/
	return stack1;
}


/**
 * @brief Give a integer result to the expression by utilizing the extra stack object
 * @param The RPN stack object
 * @return integer result
 * @retval int
 */

int calculateStack(filoarray* stack1,listhead* varListIn)/*����ҵ�ã�ֻ֧���������㣬��֧�ֱ��������븡��������*/
{
	/**
	* @brief filoarray* calculateStack Newing a stack object for calculation, the default length is 0x20
	*/
	filoarray* calculateStack = StructStack_alloc(0x20);
	
	/**
	* @brief int copyOrigTop One redundant pointer of the inputted stack's contained array, as the indecator of the pushing the char ptr to the calculate stack 
	*/
	int copyOrigTop = 0;  
	/*����һ��ԭʼջ��ջ������*/

	/**
	* @brief int calBuffer[3] The buffer for calculation, 1st and 2nd place for the operand, and the last for the result
	*/
	int calBuffer[3] = { 0 };        /*��������������*/
	
	///char tempRestChar_alloc[100] A char buffer, incharge of receive the converted result, and send the string-integer value to the dynamic array
	char tempRestChar_alloc[100] = {0};						/*�ַ���ָ�룬���ռ�����ת�ɵ��ַ������mallocָ��*/
	char* convertedVarPtr = NULL;
	/**
	* @brief listhead* tempResultList_alloc Structure a dynamic array for carrying the strncpyed char from the char buffer, and link its body's contained string (The estimated result) for the calculate stack
	*/
	listhead* tempResultList_alloc = NULL;

	/**
	* @brief char* tmpTr Contains the temproary base address of the string in last element of the dynamic array
	*/
	char* tmpTr = NULL;								/*��Ų���GetLastElement_List(tempResultList_alloc)->contains�����Ļ�ַָ�룬���������������0*/
	tempResultList_alloc = StructList_alloc(0x00);	/*����һ����̬���飬�����õ���*/


	//int tempRest = 0;
	while (copyOrigTop<=stack1->top && stack1->top != -1)/*��ԭʼջû����������Ҹ��Ƶ�ջ��ָ��С��ԭʼָ��*/
	{
		pushElement(calculateStack, stack1->fifoArray[copyOrigTop]);
		printf("����ջ�Ķ�����%c\n",*(char*)getLastElement(calculateStack));
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
					//calBuffer[0] = atoi((char*)getLastElement(calculateStack));/*������ջ����ջ1��,����ת��������*/
					calBuffer[0] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[1] = atoi((char*)getLastElement(calculateStack));
					calBuffer[1] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					calBuffer[2] = calBuffer[1] + calBuffer[0];
					itoa(calBuffer[2], tempRestChar_alloc,10);			/*ת�����ַ���ָ��*/
					//printf("%s\n", tempRestChar_alloc);
					AddNode_alloc(tempResultList_alloc);
					(GetLastElement_List(tempResultList_alloc))->contains = malloc(strnlen(tempRestChar_alloc,100)+1);/*�½���ż������Ķ�̬����ڵ����ռ�*/
					strncpy(GetLastElement_List(tempResultList_alloc)->contains, tempRestChar_alloc, strnlen(tempRestChar_alloc, 100));
					/*�½��ڵ��ڿռ俽��ԭ���ַ������¿ռ�*/
					tmpTr = GetLastElement_List(tempResultList_alloc)->contains;
					tmpTr[strnlen(tempRestChar_alloc, 100)] = 0;
					/*β�η��*/

					//printf("%s\n", GetLastElement_List(tempResultList_alloc)->contains);
					//pushElement(calculateStack, tempRestChar_alloc);

					pushElement(calculateStack, (GetLastElement_List(tempResultList_alloc))->contains);	/*���ַ����Ӷ�̬����ѹ��ջ��*/
					//printStack(calculateStack);
					break;
				case '-':
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[0] = atoi((char*)getLastElement(calculateStack));/*������ջ����ջ1��,����ת��������*/
					calBuffer[0] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[1] = atoi((char*)getLastElement(calculateStack));
					calBuffer[1] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					calBuffer[2] = calBuffer[1] - calBuffer[0];
					itoa(calBuffer[2], tempRestChar_alloc, 10);			/*ת�����ַ���ָ��*/
					AddNode_alloc(tempResultList_alloc);
					(GetLastElement_List(tempResultList_alloc))->contains = malloc(strnlen(tempRestChar_alloc, 100) + 1);/*�½���ż������Ķ�̬����ڵ����ռ�*/
					strncpy(GetLastElement_List(tempResultList_alloc)->contains, tempRestChar_alloc, strnlen(tempRestChar_alloc, 100));
					/*�½��ڵ��ڿռ俽��ԭ���ַ������¿ռ�*/
					tmpTr = GetLastElement_List(tempResultList_alloc)->contains;
					tmpTr[strnlen(tempRestChar_alloc, 100)] = 0;
					/*β�η��*/

					//printf("%s\n", GetLastElement_List(tempResultList_alloc)->contains);
					//pushElement(calculateStack, tempRestChar_alloc);

					pushElement(calculateStack, (GetLastElement_List(tempResultList_alloc))->contains);	/*���ַ����Ӷ�̬����ѹ��ջ��*/
					//printStack(calculateStack);
					break;
				case '*':
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[0] = atoi((char*)getLastElement(calculateStack));/*������ջ����ջ1��,����ת��������*/
					calBuffer[0] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[1] = atoi((char*)getLastElement(calculateStack));
					calBuffer[1] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					calBuffer[2] = calBuffer[1] * calBuffer[0];
					itoa(calBuffer[2], tempRestChar_alloc, 10);			/*ת�����ַ���ָ��*/
					AddNode_alloc(tempResultList_alloc);
					(GetLastElement_List(tempResultList_alloc))->contains = malloc(strnlen(tempRestChar_alloc, 100) + 1);/*�½���ż������Ķ�̬����ڵ����ռ�*/
					strncpy(GetLastElement_List(tempResultList_alloc)->contains, tempRestChar_alloc, strnlen(tempRestChar_alloc, 100));
					/*�½��ڵ��ڿռ俽��ԭ���ַ������¿ռ�*/
					tmpTr = GetLastElement_List(tempResultList_alloc)->contains;
					tmpTr[strnlen(tempRestChar_alloc, 100)] = 0;
					/*β�η��*/

					//printf("%s\n", GetLastElement_List(tempResultList_alloc)->contains);
					//pushElement(calculateStack, tempRestChar_alloc);

					pushElement(calculateStack, (GetLastElement_List(tempResultList_alloc))->contains);	/*���ַ����Ӷ�̬����ѹ��ջ��*/
					//printStack(calculateStack);
					break;
				case '/':
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[0] = atoi((char*)getLastElement(calculateStack));/*������ջ����ջ1��,����ת��������*/
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
					itoa(calBuffer[2], tempRestChar_alloc, 10);			/*ת�����ַ���ָ��*/
					AddNode_alloc(tempResultList_alloc);
					(GetLastElement_List(tempResultList_alloc))->contains = malloc(strnlen(tempRestChar_alloc, 100) + 1);/*�½���ż������Ķ�̬����ڵ����ռ�*/
					strncpy(GetLastElement_List(tempResultList_alloc)->contains, tempRestChar_alloc, strnlen(tempRestChar_alloc, 100));
					/*�½��ڵ��ڿռ俽��ԭ���ַ������¿ռ�*/
					tmpTr = GetLastElement_List(tempResultList_alloc)->contains;
					tmpTr[strnlen(tempRestChar_alloc, 100)] = 0;
					/*β�η��*/

					//printf("%s\n", GetLastElement_List(tempResultList_alloc)->contains);
					//pushElement(calculateStack, tempRestChar_alloc);

					pushElement(calculateStack, (GetLastElement_List(tempResultList_alloc))->contains);	/*���ַ����Ӷ�̬����ѹ��ջ��*/
					//printStack(calculateStack);
					break;
				case '%':
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[0] = atoi((char*)getLastElement(calculateStack));/*������ջ����ջ1��,����ת��������*/
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
					itoa(calBuffer[2], tempRestChar_alloc, 10);			/*ת�����ַ���ָ��*/
					AddNode_alloc(tempResultList_alloc);
					(GetLastElement_List(tempResultList_alloc))->contains = malloc(strnlen(tempRestChar_alloc, 100) + 1);/*�½���ż������Ķ�̬����ڵ����ռ�*/
					strncpy(GetLastElement_List(tempResultList_alloc)->contains, tempRestChar_alloc, strnlen(tempRestChar_alloc, 100));
					/*�½��ڵ��ڿռ俽��ԭ���ַ������¿ռ�*/
					tmpTr = GetLastElement_List(tempResultList_alloc)->contains;
					tmpTr[strnlen(tempRestChar_alloc, 100)] = 0;
					/*β�η��*/

					//printf("%s\n", GetLastElement_List(tempResultList_alloc)->contains);
					//pushElement(calculateStack, tempRestChar_alloc);

					pushElement(calculateStack, (GetLastElement_List(tempResultList_alloc))->contains);	/*���ַ����Ӷ�̬����ѹ��ջ��*/
					//printStack(calculateStack);
					break;
				case '^':
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[0] = atoi((char*)getLastElement(calculateStack));/*������ջ����ջ1��,����ת��������*/
					calBuffer[0] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					convertedVarPtr = isValueInList_alloc(varListIn, (char*)getLastElement(calculateStack));
					//calBuffer[1] = atoi((char*)getLastElement(calculateStack));
					calBuffer[1] = atoi(convertedVarPtr);
					free(convertedVarPtr);
					popElement(calculateStack);
					calBuffer[2] = (int) pow(calBuffer[1],calBuffer[0]);
					itoa(calBuffer[2], tempRestChar_alloc, 10);			/*ת�����ַ���ָ��*/
					AddNode_alloc(tempResultList_alloc);
					(GetLastElement_List(tempResultList_alloc))->contains = malloc(strnlen(tempRestChar_alloc, 100) + 1);/*�½���ż������Ķ�̬����ڵ����ռ�*/
					strncpy(GetLastElement_List(tempResultList_alloc)->contains, tempRestChar_alloc, strnlen(tempRestChar_alloc, 100));
					/*�½��ڵ��ڿռ俽��ԭ���ַ������¿ռ�*/
					tmpTr = GetLastElement_List(tempResultList_alloc)->contains;
					tmpTr[strnlen(tempRestChar_alloc, 100)] = 0;
					/*β�η��*/

					//printf("%s\n", GetLastElement_List(tempResultList_alloc)->contains);
					//pushElement(calculateStack, tempRestChar_alloc);

					pushElement(calculateStack, (GetLastElement_List(tempResultList_alloc))->contains);	/*���ַ����Ӷ�̬����ѹ��ջ��*/
					//printStack(calculateStack);
					break;
			}
			copyOrigTop++;/*ԭʼջ�ĸ���ջ����1*/
		}
		else
		{
			copyOrigTop++;/*ԭʼջ�ĸ���ջ����1*/
		}
		/*Ӧ����û��ʲô�ڴ�й©��������*/
	}
	printStack(calculateStack);
	calBuffer[2] = atoi((char*)getLastElement(calculateStack));
	DeStructStack_free(calculateStack);
	DestructList_free(tempResultList_alloc);
	return calBuffer[2];
}

