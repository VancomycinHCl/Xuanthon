#pragma once
#include "stack.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "dynarray.h"
#include "calMem.h"


uint8_t SigPower(char* elementIn);
filoarray* ReversePolandExp_alloc(char* ExpressionIn);
void printStack(filoarray* stack1);
char* printStack_char_alloc(filoarray* stack1);
int calculateStack(filoarray* stack1,listhead* varListIn);