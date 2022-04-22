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

//NumberAnalyse_alloc��Ҫ�����б������ַ����Ƿ��������򸡵��������������������token��̬������
//����״̬�°���˵�ܽ������硰8��8.0��8.�����������͵�������ʽ�������ܹ��б��Ƿ���8a��������ֵֹı�������������������δ������Ϊtoken��Ԫ�����ڵ㡱
//����ִ����Ϻ�᷵��������ֹλ�õĺ����Ǹ���ַ������  "a=8.0+1;" ���صľ���+�ĵ�ַ, "8a = 114514;"���صľ���a�ĵ�ַ

//������ת����->�������ִ�ͷ�ַ���->��ʼ�����ַ���->����С����->��������->�����������->ֹͣ��������ֵ��������token��
//														|		 |===>����δ�����������ĸ����ֹ��δ����ڵ������
//														|==>û������С����->���������Ѷ������->ֹͣ��������ֵ������token��
//																	|=====>��������δ���������������������ĸ����->����δ���屨��token

/*----------------------------------------------------------------*/

char* NumberAnalyse_alloc(char* currentPtr, char* lastPtr, listhead* tokenList)/*���ַ������Զ������Ӧ����/С����token�����ص�token����*/
{
	int arrayLength = 0;
	lastPtr = currentPtr; //����һ�ַ�ָ��ק����ǰָ��λ��
	while (*currentPtr <= '9' && *currentPtr >= '0')//�ڵ�ǰ������ַ�������ʱ
	{
		currentPtr++;								//����������
	}
	if (*currentPtr == '.')							//����С����ʱ
	{
		currentPtr++;
		while (*currentPtr <= '9' && *currentPtr >= '0')//С���������
		{
			currentPtr++;
		}
		if (*currentPtr == '+' || *currentPtr == '-' || *currentPtr == '*' || *currentPtr == '/' || *currentPtr == '%' ||
			*currentPtr == '^' || *currentPtr == '=' || *currentPtr == ';' || *currentPtr == '(' || *currentPtr == ')' ||
			*currentPtr == ' ' || *currentPtr == '\n')				/*С�������Ҳû�����ˣ����Һ����Ƕ�����ַ�*/
		{
			TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
			newNode->contains = lastPtr;							/*ע����Ϣ����������ڵ�İ�������ʼλ����Ҫ�ǹ̶����ϴδ���ͷ*/
			newNode->length = currentPtr - lastPtr;					/*���Ⱦ��ǵ�ǰ����ķ������ַ�λ��-��ʼλ��*/
			newNode->token = IS_FLNUM;
			AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
		}
		else{
			TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
			newNode->contains = lastPtr;							/*ע����Ϣ����������ڵ�İ�������ʼλ����Ҫ�ǹ̶����ϴδ���ͷ*/
			newNode->length = 0;									/*����δ������Ϊ��������Ҫ���ս�һ������*/
			newNode->token = IS_UNDEF;								/*δ�����־�����泤��Ϊ0*/
			AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
			arrayLength = GetLength(tokenList);						/*���Ԫ��*/
			ModifyElement(tokenList, arrayLength, newNode);
		}
	}

	else if(*currentPtr == '+' || *currentPtr == '-' || *currentPtr == '*' || *currentPtr == '/' || *currentPtr == '%' ||
			*currentPtr == '^' || *currentPtr == '=' || *currentPtr == ';' || *currentPtr == '(' || *currentPtr == ')' ||
			*currentPtr == ' ' || *currentPtr == '\n')			//û��С����������Ѷ����ַ�
	{
		TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
		newNode->contains = lastPtr;							/*ע����Ϣ����������ڵ�İ�������ʼλ����Ҫ�ǹ̶����ϴδ���ͷ������������Ϊ��ַ*/
		newNode->length = currentPtr-lastPtr;					/*���Ⱦ��ǵ�ǰ���Ŵ�ָ��-��һ�εط��Ĳ�����Ѿ����������ֽ�ֹ�����ʲ��ü�һ���������ƫ����*/
		newNode->token = IS_ITNUM;								/*��עΪ����*/
		AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
		arrayLength = GetLength(tokenList);
		ModifyElement(tokenList, arrayLength, newNode);			/*ִ�����*/
	}
	else {
		TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
		newNode->contains = lastPtr;							/*ע����Ϣ����������ڵ�İ�������ʼλ����Ҫ�ǹ̶����ϴδ���ͷ*/
		newNode->length = 0;
		newNode->token = IS_UNDEF;
		AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
		arrayLength = GetLength(tokenList);
		ModifyElement(tokenList, arrayLength, newNode);
	}
	return currentPtr;
}

