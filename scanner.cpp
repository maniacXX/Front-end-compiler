
// ----------------------------- scanner.cpp --------------------------------

#include "scanner.h"

#define TOKEN_LEN 100 			// 单词的最大长度

static char TokenBuffer[TOKEN_LEN];	// 单词的字符缓冲

static FILE *InFile; // 输入文件流

unsigned int LineNo; // 跟踪源文件行号


// ----------------- 初始化词法分析器
extern int InitScanner (const char *FileName) { 

	LineNo = 1;

	InFile = fopen (FileName, "r");	//只读打开

	if (InFile != NULL) 
		return 1;	//成功
	else 
		return 0;
}

// ----------------- 关闭词法分析器
extern void CloseScanner (void) { 

	if (InFile != NULL) fclose (InFile);

}

// ----------------- 从输入源程序文件流中读入一个字符
static char GetChar(void) { 

	int Char;

	Char = getc (InFile);	// 读一个字符

	return toupper(Char);	// 转换为大写
}

// ----------------- 把预读的字符退回到输入源程序的文件流中
static void BackChar(char Char) { 

	if (Char != EOF) ungetc (Char, InFile);

}

// ----------------- 加入字符到记号缓冲区
static void AddCharTokenString (char Char) { 

int TokenLength;

	TokenLength = strlen (TokenBuffer);	//返回当前实际串长度

	if (TokenLength + 1 >= sizeof (TokenBuffer)) return;	//不能超过最大长度（100）

	TokenBuffer[TokenLength] = Char;
	TokenBuffer[TokenLength+ 1] = '\0';
}

// ----------------- 清空记号缓冲区
static void EmptyTokenString () { 

	memset(TokenBuffer, 0, TOKEN_LEN);	//缓冲区全部置为 0

}

int find(const char * IDString){
	int loop=-1,count=0;
	char back[100];//用于退回字符
	char c;

	do{
		c=GetChar();
		back[count++]=c;

		AddCharTokenString(c);
		
		int i,tablen;
		tablen = sizeof (TokenTab) / sizeof(TokenTab[0]);	//预定义单词表项目数

		for (i = 0; i< tablen ; i ++) {
			if (strcmp (TokenTab[i].lexeme, IDString) == 0) 
			{
				int realisticLoop=find(TokenBuffer);//递归寻找最终匹配项
				if(realisticLoop==-1)
					return i;
				else
					return realisticLoop;
			}
		}

	}while(c==32);//跳开空格

	//本次递归没有找到匹配项，将多读的数据退给文件流
	count--;
	for(;count>=0;count--){
		BackChar(back[count]);
	}

	return loop;//读到空格也找不到新的匹配项就返回-1表示找不到了
}

// -----------------判断字符串是否在预定义单词表中
static Token JudgeKeyToken(const char * IDString) { 

	int loop,tablen;
	Token errortoken;
	
	tablen = sizeof (TokenTab) / sizeof(TokenTab[0]);	//预定义单词表项目数

	for (loop = 0; loop < tablen ; loop ++) {
		if (strcmp (TokenTab[loop].lexeme, IDString) == 0) 
		{
			//避免出现误读，需要检查当前字符串是否是真正要读取字符串的子串
			int realisticLoop=find(TokenBuffer);
			if(realisticLoop==-1)
				return TokenTab[loop];
			else
				return TokenTab[realisticLoop];
		}
	}

	memset(&errortoken, 0, sizeof(Token));

	errortoken.type = ERRTOKEN;

	return errortoken;
}

// ----------------- 获取一个记号
//201631061113 徐高瑞
extern Token GetToken (void) {

	Token token;
	char c;
	while((c=GetChar())==32);
	
	if(c==EOF){
		token.type=NONTOKEN;
		return token;
	}

	if(c<='9'&&c>='0')//常数筛选
	{
		char c1;
		int div=10;
		double result=(double)((int)(c-'0'));
		while(1)
		{
			c1=GetChar();
			if(c1<='9'&&c1>='0'){//整数部分
				result*=10;
				result+=(double)((int)(c1-'0'));
			}else if(c1=='.'){//小数部分
				while(1)
				{
					c1=GetChar();
					if(c1<='9'&&c1>='0'){
						result+=((double)((int)(c1-'0')))/div;
						div*=10;
					}else{
						BackChar(c1);
						token.FuncPtr=NULL;
						token.lexeme=(char *)malloc(10*sizeof(char));
						strcpy(token.lexeme,"CONST_NUM");
						token.value=result;
						token.type=CONST_ID;
						return token;
					}
				}
			}else{
				BackChar(c1);
				token.FuncPtr=NULL;
				token.lexeme=(char *)malloc(10*sizeof(char));
				strcpy(token.lexeme,"CONST_NUM");
				token.value=result;
				token.type=CONST_ID;
				return token;
			}
		}
	}

	//运算符,标点符号，括号筛选
	if(c=='+'||c=='-'||c=='*'||c=='/'||c=='('||c==')'||c==','||c==';')
	{
		token.lexeme=(char *)malloc(20*sizeof(char));
		if(c=='+')
		{	
			strcpy(token.lexeme,"OPERATOR_PLUS");
			token.type=PLUS;
		}
		else if(c=='-')
		{
			strcpy(token.lexeme,"OPERATOR_MINUS");
			token.type=MINUS;
		}
		else if(c=='*')
		{
			strcpy(token.lexeme,"OPERATOR_MUL");
			token.type=MUL;
		}
		else if(c=='/')
		{
			strcpy(token.lexeme,"OPERATOR_DIV");
			token.type=DIV;
		}
		else if(c=='(')
		{
			strcpy(token.lexeme,"L_BRACKET");
			token.type=L_BRACKET;
		}
		else if(c==')')
		{
			strcpy(token.lexeme,"R_BRACKET");
			token.type=R_BRACKET;
		}
		else if(c==',')
		{
			strcpy(token.lexeme,"COMMA");
			token.type=COMMA;
		}
		else if(c==';')
		{
			strcpy(token.lexeme,"SEMICO");
			token.type=SEMICO;
		}

		token.FuncPtr=NULL;
		token.value=0;
		return token;
	}


	BackChar(c);//预读入字符回退
	
	do{
		while((c=GetChar())==32);//跳开空格

		AddCharTokenString(c);
		token=JudgeKeyToken(TokenBuffer);
	}while(token.type == ERRTOKEN);
	
	EmptyTokenString();
	return token;
} // end of GetTOken
