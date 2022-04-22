#pragma once
#include <stdint.h>
#include <stdlib.h>

#define _DEBUG 1

#if _DEBUG
	#include <stdio.h>
#endif

#ifndef DEFAULT_ARRAY_SIZE

	#define DEFAULT_ARRAY_SIZE 0x80

#endif // !ARRAY_SIZE

typedef struct fifoArray
{
	void* readptr;
	void* writeptr;
	void* initptr;
	uint8_t size;
}fifoArray;
// I newed a new type called fifoArray, the containers in the fifoArray is a void ptr, which can mount the general type of variables.

fifoArray* InitFIFOArray_alloc(uint8_t arraySize);
fifoArray* DeleteArray_free(fifoArray* targetArray);

void ClearArray(fifoArray* targetArray);
void PushArray(fifoArray* targetArray, void* elementIn);
void PopArray(fifoArray* targetArray);
void PrintArray(fifoArray* targetArray);

uint8_t IsArrayFull(fifoArray* targetArray);
