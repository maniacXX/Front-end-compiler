
//-----------------parsermain.cpp--------------------
#include <stdio.h>

extern void Parser(char * SrcFilePtr);

void main(int argc, char *arg[]) {

	char fileName[100];

	printf("������Դ�ļ���!\n" );
	scanf("%s",&fileName);
	
	Parser(fileName);		//�����﷨����
}
