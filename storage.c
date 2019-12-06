#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"


/* 
  definition of storage cell structure ----
  members :
  int building : building number of the destination
  int room : room number of the destination
  int cnt : number of packages in the cell
  char passwd[] : password setting (4 characters)
  char *contents : package context (message string)
*/
typedef struct {
	int building;
	int room; // room은 n*100 + m으로 표기 (n:열,층  m:column, 호수. ) 
	int cnt;
	char passwd[PASSWD_LEN+1];
	
	char *context;
} storage_t;


static storage_t** deliverySystem; 			//deliverySystem
static int storedCnt = 0;					//number of cells occupied
static int systemSize[2] = {0, 0};  		//row/column of the delivery system
static char masterPassword[PASSWD_LEN+1];	//master password




// ------- inner functions ---------------

//print the inside context of a specific cell
//int x, int y : cell to print the context
static void printStorageInside(int x, int y) {
	printf("\n------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n");
	if (deliverySystem[x][y].cnt > 0)
		printf("<<<<<<<<<<<<<<<<<<<<<<<< : %s >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", deliverySystem[x][y].context);
	else
		printf("<<<<<<<<<<<<<<<<<<<<<<<< empty >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		
	printf("------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n\n");
}

// 택배 꺼낼 때 혹은 처음에 보관함 생성 때 쓰도록 의도. 
//initialize the storage
//set all the member variable as an initial value
//and allocate memory to the context pointer
//int x, int y : cell coordinate to be initialized (초기화를 하고 싶은 storage의 좌표) 
static void initStorage(int x, int y) {
	
	// initialize the storage
	 // set all the memver variable as an initial value.
	struct storage_t; 
	
	// allocate memotry to the context pointer
	char *context;
	context = (char *)malloc(sizeof(char)); 
	if (context == NULL){
		printf("wrong memory allocation");
		exit(1);
	}
	
	free(context);
		
	
}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y) {
	
}





// ------- API function for main.c file ---------------

// 보관 상태가 변경될 때마다 설정파일에 새로 write(입력)함. 
//backup the delivery system context to the file system
//char* filepath : filepath and name to write
//return : 0 - backup was successfully done, -1 - failed to backup
int str_backupSystem(char* filepath) {
	FILE *fp = NULL;
	
	// file open
	// write 변경사항 in the file.
	 //  넣었으면 파일에 추가.
	 // 꺼냈으면 파일에서 삭제.
	// file close 
	
}


// initStorage함수 이용.  
//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {
	

	//  filepath and name to read config parameters
	// file open
	FILE *fp = NULL;
	int buliding, row, column, room, cnt;
	int N,M;
	char passwd[PASSWD_LEN+1];
	char *context;
	 
	fp = open("char* filepath", "r"); // 이 부분 수정 필요 
	if (fp == NULL)
	 printf("can't open the file");
	
	
	
	//file read - file 값 불러오기.
	
	 //첫번째 줄에서 받은 2개의 숫자 값 = N, M으로 설정. 
	fscanf(fp, "%d %d", &N, &M);
	
	//while ( /* 문자*/  = fgets(fp) != EOF)
	//	putchar();
	
	// (x,y)에 택배가 존재한다면
	 // initStorage 초기화 = 동,룸번호, 내용을 등 매치.
	 	
	
	// create delivery system on the double pointer deliverySystem
	int i;
	struct storage_t **deliverySystem;
	
	deliverySystem = (struct storage_t**)malloc(4 * sizeof (struct storage_t*));
	for(i=0; i<3; i++)
		deliverySystem[i] = (struct storage_t*)malloc(6 * sizeof (struct storage_t));
	
	 	
	// file close	
	fclose(fp); 
	
}

//free the memory of the deliverySystem 
void str_freeSystem(void) {
	
	int i;
	for (i=0; i<4; i++)
		free (deliverySystem[i]);
	free(deliverySystem);	
	
}



//print the current state of the whole delivery system (which cells are occupied and the destination of the each occupied cells)
void str_printStorageStatus(void) {
	int i, j;
	printf("----------------------------- Delivery Storage System Status (%i occupied out of %i )-----------------------------\n\n", storedCnt, systemSize[0]*systemSize[1]);
	
	printf("\t");
	for (j=0;j<systemSize[1];j++)
	{
		printf(" %i\t\t",j);
	}
	printf("\n-----------------------------------------------------------------------------------------------------------------\n");
	
	for (i=0;i<systemSize[0];i++)
	{
		printf("%i|\t",i);
		for (j=0;j<systemSize[1];j++)
		{
			if (deliverySystem[i][j].cnt > 0)
			{
				printf("%i,%i\t|\t", deliverySystem[i][j].building, deliverySystem[i][j].room);
			}
			else
			{
				printf(" -  \t|\t");
			}
		}
		printf("\n");
	}
	printf("--------------------------------------- Delivery Storage System Status --------------------------------------------\n\n");
}



//check if the input cell (x,y) is valid and whether it is occupied or not
int str_checkStorage(int x, int y) {
	if (x < 0 || x >= systemSize[0])
	{
		return -1;
	}
	
	if (y < 0 || y >= systemSize[1])
	{
		return -1;
	}
	
	return deliverySystem[x][y].cnt;	
}


//put a package (msg) to the cell
//input parameters
//int x, int y : coordinate of the cell to put the package
//int nBuilding, int nRoom : building and room numbers of the destination
//char msg[] : package context (message string)
//char passwd[] : password string (4 characters)
//return : 0 - successfully put the package, -1 - failed to put
int str_pushToStorage(int x, int y, int nBuilding, int nRoom, char msg[MAX_MSG_SIZE+1], char passwd[PASSWD_LEN+1]) {
	
}



//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {
	
}

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) {
	
	return cnt;
}
