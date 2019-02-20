
// ----------------------------- scanner.h --------------------------------

#ifndef SCANNER_H
#define SCANNER_H
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>

// 单词种别码
enum Token_Type { 
	ORIGIN, SCALE, ROT, IS, TO, 			// 保留字
	STEP, DRAW, FOR, FROM, 					// 保留字
	T, 										// 参数
	SEMICO, L_BRACKET, R_BRACKET, COMMA, 	// 分隔符号
	PLUS, MINUS, MUL, DIV, POWER, 	// 运算符
	FUNC, 									// 函数
	CONST_ID, 								// 常数
	NONTOKEN, 								// 空记号
	ERRTOKEN 								// 出错记号
};

typedef double (*MathFuncPtr) (double);		// 数学函数指针

// 记号与符号表结构
struct Token {
	Token_Type type; 			// 记号类别
	char *lexeme;				// 字符串常量值
	double value;				// 常数值
	double (*FuncPtr)(double); 	// 函数指针
};

// 预定义单词表
static struct Token TokenTab[18] = {
	//语言中使用的符号常数
	{CONST_ID,  "PI",		3.1415926,	NULL},
	{CONST_ID,  "E",		2.71828,	NULL},
	//语言中惟一的变量
	{T,			"T",		0.0,		NULL},
	//语言允许用的数学函数名
	{FUNC,		"SIN",		0.0,		sin},
	{FUNC,		"COS",		0.0,		cos},
	{FUNC,		"TAN",		0.0,		tan},
	{FUNC,		"LN",		0.0,		log},
	{FUNC,		"EXP",		0.0,		exp},
	{FUNC,		"SQRT",		0.0,		sqrt},
	//语句关键字
	{ORIGIN,	"ORIGIN",	0.0,		NULL},
	{SCALE,		"SCALE",	0.0,		NULL},
	{ROT,		"ROT",		0.0,		NULL},
	{IS,		"IS",		0.0,		NULL},
	{FOR,		"FOR",		0.0,		NULL},
	{FROM,		"FROM",		0.0,		NULL},
	{TO,		"TO",		0.0,		NULL},
	{STEP,		"STEP",		0.0,		NULL},
	{DRAW,		"DRAW",		0.0,		NULL}
};

extern unsigned int LineNo; 		// 跟踪记号所在源文件行号

extern int InitScanner (const char *); 	// 初始化词法分析器

extern Token GetToken (void); 		// 获取记号函数

extern void CloseScanner (void); 	// 关闭词法分析器
