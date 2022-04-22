#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define DEFAULT_LIST_LENGTH 0x80
#define _DEBUG 1

typedef struct body listbody;
typedef struct head listhead;

/**
* @brief The contains of linklist body is 1. void ptr, so that can mount any objects, class body's ptr, for the next one. And the integer calltime is for the potential reference time and destructure system
*/
struct body
{
	void* contains; ///<contained object ptr
	listbody* next; ///<next body object
	int callTime;	///<count for the reference time
};

/**
* @brief contains the length of the array and the start body object's address.
*/
struct head
{
	uint8_t length;	///<length of the array
	listbody* start;///<start of the list in body object
};

//#define FOR_LIST_ITER(targetList,index)\
//for(listbody* nextNode = targetList->start;nextNode != NULL; nextNode = nextNode->next)


listhead* StructList_alloc(uint8_t listLength);
listhead* DestructList_free(listhead* targetList);

listbody* AddNode_alloc(listhead* targetList);
listbody* InsertNode_alloc(listhead* targetList, const int position);
listbody* DeleteNode_free(listhead* targrtList, const int position);
listbody* PopNode_free(listhead* targetList);
listbody* IterList(listhead* targetList, const int position);
uint8_t GetLength(listhead* targetList);
listbody* ModifyElement(listhead* targetList, const int position, void* targetElement);
listbody* GetLastElement_List(listhead* targetList);

#ifdef _DEBUG
void PrintList(listhead* targetList);
#endif
