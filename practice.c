#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define K 24
#define STORAGE_FILEPATH 		"storage.txt"

static int systemSize[2] = {0, 0};  		//row/column of the delivery system

int file (char* filepath){
	
	int i;
	int row, column, buliding, room;
	char passwd[5];
	char *context[100];
	char MASTERPASSWD[5];
	
	FILE *fp;
	
	fp = fopen(filepath, "r");
	
	if (fp == NULL)
		printf("file wrong");
	
	fscanf(fp, "%d %d", &systemSize[0], &systemSize[1]);
	
	struct storage_t **deliverySystem;
	
	deliverySystem = (struct storage_t**)malloc(systemSize[0] * sizeof (struct storage_t*));
	for(i=0; i<systemSize[0]; i++)
		deliverySystem[i] = (struct storage_t*)malloc(systemSize[1] * sizeof (struct storage_t));
	
	
	fscanf(fp, "%s", MASTERPASSWD);
	// for 내용물이 없을 때 까지 
	//while ( c = fgetc(fp) != EOF) 
	for (i=0; i<K; i++)
		fscanf(fp, "%d %d %d %d %s %s", &row, &column, &deliverySystem[row,column].building, &deliverySystem[row,column].room, deliverySystem[row,column].passwd, deliverySystem[row,column].context);

	printf("%d %d\n",N,M );
	printf("%s\n", MASTERPASSWD);
	
	// 첫 번째 택배항목부터, K번째 택배 항목까지. 
	for (i=0; i<K; i++) 
		printf("%d %d %d %d %s %s\n", row, column, deliverySystem[row,column].building, deliverySystem[row,column].room, deliverySystem[row,column].passwd, deliverySystem[row,column].context);
		
	fclose(fp);
	
	return;
}


void main(void) {
		
	int i;
	int N,M;
	int row, column, buliding, room;
	char passwd[5];
	char *context[100];
	char MASTERPASSWD[5];
	FILE *fp;
	
	file (STORAGE_FILEPATH);

	
	/*
	printf("%d %d\n",N,M );
	printf("%s\n", MASTERPASSWD);
	
	// 첫 번째 택배항목부터, K번째 택배 항목까지. 
	for (i=0; i<K; i++) 
		printf("%d %d %d %d %s %s", row, column, buliding, room, passwd, context);
	
	*/
	
	return;
}
