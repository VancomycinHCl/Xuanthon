#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "fifoarray.h"

fifoArray* InitFIFOArray_alloc(const uint8_t arraySize)
{
	uint8_t actualSize = arraySize;
	if (arraySize > 0x80) {
		actualSize = 0x80;
	}
	fifoArray* returnArray = (fifoArray*) malloc(sizeof(fifoArray));
	returnArray->initptr = malloc(sizeof(void*) * actualSize);
	returnArray->readptr = returnArray->initptr;
	returnArray->writeptr = returnArray->initptr;
	returnArray->size = actualSize;
	return (returnArray);
}

fifoArray* DeleteArray_free(fifoArray* targetArray)
{
	free(targetArray->initptr);
	targetArray->initptr = NULL;
	targetArray->readptr = NULL;
	targetArray->writeptr = NULL;
	free(targetArray);
	return (targetArray);
}

void ClearArray(fifoArray* targetArray)
{
	uint8_t count = targetArray->size;
	void* listIn = targetArray->initptr;
	while (count>0)
	{
		(int*) listIn[count-1] = NULL;
		
	}
	count++;

	return targetArray;
}