char* LetterVariableAnalyse_alloc(char* currentPtr, char* lastPtr, listhead* tokenList)
{
	int arrayLength = 0;
	lastPtr = currentPtr;								//��������������prel��������������ʽʱ��([a-Z]|[0-9]|_)+
	while ( (*currentPtr >= 'a' && *currentPtr <= 'z' ) || (*currentPtr >= 'A' && *currentPtr <= 'Z') || (*currentPtr <= '9' && *currentPtr >= '0') || *currentPtr == '_')
	{
		currentPtr++;
	}
	if (*currentPtr == '+' || *currentPtr == '-' || *currentPtr == '*' || *currentPtr == '/' || *currentPtr == '%' ||
		*currentPtr == '^' || *currentPtr == '=' || *currentPtr == ';' || *currentPtr == '(' || *currentPtr == ')' ||
		*currentPtr == ' ' || *currentPtr == '\n')
	{
		TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
		newNode->contains = lastPtr;							/*ע����Ϣ����������ڵ�İ�������ʼλ����Ҫ�ǹ̶����ϴδ���ͷ*/
		newNode->length = currentPtr - lastPtr;					/*���Ⱦ��ǵ�ǰ��������λ��-��ʼλ��*/
		newNode->token = IS_VAR;
		AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
		arrayLength = GetLength(tokenList);
		ModifyElement(tokenList, arrayLength, newNode);
	}
	else
	{
		TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
		newNode->contains = lastPtr;							/*ע����Ϣ����������ڵ�İ�������ʼλ����Ҫ�ǹ̶����ϴδ���ͷ*/
		newNode->length = 0;									/*����δ������Ϊ��������Ҫ���ս�һ������*/
		newNode->token = IS_UNDEF;								/*δ�����־�����泤��Ϊ0*/
		AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
		arrayLength = GetLength(tokenList);						/*���Ԫ��*/
		ModifyElement(tokenList, arrayLength, newNode);
	}
	return currentPtr;
}

