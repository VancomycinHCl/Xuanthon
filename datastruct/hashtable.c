#pragma once
#include <stdint.h>
#include <stdlib.h>
#include "hashtable.h"

#ifdef _DEBUG
	#include <stdio.h>
#endif


hasht* InitTable_alloc(uint8_t lengthIn)
{
	hasht* returnptr;					/*give a ptr to the struct*/
	returnptr = (hasht*) malloc(sizeof(hasht));	/* give a heap space to the ptr*/
	if (returnptr != NULL)
	{
		returnptr->length = lengthIn;		/*length register*/
		returnptr->table = malloc(sizeof(int*) * lengthIn); /*give the ptrArray table a space to contain the ptr*/
		return (returnptr);					/*return*/
	}
	else
	{
		return returnptr;
	}
}

int SumEssential(char* elementIn) /*Here I intended to sum up the ascii value of the inputted element (Mostly is variable, contains key & key value)*/
{
	char charPointer = elementIn[0];
	int count = 0;
	int returnSum = 0;

	while (charPointer)
	{
		returnSum += (uint8_t)charPointer;
		charPointer = elementIn[++count];
	}

	return returnSum;
}

int SumNEssential(char* elementIn,int numberLimit) /*Another version, but have a limit in the length of string*/
{
	char charPointer = elementIn[0];
	int count = 0;
	int returnSum = 0;

	while (charPointer && count<numberLimit)
	{
		returnSum += (uint8_t)charPointer;
		charPointer = elementIn[++count];
	}

	return returnSum;
}


int CalculateRemainder(hasht* targetTable, int essentialIn)/*One part of the hash func, in charge of calculate a hash value from the given*/
{
	if (targetTable)
	{
		return essentialIn % targetTable->length;
	}
	else
	{
		return (-1);
	}
}

int* CalculateHashValue(hasht* targetTable, int(*HashFunction(char* elementIn)),char* elementIn)/*func ptr as a parameter, so that we can change the hash table by changing the hash func.*/
{
	int essential = 0;
	int offSet = 0;
	int* returnPtr = NULL;

	essential = HashFunction(elementIn);							//calculate the essential word
	offSet = CalculateRemainder(targetTable,essential);				//calculate the hash table combined with the length of hash table
	returnPtr = offSet != -1 ? targetTable->table + offSet:NULL;	
	//if the targetTable does not exist, the last statement will return as -1, and the result will return 0(NULL)
	//if the offset exist, we will add the base address with the result as a returing
	return returnPtr;
}

void ClearHashNode(hasht* targetTable) /*delete the int ptr in the table*/
{
	uint8_t count = 0;
	while (count < targetTable->length)
	{
		count++;
	}
	targetTable->table[count] = NULL;
}

hasht* ClearHashTable(hasht* targetTable)
{
	int count = 0;

	if (targetTable)
	{
		while (count < targetTable->length)
		{
			targetTable->table[count] = NULL;
			count++;
		}

		return targetTable;
	}
	else
	{
		return targetTable;
	}

}

hasht* DeleteHashTable(hasht* targetTable)
{
	int count = 0;

	if (targetTable)
	{
		while (count < targetTable->length)
		{
			targetTable->table[count] = NULL;
			count++;
		}
		free(targetTable);
		return NULL;
	}
	else
	{
		return targetTable;
	}

}