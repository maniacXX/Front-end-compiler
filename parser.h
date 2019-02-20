
// ----------------------------- parser.h --------------------------------

#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"

typedef double (*FuncPtr)(double);

struct ExprNode {	// 语法树节点类型
	
	enum Token_Type OpCode;	// 记号类别号 PLUS, MINUS, MUL, DIV,POWER, FUNC, CONST_ID 等
	
	//记号内容
	union {
		struct { ExprNode *Left, *Right; } CaseOperator;		//运算符结点类型

		struct { ExprNode *Child; FuncPtr MathFuncPtr; } CaseFunc;	//函数结点类型

		double CaseConst;		//常数

		double *CaseParmPtr;	//变量 T

	} Content;
};

extern void Parser(char *SrcFilePtr); // 语法分析器主程序

#endif
