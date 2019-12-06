#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #define STORAGE_FILEPATH 		"storage.txt"

static int N, M;
/*
int file (char* filepath){
	
	int N,M;
	FILE *fp;
	
	fp = fopen(char* filepath, "r");
	
	if (fp == NULL)
		printf("file wrong");
	
	fscanf(fp, "%d %d", &N, &M);
	
	fclose(fp);
	
	return;
}
*/

void main(void) {
	
	//file (STORAGE_FILEPATH);
	
	int N,M;
	int row, column, buliding, room;
	char passwd[5];
	char *context[100];
	char MASTERPASSWD[5];
	FILE *fp;
	
	char c;
	
	fp = fopen("storage.txt", "r");
	
	if (fp == NULL)
		printf("file wrong");
	
	fscanf(fp, "%d %d", &N, &M);
	fscanf(fp, "%s", MASTERPASSWD);
	// for 내용물이 없을 때 까지 
	while ( c = fgetc(fp) != EOF) 
	{
	fscanf(fp, "%d %d %d %d %s %s", &row, &column, &buliding, &room, passwd, context);
	}
	fclose(fp);
	
	printf("%d %d\n",N,M );
	printf("%s\n", MASTERPASSWD);
	printf("%d %d %d %d %s %s", row, column, buliding, room, passwd, context);
	
	return;
}