listhead* LexicalAnalysis_alloc(char* commandIn)
{
	char* currentPtr = commandIn;						/*��ǰ������Ǹ�λ��*/
	char* lastPtr = commandIn;							/*��һ�������ؼ��ֵ�λ��*/
	uint16_t retToken = IS_EMPTY;							/*�����Ƿ�Ϊ�ջ����дʷ�����*/
	listhead* tokenList = StructList_alloc(0);				/*����һ����̬���飬�������tokenԪ��*/
	uint8_t arrayLength = 0;

	while (*currentPtr != '\0' && *currentPtr != '\n')		/*��û������ͷʱ*/
	{
		if (*currentPtr == ' ')									/*�����ո�����*/
		{
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == '=')								/*�������ں�*/
		{
			TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
			newNode->contains = currentPtr;							/*ע����Ϣ*/
			newNode->length = 1;
			newNode->token = IS_EUQA;
			AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList,arrayLength,newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == '+')
		{
			TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
			newNode->contains = currentPtr;							/*ע����Ϣ*/
			newNode->length = 1;
			newNode->token = IS_PLUS;
			AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == '-')
		{
			TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
			newNode->contains = currentPtr;							/*ע����Ϣ*/
			newNode->length = 1;
			newNode->token = IS_MINUS;
			AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == '/')
		{
			TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
			newNode->contains = currentPtr;							/*ע����Ϣ*/
			newNode->length = 1;
			newNode->token = IS_DIVI;
			AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == '*')
		{
			TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
			newNode->contains = currentPtr;							/*ע����Ϣ*/
			newNode->length = 1;
			newNode->token = IS_MULT;
			AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == '^')
		{
			TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
			newNode->contains = currentPtr;							/*ע����Ϣ*/
			newNode->length = 1;
			newNode->token = IS_POWE;
			AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == '%')
		{
			TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
			newNode->contains = currentPtr;							/*ע����Ϣ*/
			newNode->length = 1;
			newNode->token = IS_MOD;
			AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == '(')
		{
			TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
			newNode->contains = currentPtr;							/*ע����Ϣ*/
			newNode->length = 1;
			newNode->token = IS_LBRAK;
			AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == ')')
		{
			TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
			newNode->contains = currentPtr;							/*ע����Ϣ*/
			newNode->length = 1;
			newNode->token = IS_RBRAK;
			AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr == ';')
		{
			TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
			newNode->contains = currentPtr;							/*ע����Ϣ*/
			newNode->length = 1;
			newNode->token = IS_SECOL;
			AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr++;
		}
		else if (*currentPtr <= '9' && *currentPtr >= '0')					/*�������ؼ�����һ����ĸ�����б�֮ǰ�������������б���������ɸѡ�� "8a" ���ֲ������﷨�淶�����ֺͱ���*/
		{
			lastPtr = currentPtr;											/*��һ�εĴ���ָ����ڵ�ǰָ�룬��Ϊ���ﲻ��Ҫ�ع�*/
			currentPtr = NumberAnalyse_alloc(currentPtr,lastPtr,tokenList);	/*���������б����жϲ����token����ǰָ����Զ�ֹͣ�����ֽ���/δ������Ϊ�����ĵط������Բ���Ҫ������*/
		}
		else if (strncmp(currentPtr, "int ", 4) == 0 )			/*���б���ĸ���ɵı���֮ǰ��Ҫ�б�int��float�ؼ��֣��ؼ��ֳ��ָ�ʽΪ��int���ո� */
		{
			TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
			newNode->contains = currentPtr;							/*ע����Ϣ*/
			newNode->length = 4;
			newNode->token = IS_INT;
			AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr+=4;
		}
		else if (strncmp(currentPtr, "float ", 6) == 0)
		{
			TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
			newNode->contains = currentPtr;							/*ע����Ϣ*/
			newNode->length = 6;
			newNode->token = IS_FLOAT;
			AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
			arrayLength = GetLength(tokenList);
			ModifyElement(tokenList, arrayLength, newNode);
			lastPtr = currentPtr;
			currentPtr+=6;
		}
		else if ((*currentPtr >= 'A' && *currentPtr <= 'Z') || (*currentPtr >= 'a' && *currentPtr <= 'z')) /*��������ַ�����ĸ�ַ�ʱ*/
		{
		/*��Ҫ������ĸ�б���򣬴��¹����������б�һ�£����Ȱ��������ַ�ֱ������δ�����ַ������Ѷ��������ֹͣ��ȡ��д��ڵ���������ֹ�ĵ�ַ*/
			lastPtr = currentPtr;														/*��һ�εĴ���ָ����ڵ�ǰָ�룬��Ϊ���ﲻ��Ҫ�ع�*/
			currentPtr = LetterVariableAnalyse_alloc(currentPtr, lastPtr, tokenList);	/*������ĸ�б����жϲ����token����ǰָ����Զ�ֹͣ�����ֽ���/δ������Ϊ�����ĵط������Բ���Ҫ������*/
		}
		else														/*�����Ƿ���δ�����ַ�ʱ*/
		{
			TokenNode* newNode = StructNode_alloc();				/*�½�һ��token��ϢԪ��*/
			newNode->contains = lastPtr;							/*ע����Ϣ����������ڵ�İ�������ʼλ����Ҫ�ǹ̶����ϴδ���ͷ*/
			newNode->length = 0;									/*δ������Ϊ���ȼ���Ϊ0*/
			newNode->token = IS_UNDEF;								/*��עδ������ű�ǩ*/
			AddNode_alloc(tokenList);								/*token��̬������Ӷ�ӦԪ��*/
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
