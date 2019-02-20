
// ----------------------------- parser.cpp --------------------------------
//201631061113 徐高瑞
#include "parser.h"

#define call_match(x) printf("匹配记号："); printf(x), printf("\n")

#define Tree_trace(x) PrintSyntaxTree(x, 1);

double  Parameter=0; // 参数 T

static Token token; // 全局变量，保存词法分析结果

// ----------------- 辅助函数
static void FetchToken ();											//取记号
static void MatchToken (enum Token_Type AToken);					//匹配记号

static void SyntaxError (int case_of);								//处理语法错误
static void ErrMsg(unsigned LineNo, char *descrip, char *string );	//显示错误信息

static void PrintSyntaxTree(struct ExprNode * root, int indent);	//打印语法树

// ----------------- 非终结符（语法单位）的递归子程
static void Program ();					//程序

static void Statement ();				//语句
static void OriginStatement ();			//原点设置语句
static void RotStatement ();			//旋转语句
static void ScaleStatement ();			//比例变换语句
static void ForStatement ();			//循环画点语句

static struct ExprNode *Expression ();	//表达式
static struct ExprNode *Term ();		//加减项
static struct ExprNode *Factor ();		//乘积项
static struct ExprNode *Component ();	//乘方项
static struct ExprNode *Atom ();		//基本项
struct ExprNode * funcchild();

extern void Parser(char *SrcFilePtr);		// 主函数

static struct ExprNode *MakeExprNode(enum Token_Type opcode, ...);	//语法树构造

// ------------------------ 调用词法分析器 GetToken 获取一个记号
static void FetchToken () {

	token = GetToken ();
	
	if (token.type == ERRTOKEN) SyntaxError(1);
}

// ------------------------ 匹配记号
static void MatchToken (enum Token_Type The_Token) {

	if (token.type != The_Token) SyntaxError (2);

	FetchToken();

}

// ------------------------ 语法错误处理
static void SyntaxError (int case_of) {

	switch(case_of) {
		case 1: 
			ErrMsg (LineNo, "错误记号", token.lexeme);
			break;
		case 2:
			ErrMsg (LineNo, "不是预期记号", token.lexeme);
			break;
	}
}

// ------------------------ 打印错误信息
void ErrMsg(unsigned LineNo, char *descrip, char *string ) {

	printf("行号 %5d:%s %s!\n", LineNo, descrip, string);

	

	CloseScanner();

	exit(1);	//出错则退出编译程序
}

// ------------------------ 先序遍历并打印表达式的语法树
void PrintSyntaxTree(struct ExprNode* root, int indent) {

	int temp;

	for (temp=1; temp<=indent; temp++) printf("\t"); // 用制表符缩进

	switch(root->OpCode) { // 打印根节点
		case PLUS: 
			printf("%s\n", "+"); break;
		case MINUS:
			printf("%s\n", "-"); break;
		case MUL:
			printf("%s\n", "*"); break;
		case DIV:
			printf("%s\n", "/"); break;
		case POWER:
			printf("%s\n", "**"); break;
		case FUNC:
			printf("%x\n", root->Content.CaseFunc.MathFuncPtr); break;
		case CONST_ID:
			printf("%f\n", root->Content.CaseConst); break;
		case T:
			printf("%s\n", "T" ); break;
		default:
			printf("错误的树节点!\n"); 
			exit(0);
	}

	if(root->OpCode == CONST_ID || root->OpCode == T) return; // 叶子节点返回
	
	if(root->OpCode == FUNC) 
		// 递归打印一个孩子的节点
		PrintSyntaxTree(root->Content.CaseFunc.Child, indent+1);
	else {
		// 递归打印两个孩子的节点
		PrintSyntaxTree(root->Content.CaseOperator.Left, indent+1);
		PrintSyntaxTree(root->Content.CaseOperator.Right, indent+1);
	}

}

// ----------------- 生成语法树节点
static struct ExprNode * MakeExprNode(enum Token_Type opcode, ...) {

	struct ExprNode *ExprPtr = new (struct ExprNode);
	va_list ArgPtr;

	ExprPtr->OpCode = opcode; // 接受记号的类别

	va_start(ArgPtr, opcode);

	switch(opcode) {	// 根据记号的类别构造不同的节点
		case CONST_ID: // 常数
			ExprPtr->Content.CaseConst = (double)va_arg(ArgPtr, double);
			break;

		case T:
			ExprPtr->Content.CaseParmPtr = &Parameter;
			break;

		case FUNC:
			ExprPtr->Content.CaseFunc.MathFuncPtr = (FuncPtr)va_arg(ArgPtr, FuncPtr);
			ExprPtr->Content.CaseFunc.Child = (struct ExprNode *) va_arg(ArgPtr, struct ExprNode *);
			break;

		default:
			ExprPtr->Content.CaseOperator.Left = (struct ExprNode *)va_arg(ArgPtr, struct ExprNode *);
			ExprPtr->Content.CaseOperator.Right = (struct ExprNode *)va_arg(ArgPtr, struct ExprNode *);
			break;
	}

