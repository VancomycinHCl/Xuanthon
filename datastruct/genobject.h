#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


typedef struct GeneralObject LiuthonObject;

/*Here I defined a general class in order to generate and destory variables*/
/*The hand-made coding language doesn't support array in the origional designing, so there is no difference in the size of variables*/
/*If the reference time sets to zero, the virtue machine will be given a permission to free the allocated heap.*/
/*object_detail is in charge of linking the actual data type (also the struct ptr)*/
/*Variables will be characted by its address, in order to store them in the dynamic array under the management of a hash table*/
/*To extract the key-value archecture, the detailed formate is ====> {key:=var name ; value:=address of General Object}*/

/*Refered from Cpython API -> object.h*/

struct GeneralObject
{
	void* object_detail;
	int reference_times;
};

LiuthonObject* StructObject();
uint8_t IsZeroReference(LiuthonObject* targetObject);
LiuthonObject* DestructObject(LiuthonObject* targetObject);
void ValueingObject(LiuthonObject* targetObject,void* valueIn);