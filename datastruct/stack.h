#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef struct stack filoarray;

#define DEFAULT_MAXSIZE 0x80///<default length of the stack
#define STACK_OVER_FLOW -2	///<error code when stack overflow confrented
#define STACK_UNDER_FLOW -1	///<error code when stack underflow confrented

/**
* @brief the structure of a stack object
*/
struct stack
{
	void** fifoArray;	///<the base address of the void ptr array
	int top;			///<the stack top offest
	int size;			///<maxinum size of the array 
};

filoarray* StructStack_alloc(const int MAXSIZE);
filoarray* DeStructStack_free(filoarray* targetArray);

void* popElement(filoarray* targetArray);
void* pushElement(filoarray* targetArray, void* targetElementIn);
void* getLastElement(filoarray* targetArray);
int CheckArrayState(filoarray* targetArry);
void ClearStack(filoarray* targetArray);