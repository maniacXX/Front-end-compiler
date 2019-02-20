
// ----------------------------- scanner.cpp --------------------------------

#include "scanner.h"

#define TOKEN_LEN 100 			// ���ʵ���󳤶�

static char TokenBuffer[TOKEN_LEN];	// ���ʵ��ַ�����

static FILE *InFile; // �����ļ���

unsigned int LineNo; // ����Դ�ļ��к�


// ----------------- ��ʼ���ʷ�������
extern int InitScanner (const char *FileName) { 

	LineNo = 1;

	InFile = fopen (FileName, "r");	//ֻ����

	if (InFile != NULL) 
		return 1;	//�ɹ�
	else 
		return 0;
}

// ----------------- �رմʷ�������
extern void CloseScanner (void) { 

	if (InFile != NULL) fclose (InFile);

}

// ----------------- ������Դ�����ļ����ж���һ���ַ�
static char GetChar(void) { 

	int Char;

	Char = getc (InFile);	// ��һ���ַ�

	return toupper(Char);	// ת��Ϊ��д
}

// ----------------- ��Ԥ�����ַ��˻ص�����Դ������ļ�����
static void BackChar(char Char) { 

	if (Char != EOF) ungetc (Char, InFile);

}

// ----------------- �����ַ����ǺŻ�����
static void AddCharTokenString (char Char) { 

int TokenLength;

	TokenLength = strlen (TokenBuffer);	//���ص�ǰʵ�ʴ�����

	if (TokenLength + 1 >= sizeof (TokenBuffer)) return;	//���ܳ�����󳤶ȣ�100��

	TokenBuffer[TokenLength] = Char;
	TokenBuffer[TokenLength+ 1] = '\0';
}

// ----------------- ��ռǺŻ�����
static void EmptyTokenString () { 

	memset(TokenBuffer, 0, TOKEN_LEN);	//������ȫ����Ϊ 0

}

int find(const char * IDString){
	int loop=-1,count=0;
	char back[100];//�����˻��ַ�
	char c;

	do{
		c=GetChar();
		back[count++]=c;

		AddCharTokenString(c);
		
		int i,tablen;
		tablen = sizeof (TokenTab) / sizeof(TokenTab[0]);	//Ԥ���嵥�ʱ���Ŀ��

		for (i = 0; i< tablen ; i ++) {
			if (strcmp (TokenTab[i].lexeme, IDString) == 0) 
			{
				int realisticLoop=find(TokenBuffer);//�ݹ�Ѱ������ƥ����
				if(realisticLoop==-1)
					return i;
				else
					return realisticLoop;
			}
		}

	}while(c==32);//�����ո�

	//���εݹ�û���ҵ�ƥ���������������˸��ļ���
	count--;
	for(;count>=0;count--){
		BackChar(back[count]);
	}

	return loop;//�����ո�Ҳ�Ҳ����µ�ƥ����ͷ���-1��ʾ�Ҳ�����
}

// -----------------�ж��ַ����Ƿ���Ԥ���嵥�ʱ���
static Token JudgeKeyToken(const char * IDString) { 

	int loop,tablen;
	Token errortoken;
	
	tablen = sizeof (TokenTab) / sizeof(TokenTab[0]);	//Ԥ���嵥�ʱ���Ŀ��

	for (loop = 0; loop < tablen ; loop ++) {
		if (strcmp (TokenTab[loop].lexeme, IDString) == 0) 
		{
			//��������������Ҫ��鵱ǰ�ַ����Ƿ�������Ҫ��ȡ�ַ������Ӵ�
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

// ----------------- ��ȡһ���Ǻ�
//201631061113 �����
extern Token GetToken (void) {

	Token token;
	char c;
	while((c=GetChar())==32);
	
	if(c==EOF){
		token.type=NONTOKEN;
		return token;
	}

	if(c<='9'&&c>='0')//����ɸѡ
	{
		char c1;
		int div=10;
		double result=(double)((int)(c-'0'));
		while(1)
		{
			c1=GetChar();
			if(c1<='9'&&c1>='0'){//��������
				result*=10;
				result+=(double)((int)(c1-'0'));
			}else if(c1=='.'){//С������
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

	//�����,�����ţ�����ɸѡ
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


	BackChar(c);//Ԥ�����ַ�����
	
	do{
		while((c=GetChar())==32);//�����ո�

		AddCharTokenString(c);
		token=JudgeKeyToken(TokenBuffer);
	}while(token.type == ERRTOKEN);
	
	EmptyTokenString();
	return token;
} // end of GetTOken
