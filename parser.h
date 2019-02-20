
// ----------------------------- parser.h --------------------------------

#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"

typedef double (*FuncPtr)(double);

struct ExprNode {	// �﷨���ڵ�����
	
	enum Token_Type OpCode;	// �Ǻ����� PLUS, MINUS, MUL, DIV,POWER, FUNC, CONST_ID ��
	
	//�Ǻ�����
	union {
		struct { ExprNode *Left, *Right; } CaseOperator;		//������������

		struct { ExprNode *Child; FuncPtr MathFuncPtr; } CaseFunc;	//�����������

		double CaseConst;		//����

		double *CaseParmPtr;	//���� T

	} Content;
};

extern void Parser(char *SrcFilePtr); // �﷨������������

#endif
