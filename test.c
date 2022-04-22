#include <stdio.h>
#include "dynarray.h"

int main(void)
{
	listhead* testList = StructList_alloc(0x20);
	PrintList(testList);
	DestructList_free(testList);
	return(0);
}
