#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "lexianal.h"
#include "dynarray.h"

TokenNode* StructNode_alloc()
{
	TokenNode* retNode = malloc(sizeof(TokenNode));
	retNode->contains = NULL;
	retNode->length = 0;
	retNode->token = IS_EMPTY;
	return retNode;
}

/*----------------------------------------------------------------*/

//NumberAnalyse_alloc主要负责判别输入字符串是否是整数或浮点数，并将操作结果插入token动态数组中
//正常状态下按理说能解析形如“8、8.0、8.、这三种类型的数字形式，并且能够判别是否是8a这种奇奇怪怪的变量命名，如果是则添加未定义行为token的元组进入节点”
//程序执行完毕后会返回数字终止位置的后面那个地址，比如  "a=8.0+1;" 返回的就是+的地址, "8a = 114514;"返回的就是a的地址

//程序运转流程->读入数字打头字符串->开始遍历字符串->遇到小数点->继续遍历->遇到定义算符->停止返回正常值（浮点数token）
//														|		 |===>遇到未定义算符与字母则终止报未定义节点进链表
//														|==>没有遇到小数点->遍历遇到已定义算符->停止返回正常值（整数token）
//																	|=====>遍历遇到未定义算符或者遇到数字字母混杂->返回未定义报错token

/*----------------------------------------------------------------*/

char* NumberAnalyse_alloc(char* currentPtr, char* lastPtr, listhead* tokenList)/*数字分析，自动添加相应整数/小数的token并挂载到token表中*/
{
	int arrayLength = 0;
	lastPtr = currentPtr; //把上一字符指针拽到当前指针位置
	while (*currentPtr <= '9' && *currentPtr >= '0')//在当前处理的字符是数字时
	{
		currentPtr++;								//接着往下走
	}
	if (*currentPtr == '.')							//处理到小数点时
	{
		currentPtr++;
		while (*currentPtr <= '9' && *currentPtr >= '0')//小数点后有数
		{
			currentPtr++;
		}
		if (*currentPtr == '+' || *currentPtr == '-' || *currentPtr == '*' || *currentPtr == '/' || *currentPtr == '%' ||
			*currentPtr == '^' || *currentPtr == '=' || *currentPtr == ';' || *currentPtr == '(' || *currentPtr == ')' ||
			*currentPtr == ' ' || *currentPtr == '\n')				/*小数点后面也没有数了，并且后面是定义的字符*/
		{
			TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
			newNode->contains = lastPtr;							/*注册信息，但是这个节点的包含物起始位置需要是固定的上次处理头*/
			newNode->length = currentPtr - lastPtr;					/*长度就是当前定义的非数字字符位置-起始位置*/
			newNode->token = IS_FLNUM;
			AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
		}
		else{
			TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
			newNode->contains = lastPtr;							/*注册信息，但是这个节点的包含物起始位置需要是固定的上次处理头*/
			newNode->length = 0;									/*定义未定义行为，这里需要最终接一个报错*/
			newNode->token = IS_UNDEF;								/*未定义标志，上面长度为0*/
			AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
			arrayLength = GetLength(tokenList);						/*添加元素*/
			ModifyElement(tokenList, arrayLength, newNode);
		}
	}

	else if(*currentPtr == '+' || *currentPtr == '-' || *currentPtr == '*' || *currentPtr == '/' || *currentPtr == '%' ||
			*currentPtr == '^' || *currentPtr == '=' || *currentPtr == ';' || *currentPtr == '(' || *currentPtr == ')' ||
			*currentPtr == ' ' || *currentPtr == '\n')			//没有小数点后面是已定义字符
	{
		TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
		newNode->contains = lastPtr;							/*注册信息，但是这个节点的包含物起始位置需要是固定的上次处理头，这里可以理解为基址*/
		newNode->length = currentPtr-lastPtr;					/*长度就是当前符号处指针-上一次地方的差，由于已经到达了数字截止处，故不用加一，这里就是偏移量*/
		newNode->token = IS_ITNUM;								/*标注为整数*/
		AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
		arrayLength = GetLength(tokenList);
		ModifyElement(tokenList, arrayLength, newNode);			/*执行添加*/
	}
	else {
		TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
		newNode->contains = lastPtr;							/*注册信息，但是这个节点的包含物起始位置需要是固定的上次处理头*/
		newNode->length = 0;
		newNode->token = IS_UNDEF;
		AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
		arrayLength = GetLength(tokenList);
		ModifyElement(tokenList, arrayLength, newNode);
	}
	return currentPtr;
}

char* LetterVariableAnalyse_alloc(char* currentPtr, char* lastPtr, listhead* tokenList)
{
	int arrayLength = 0;
	lastPtr = currentPtr;								//当我们遇到的是prel风格的如下正则表达式时：([a-Z]|[0-9]|_)+
	while ( (*currentPtr >= 'a' && *currentPtr <= 'z' ) || (*currentPtr >= 'A' && *currentPtr <= 'Z') || (*currentPtr <= '9' && *currentPtr >= '0') || *currentPtr == '_')
	{
		currentPtr++;
	}
	if (*currentPtr == '+' || *currentPtr == '-' || *currentPtr == '*' || *currentPtr == '/' || *currentPtr == '%' ||
		*currentPtr == '^' || *currentPtr == '=' || *currentPtr == ';' || *currentPtr == '(' || *currentPtr == ')' ||
		*currentPtr == ' ' || *currentPtr == '\n')
	{
		TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
		newNode->contains = lastPtr;							/*注册信息，但是这个节点的包含物起始位置需要是固定的上次处理头*/
		newNode->length = currentPtr - lastPtr;					/*长度就是当前定义的算符位置-起始位置*/
		newNode->token = IS_VAR;
		AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
		arrayLength = GetLength(tokenList);
		ModifyElement(tokenList, arrayLength, newNode);
	}
	else
	{
		TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
		newNode->contains = lastPtr;							/*注册信息，但是这个节点的包含物起始位置需要是固定的上次处理头*/
		newNode->length = 0;									/*定义未定义行为，这里需要最终接一个报错*/
		newNode->token = IS_UNDEF;								/*未定义标志，上面长度为0*/
		AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
		arrayLength = GetLength(tokenList);						/*添加元素*/
		ModifyElement(tokenList, arrayLength, newNode);
	}
	return currentPtr;
}

