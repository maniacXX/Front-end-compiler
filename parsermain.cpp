
//-----------------parsermain.cpp--------------------
#include <stdio.h>

extern void Parser(char * SrcFilePtr);

void main(int argc, char *arg[]) {

	char fileName[100];

	printf("请输入源文件名!\n" );
	scanf("%s",&fileName);
	
	Parser(fileName);		//进行语法分析
}
