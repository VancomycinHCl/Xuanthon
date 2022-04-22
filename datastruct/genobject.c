#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "genobject.h"


LiuthonObject* StructObject()
{
	LiuthonObject* returnObject = (LiuthonObject*)malloc(sizeof(LiuthonObject));
	returnObject->object_detail = NULL;
	returnObject->reference_times = 0;
	return returnObject;
}

uint8_t IsZeroReference(LiuthonObject* targetObject)
{
	uint8_t returning = 0;
	returning = targetObject->reference_times ? 1 : 0;
	return returning;
}

LiuthonObject* DestructObject(LiuthonObject* targetObject)
{
	uint8_t referenceTime = 0;
	referenceTime = IsZeroReference(targetObject);
	if (!referenceTime)
	{
		free(targetObject->object_detail);
		targetObject->object_detail = NULL;
		free(targetObject);
		return(targetObject);
	}
	else
	{
		return NULL;
	}
}

void ValueingObject(LiuthonObject* targetObject, void* valueIn)
{
	targetObject->object_detail = valueIn;
	targetObject->reference_times++;
}