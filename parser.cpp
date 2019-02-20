
// ----------------------------- parser.cpp --------------------------------
//201631061113 �����
#include "parser.h"

#define call_match(x) printf("ƥ��Ǻţ�"); printf(x), printf("\n")

#define Tree_trace(x) PrintSyntaxTree(x, 1);

double  Parameter=0; // ���� T

static Token token; // ȫ�ֱ���������ʷ��������

// ----------------- ��������
static void FetchToken ();											//ȡ�Ǻ�
static void MatchToken (enum Token_Type AToken);					//ƥ��Ǻ�

static void SyntaxError (int case_of);								//�����﷨����
static void ErrMsg(unsigned LineNo, char *descrip, char *string );	//��ʾ������Ϣ

static void PrintSyntaxTree(struct ExprNode * root, int indent);	//��ӡ�﷨��

// ----------------- ���ս�����﷨��λ���ĵݹ��ӳ�
static void Program ();					//����

static void Statement ();				//���
static void OriginStatement ();			//ԭ���������
static void RotStatement ();			//��ת���
static void ScaleStatement ();			//�����任���
static void ForStatement ();			//ѭ���������

static struct ExprNode *Expression ();	//���ʽ
static struct ExprNode *Term ();		//�Ӽ���
static struct ExprNode *Factor ();		//�˻���
static struct ExprNode *Component ();	//�˷���
static struct ExprNode *Atom ();		//������
struct ExprNode * funcchild();

extern void Parser(char *SrcFilePtr);		// ������

static struct ExprNode *MakeExprNode(enum Token_Type opcode, ...);	//�﷨������

// ------------------------ ���ôʷ������� GetToken ��ȡһ���Ǻ�
static void FetchToken () {

	token = GetToken ();
	
	if (token.type == ERRTOKEN) SyntaxError(1);
}

// ------------------------ ƥ��Ǻ�
static void MatchToken (enum Token_Type The_Token) {

	if (token.type != The_Token) SyntaxError (2);

	FetchToken();

}

// ------------------------ �﷨������
static void SyntaxError (int case_of) {

	switch(case_of) {
		case 1: 
			ErrMsg (LineNo, "����Ǻ�", token.lexeme);
			break;
		case 2:
			ErrMsg (LineNo, "����Ԥ�ڼǺ�", token.lexeme);
			break;
	}
}

// ------------------------ ��ӡ������Ϣ
void ErrMsg(unsigned LineNo, char *descrip, char *string ) {

	printf("�к� %5d:%s %s!\n", LineNo, descrip, string);

	

	CloseScanner();

	exit(1);	//�������˳��������
}

// ------------------------ �����������ӡ���ʽ���﷨��
void PrintSyntaxTree(struct ExprNode* root, int indent) {

	int temp;

	for (temp=1; temp<=indent; temp++) printf("\t"); // ���Ʊ������

	switch(root->OpCode) { // ��ӡ���ڵ�
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
			printf("��������ڵ�!\n"); 
			exit(0);
	}

	if(root->OpCode == CONST_ID || root->OpCode == T) return; // Ҷ�ӽڵ㷵��
	
	if(root->OpCode == FUNC) 
		// �ݹ��ӡһ�����ӵĽڵ�
		PrintSyntaxTree(root->Content.CaseFunc.Child, indent+1);
	else {
		// �ݹ��ӡ�������ӵĽڵ�
		PrintSyntaxTree(root->Content.CaseOperator.Left, indent+1);
		PrintSyntaxTree(root->Content.CaseOperator.Right, indent+1);
	}

}

// ----------------- �����﷨���ڵ�
static struct ExprNode * MakeExprNode(enum Token_Type opcode, ...) {

	struct ExprNode *ExprPtr = new (struct ExprNode);
	va_list ArgPtr;

	ExprPtr->OpCode = opcode; // ���ܼǺŵ����

	va_start(ArgPtr, opcode);