	va_end(ArgPtr);

	return ExprPtr;
}

// ------------------------ 绘图语言解释器入口(与主程序的外边接口)
void Parser(char *SrcFilePtr) {

	// 初始化词法分析器，并打开源程序文件
	if(!InitScanner(SrcFilePtr)) {
		printf("源程序文件打开错误!\n");
		return;
	}

	FetchToken(); // 获取第一个记号

	Program(); // 递归下降分析

	CloseScanner(); // 关闭词法分析器

	return;
}

// ------------------------ Program 递归子程序
static void Program () {

	while (token.type != NONTOKEN) {
		Statement ();
		MatchToken (SEMICO);
	}

}

// ------------------------ Statement 的递归子程序
static void Statement() {

	switch (token.type) {
		case ORIGIN: 
			OriginStatement(); break;
		case SCALE: 
			ScaleStatement(); break;
		case ROT: 
			RotStatement(); break;
		case FOR: 
			ForStatement(); break;
		default: 
			SyntaxError(2);
	}

}

// ------------------------ OriginStatement 的递归子程序
static void OriginStatement (void) {

}

// ------------------------ ScaleStatement 的递归子程序
static void ScaleStatement (void) {

}

// ------------------------ RotStatement 的递归字程序
static void RotStatement (void) {

}

// ------------------------ For 递归子程序
static void ForStatement (void) {

struct ExprNode *start_ptr, *end_ptr, *step_ptr,*x_ptr,*y_ptr; // 各表达式语法树根节点指针
	
	MatchToken (FOR);	
	call_match("FOR");
	MatchToken(T); 
	call_match("T");
	MatchToken (FROM); 
	call_match("FROM");

	start_ptr = Expression(); //分析起点表达式

	MatchToken(TO);
	call_match("TO");

	end_ptr = Expression(); //分析终点表达式

	MatchToken (STEP);
	call_match("STEP");

	step_ptr = Expression(); // 分析步长表达式

	MatchToken (DRAW);
	call_match("DRAW");

	MatchToken (L_BRACKET);
	call_match("(");

	x_ptr = Expression(); //分析横坐标表达式

	MatchToken(COMMA);
	call_match(",");

	y_ptr = Expression(); //分析纵坐标表达式

	MatchToken (R_BRACKET);
	call_match(")");

}

// ------------------------- Expression 的递归子程序
//将运算符放进去的情况下得到一个完整的式子
static struct ExprNode* Expression() {

	struct ExprNode *left, *right; // 左右子树节点的指针
	Token_Type token_tmp; // 当前记号

	left = Term(); // 分析左操作数

	while (token.type == PLUS || token.type == MINUS || token.type == MUL || token.type == DIV) {

		token_tmp = token.type;

		MatchToken(token_tmp);	//

		right = Term();			//分析右操作数

		left = MakeExprNode(token_tmp, left, right);

	}

	Tree_trace(left); // 打印表达式语法树

	return left; // 返回表达式语法树指针
}

// ------------------------ Term 递归子程序
//取一个值封装成ExprNode并返回
static struct ExprNode *Term(){
	struct ExprNode *ExprPtr = new (struct ExprNode);  

	ExprPtr->OpCode = token.type; // 接受记号的类别 

	switch(token.type) {	// 根据记号的类别构造不同的节点
		case CONST_ID: // 常数
			ExprPtr->Content.CaseConst = token.value;
			break;

		case T:
			ExprPtr->Content.CaseParmPtr = &Parameter;
			break;

		case FUNC:
			ExprPtr->Content.CaseFunc.MathFuncPtr = token.FuncPtr;
			ExprPtr->Content.CaseFunc.Child = funcchild();//递归得到参数
			break;
	}
	
	MatchToken(token.type);

	return ExprPtr;
}

//递归的函数的参数
struct ExprNode * funcchild()
{

	struct ExprNode *left, *right; // 左右子树节点的指针

	MatchToken(token.type);
	MatchToken (L_BRACKET);

	Token_Type token_tmp; // 当前记号
 
	left = Term(); // 分析左操作数

	while (token.type == PLUS || token.type == MINUS || token.type == MUL || token.type == DIV) {

		token_tmp = token.type;

		MatchToken(token_tmp);	//

		right = Term();			//分析右操作数

		left = MakeExprNode(token_tmp, left, right);

	}

	return left;
}

// ------------------------ Factor 递归子程序
static struct ExprNode * Factor () {

}

// ------------------------ Component 递归子程序
static struct ExprNode* Component() {

}

// ------------------------ Atom 递归子程序
static struct ExprNode* Atom() {

}