listhead* LexicalAnalysis_alloc(char* commandIn)
{
	char* currentPtr = commandIn;						/*当前处理的那个位置*/
	char* lastPtr = commandIn;							/*上一次遇到关键字的位置*/
	uint16_t retToken = IS_EMPTY;							/*整行是否为空或者有词法错误*/
	listhead* tokenList = StructList_alloc(0);				/*构造一个动态数组，用来存放token元组*/
	uint8_t arrayLength = 0;

	while (*currentPtr != '\0' && *currentPtr != '\n')		/*当没遇到尽头时*/
	{
		if (*currentPtr == ' ')									/*遇到空格跳过*/
		{
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == '=')								/*遇到等于号*/
		{
			TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
			newNode->contains = currentPtr;							/*注册信息*/
			newNode->length = 1;
			newNode->token = IS_EUQA;
			AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList,arrayLength,newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == '+')
		{
			TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
			newNode->contains = currentPtr;							/*注册信息*/
			newNode->length = 1;
			newNode->token = IS_PLUS;
			AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == '-')
		{
			TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
			newNode->contains = currentPtr;							/*注册信息*/
			newNode->length = 1;
			newNode->token = IS_MINUS;
			AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == '/')
		{
			TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
			newNode->contains = currentPtr;							/*注册信息*/
			newNode->length = 1;
			newNode->token = IS_DIVI;
			AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == '*')
		{
			TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
			newNode->contains = currentPtr;							/*注册信息*/
			newNode->length = 1;
			newNode->token = IS_MULT;
			AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == '^')
		{
			TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
			newNode->contains = currentPtr;							/*注册信息*/
			newNode->length = 1;
			newNode->token = IS_POWE;
			AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == '%')
		{
			TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
			newNode->contains = currentPtr;							/*注册信息*/
			newNode->length = 1;
			newNode->token = IS_MOD;
			AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == '(')
		{
			TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
			newNode->contains = currentPtr;							/*注册信息*/
			newNode->length = 1;
			newNode->token = IS_LBRAK;
			AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == ')')
		{
			TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
			newNode->contains = currentPtr;							/*注册信息*/
			newNode->length = 1;
			newNode->token = IS_RBRAK;
			AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == ';')
		{
			TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
			newNode->contains = currentPtr;							/*注册信息*/
			newNode->length = 1;
			newNode->token = IS_SECOL;
			AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr <= '9' && *currentPtr >= '0')					/*在启动关键字与一般字母变量判别之前，先启动数字判别，这样可以筛选出 "8a" 这种不符合语法规范的数字和变量*/
		{
			lastPtr = currentPtr;											/*上一次的处理指针等于当前指针，因为这里不需要回滚*/
			currentPtr = NumberAnalyse_alloc(currentPtr,lastPtr,tokenList);	/*送入数字判别函数判断并添加token，当前指针会自动停止在数字结束/未定义行为发生的地方，所以不需要步进了*/
		}
		else if (strncmp(currentPtr, "int ", 4) == 0 )			/*在判别字母构成的变量之前先要判别int和float关键字，关键字出现格式为：int’空格‘ */
		{
			TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
			newNode->contains = currentPtr;							/*注册信息*/
			newNode->length = 4;
			newNode->token = IS_INT;
			AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr+=4;
		}
		else if (strncmp(currentPtr, "float ", 6) == 0)
		{
			TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
			newNode->contains = currentPtr;							/*注册信息*/
			newNode->length = 6;
			newNode->token = IS_FLOAT;
			AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr+=6;
		}
		else if ((*currentPtr >= 'A' && *currentPtr <= 'Z') || (*currentPtr >= 'a' && *currentPtr <= 'z')) /*当送入的字符是字母字符时*/
		{
		/*需要启动字母判别程序，大致过程与数字判别一致，首先挨个读入字符直到遇到未定义字符或者已定义算符则停止读取并写入节点打包返回终止的地址*/
			lastPtr = currentPtr;														/*上一次的处理指针等于当前指针，因为这里不需要回滚*/
			currentPtr = LetterVariableAnalyse_alloc(currentPtr, lastPtr, tokenList);	/*送入字母判别函数判断并添加token，当前指针会自动停止在数字结束/未定义行为发生的地方，所以不需要步进了*/
		}
		else														/*当我们发现未定义字符时*/
		{
			TokenNode* newNode = StructNode_alloc();				/*新建一个token信息元组*/
			newNode->contains = lastPtr;							/*注册信息，但是这个节点的包含物起始位置需要是固定的上次处理头*/
			newNode->length = 0;									/*未定义行为长度计算为0*/
			newNode->token = IS_UNDEF;								/*标注未定义符号标签*/
			AddNode_alloc(tokenList);								/*token动态数组添加对应元素*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		
		//lastPtr = currentPtr;
		//currentPtr++;
	}

	return tokenList;
}
