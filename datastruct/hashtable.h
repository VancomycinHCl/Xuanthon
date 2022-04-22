#pragma once
#include <stdint.h>
#include <stdlib.h>

#define _DEBUG 1
#define DEFAULT_LENGTH 0x80;

#ifdef _DEBUG
	#include <stdio.h>
#endif

typedef struct hashtab
{
	uint8_t length;
	int** table;	/*here is to define an int ptr array to store a the ptr of vars*/
}hasht;

int CalculateRemainder(hasht* targetTable,int essentialIn);


hasht* InitTable_alloc(uint8_t length);
hasht* ClearHashTable(hasht* targetTable);
hasht* DeleteHashTable(hasht* targetTable);

int SumEssential(char* elementIn);
int SumNEssential(char* elementIn, int numberLimit);
int* CalculateHashValue(hasht* targetTable, int(*HashFunction(char* elementIn)), char* elementIn);
int CalculateRemainder(hasht* targetTable, int essentialIn);

void ClearHashNode(hasht* targetTable);