	switch(opcode) {	// ���ݼǺŵ�����첻ͬ�Ľڵ�
		case CONST_ID: // ����
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

// ------------------------ ��ͼ���Խ��������(�����������߽ӿ�)
void Parser(char *SrcFilePtr) {

	// ��ʼ���ʷ�������������Դ�����ļ�
	if(!InitScanner(SrcFilePtr)) {
		printf("Դ�����ļ��򿪴���!\n");
		return;
	}

	FetchToken(); // ��ȡ��һ���Ǻ�

	Program(); // �ݹ��½�����

	CloseScanner(); // �رմʷ�������

	return;
}

// ------------------------ Program �ݹ��ӳ���
static void Program () {

	while (token.type != NONTOKEN) {
		Statement ();
		MatchToken (SEMICO);
	}

}

// ------------------------ Statement �ĵݹ��ӳ���
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

// ------------------------ OriginStatement �ĵݹ��ӳ���
static void OriginStatement (void) {

}

// ------------------------ ScaleStatement �ĵݹ��ӳ���
static void ScaleStatement (void) {

}

// ------------------------ RotStatement �ĵݹ��ֳ���
static void RotStatement (void) {

}

// ------------------------ For �ݹ��ӳ���
static void ForStatement (void) {

struct ExprNode *start_ptr, *end_ptr, *step_ptr,*x_ptr,*y_ptr; // �����ʽ�﷨�����ڵ�ָ��
	
	MatchToken (FOR);	
	call_match("FOR");
	MatchToken(T); 
	call_match("T");
	MatchToken (FROM); 
	call_match("FROM");

	start_ptr = Expression(); //���������ʽ

	MatchToken(TO);
	call_match("TO");

	end_ptr = Expression(); //�����յ���ʽ

	MatchToken (STEP);
	call_match("STEP");

	step_ptr = Expression(); // �����������ʽ

	MatchToken (DRAW);
	call_match("DRAW");

	MatchToken (L_BRACKET);
	call_match("(");

	x_ptr = Expression(); //������������ʽ

	MatchToken(COMMA);
	call_match(",");

	y_ptr = Expression(); //������������ʽ

	MatchToken (R_BRACKET);
	call_match(")");

}

// ------------------------- Expression �ĵݹ��ӳ���
//��������Ž�ȥ������µõ�һ��������ʽ��
static struct ExprNode* Expression() {

	struct ExprNode *left, *right; // ���������ڵ��ָ��
	Token_Type token_tmp; // ��ǰ�Ǻ�

	left = Term(); // �����������

	while (token.type == PLUS || token.type == MINUS || token.type == MUL || token.type == DIV) {

		token_tmp = token.type;

		MatchToken(token_tmp);	//

		right = Term();			//�����Ҳ�����

		left = MakeExprNode(token_tmp, left, right);

	}

	Tree_trace(left); // ��ӡ���ʽ�﷨��

	return left; // ���ر��ʽ�﷨��ָ��
}

// ------------------------ Term �ݹ��ӳ���
//ȡһ��ֵ��װ��ExprNode������
static struct ExprNode *Term(){
	struct ExprNode *ExprPtr = new (struct ExprNode);  

	ExprPtr->OpCode = token.type; // ���ܼǺŵ���� 

	switch(token.type) {	// ���ݼǺŵ�����첻ͬ�Ľڵ�
		case CONST_ID: // ����
			ExprPtr->Content.CaseConst = token.value;
			break;

		case T:
			ExprPtr->Content.CaseParmPtr = &Parameter;
			break;

		case FUNC:
			ExprPtr->Content.CaseFunc.MathFuncPtr = token.FuncPtr;
			ExprPtr->Content.CaseFunc.Child = funcchild();//�ݹ�õ�����
			break;
	}
	
	MatchToken(token.type);

	return ExprPtr;
}

//�ݹ�ĺ����Ĳ���
struct ExprNode * funcchild()
{

	struct ExprNode *left, *right; // ���������ڵ��ָ��

	MatchToken(token.type);
	MatchToken (L_BRACKET);

	Token_Type token_tmp; // ��ǰ�Ǻ�
 
	left = Term(); // �����������

	while (token.type == PLUS || token.type == MINUS || token.type == MUL || token.type == DIV) {

		token_tmp = token.type;

		MatchToken(token_tmp);	//

		right = Term();			//�����Ҳ�����

		left = MakeExprNode(token_tmp, left, right);

	}

	return left;
}

// ------------------------ Factor �ݹ��ӳ���
static struct ExprNode * Factor () {

}

// ------------------------ Component �ݹ��ӳ���
static struct ExprNode* Component() {

}

// ------------------------ Atom �ݹ��ӳ���
static struct ExprNode* Atom() {

}
