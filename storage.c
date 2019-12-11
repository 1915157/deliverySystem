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

extern int getIntegerInput(void);

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
	
	deliverySystem[x][y].building = 0;
	deliverySystem[x][y].cnt = 0;
	deliverySystem[x][y].context = 0;
	deliverySystem[x][y].room = 0;
	deliverySystem[x][y].passwd[0] = 0;
	
	/*
	// allocate memotry to the context pointer
	char *context;
	context = (char *)malloc(sizeof(char)); // 택배를 넣을 때 혹은 설정파일에서 읽어올 때 알게된 길이를 곱해야함. 
	if (context == NULL)
	{
		printf("wrong memory allocation");
		exit(1);
	}
	
		free(context);
	*/
	
	/*
	for(i=0;i<systemSize[0];i++) 
	{
		for(j=0;j<systemSize[1];j++)
			deliverySystem[i][j].context = (char *)malloc(100 * sizeof(char));
	} 
	*/
	
	
}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y) {
	
	char getPasswd[PASSWD_LEN+1];
	
	// 비밀번호 치라고 메세지 
	getPasswd[0] = 0;
	printf("input password for (%d, %d) storage : \n", x, y);
	// 비밀번호를 받음
	scanf("%4s", getPasswd);
	fflush(stdin);
	
	// 받은 비밀번호를 해당 좌표의 비밀번호와 비교
	 // 맞으면 0 반환
	 // 틀리면 -1 반환
	if (strcmp(getPasswd,deliverySystem[x][y].passwd) == 0 || strcmp(getPasswd,masterPassword) == 0)
		return 0;
	else
		return -1; 
	
}





// ------- API function for main.c file ---------------

// 보관 상태가 변경될 때마다 설정파일에 새로 write(입력)함. 
//backup the delivery system context to the file system
//char* filepath : filepath and name to write
//return : 0 - backup was successfully done, -1 - failed to backup
int str_backupSystem(char* filepath) {
	
	int x, y;
	
	FILE *fp = NULL;
	
	fp = fopen(filepath, "w");
	 
	// file open
	// write 변경사항 in the file.
	
	fprintf("%d %d\n", systemSize[0], systemSize[1]);
	fprintf("%s\n", masterPassword);
	
	for(x=0; x<systemSize[0]; x++)
	{
		for (y=0; y<systemSize[1]; y++)
		{
			if (deliverySystem[x][y].cnt > 0)
			{
				fprintf("%d %d %d %d %s %s", &x, &y, &deliverySystem[x][y].building, &deliverySystem[x][y].room, deliverySystem[x][y].passwd, deliverySystem[x][y].context);
			}
		}
	}
	
	// file close
	fclose(fp); 
	
}


// initStorage함수 이용.  
//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {
	

	// file open
	FILE *fp = NULL;
	// int buliding, row, column, room, cnt;
	int N,M;
	int row, column; 
	char passwd[PASSWD_LEN+1];
	char *context;
	 
	fp = fopen(filepath, "r");
	if (fp == NULL)
	 printf("can't open the file");
	
	
	
	//file read - file 값 불러오기.
	
	 //첫번째 줄에서 받은 2개의 숫자 값 = N(systemSize[0]), M(systemSize[1])으로 설정. 
	fscanf(fp, "%d %d", &systemSize[0], &systemSize[1]);
	fscanf(fp, "%s",  masterPassword);
	
	// create delivery system on the double pointer deliverySystem
	int i;
	//struct storage_t **deliverySystem;
	
	deliverySystem = (storage_t**)malloc(systemSize[0] * sizeof (storage_t*));
	for(i=0; i<systemSize[0]; i++)
		deliverySystem[i] = (storage_t*)malloc(systemSize[1] * sizeof (storage_t));
		
	for(N=0;N<systemSize[0];N++) 
	{
		for(M=0;M<systemSize[1];M++)
			deliverySystem[N][M].context = (char *)malloc(10 * sizeof(char));
	} 
	
	// (x,y)에 택배가 존재한다면
	 // initStorage 초기화 = 동,룸번호, 내용을 등 매치.
	while( feof(fp) != 0 ) // 파일끝까지 ------------------------------------> 수정필요 
	{
		// 한 열에 대해서만 받을 수도 있음 (row, column때문에) 
		fscanf(fp, "%d %d %d %d %s %s", &row, &column, &deliverySystem[row][column].building, &deliverySystem[row][column].room, deliverySystem[row][column].passwd, deliverySystem[row][column].context);
		deliverySystem[row][column].cnt++; 
		storedCnt++;
	}
	// x,y의 cnt = 0 이면 , 그 해당의 좌표 storage를 초기화. 
	for(N=0; N<systemSize[0]; N++)
	{
		for(M=0; M<systemSize[1]; M++)
		{
			if (deliverySystem[N][M].cnt == 0)
				initStorage(N,M);
		}
	} 
	 	
	// file close	
	fclose(fp); 
	
}

//free the memory of the deliverySystem 
void str_freeSystem(void) {
	
	int i;
	
	for (i=0; i<systemSize[0]; i++)
		free (deliverySystem[i]);
	free(deliverySystem);	
	
	for (i=0; i<systemSize[0]; i++)
		free(deliverySystem[i].context);
	free(deliverySystem.context);
	

}



//print the current state of the whole delivery system (which cells are occupied and the destination of the each occupied cells)
void str_printStorageStatus(void) {
	int i, j;
	printf("----------------------------- Delivery Storage System Status (%i occupied out of %i )-----------------------------\n\n", storedCnt, systemSize[0]*systemSize[1]);
	
	printf("\t");
	for (j=0;j<systemSize[1]; j++)
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

// 받은 변수들을 파일에 입력
// 파일에 잘 입력했으면 0, 입력x되면 -1을 반환. 
if (str_checkStorage(x,y) != 0 || buildingValidityCheck(nBuilding, nRoom) != 0)
	return -1;
else 
{
	deliverySystem[x][y].cnt++;
	storedCnt++;
	return 0;
 } 
// storedCnt++;
	
}



//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {
	
	inputPasswd(x,y);
	if (inputPasswd(x,y) == -1)
		return -1;
	 // passwd가 틀리면 (inputPasswd = -1이면)
	  //-1 반환 (failed to extract)
	
	else if (inputPasswd(x,y) == 0)
	{
		initStorage(x,y);
		deliverySystem[x][y].cnt--;
		storedCnt--;
		printf("-------> extracting the storage (%d, %d)\n", x, y);
		return 0;
	}
	 //passwd가 맞으면 (inputPasswd = 0이면)
	 // 해당 좌표의 storage를 비움. (initStorage(x,y); )
	 // storedCnt--; (1 감소시킴) 
	 // printf(" ---------> extracting the storage(%d, %d) , x , y);
	 // 0을 반환   
}

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) {
	
	int cnt = 0;	
	// 만약 txt파일에 그 동과 호수에 택배가 없으면 0을 반환
	int x,y;
	
	for(x=0; x<systemSize[0]; x++)
	{
		for(y=0; y<systemSize[1]; y++)
		{
			if (deliverySystem[x][y].building == nBuilding && deliverySystem[x][y].room == nRoom)
			{
				printf("---------------------> Found a package in (%d,%d)\n", x, y);
				cnt++;
			}
		}	
	} 
	// 만약 그 동과 호수에 택배가 있으면
	 // 그 동과 호수에 해당되는 모든 storage를 출력
	 // "---------------> Found a package in (x,y)" 
	 // (위에것.. 있는 개수만큼) 
	  
	return cnt;
}
