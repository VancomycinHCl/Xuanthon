#pragma once
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "dynarray.h"
//
#define IS_EMPTY 0x0000
#define IS_EUQA  0x0001
#define IS_BLANK 0x0002
#define IS_DOT   0x0003
#define IS_PLUS  0x0004
#define IS_MINUS 0x0005
#define IS_MULT  0x0006
#define IS_DIVI  0x0007
#define IS_POWE  0x0008
#define IS_MOD   0x0009
#define IS_LBRAK 0x000A
#define IS_RBRAK 0x000B
#define IS_SECOL 0x000C
#define IS_ITNUM 0x000D
#define IS_FLNUM 0x000E

#define IS_INT   0x000F
#define IS_FLOAT 0x0010
#define IS_VAR   0x0011
#define IS_UNDEF 0x0012

typedef struct tokenNode TokenNode;

struct tokenNode
{
	char* contains;
	uint8_t length;
	uint16_t token;
};

TokenNode* StructNode_alloc();
char* NumberAnalyse_alloc(char* currentPtr, char* lastPtr, listhead* tokenList);
char* LetterVariableAnalyse_alloc(char* currentPtr, char* lastPtr, listhead* tokenList);
listhead* LexicalAnalysis_alloc(char* commandIn);