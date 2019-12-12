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
	int room; 
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

//initialize the storage
//set all the member variable as an initial value
//and allocate memory to the context pointer
//int x, int y : cell coordinate to be initialized 
static void initStorage(int x, int y) {
	
	// initialize the storage - set all the member variable as an initial value.
	deliverySystem[x][y].building = 0;
	deliverySystem[x][y].cnt = 0;
	deliverySystem[x][y].context = 0;
	deliverySystem[x][y].room = 0;
	deliverySystem[x][y].passwd[0] = 0;
	
}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y) {
	
	char getPasswd[PASSWD_LEN+1]; // variable that gets passwd of that coordinate 
	getPasswd[0] = 0;
	printf("input password for (%d, %d) storage : \n", x, y);	// message to user to get password for (x,y)
	scanf("%4s", getPasswd);
	fflush(stdin);
	
	//compare getpasswd with password for that coordinate ( wrong - return -1, success - return 0)
	if (strcmp(getPasswd,deliverySystem[x][y].passwd) == 0 || strcmp(getPasswd,masterPassword) == 0)
		return 0;
	else
		return -1; 
	
}





// ------- API function for main.c file ---------------
 
//backup the delivery system context to the file system
//char* filepath : filepath and name to write
//return : 0 - backup was successfully done, -1 - failed to backup
int str_backupSystem(char* filepath) {
	
	int x, y; // coordinate to repeat
	
	FILE *fp = NULL;
	
	//file open to write 
	fp = fopen(filepath, "w");
	 
	// write all of things in the file.
	fprintf("%d %d\n", systemSize[0], systemSize[1]);
	fprintf("%s\n", masterPassword);
	
	for(x=0; x<systemSize[0]; x++)
	{
		for (y=0; y<systemSize[1]; y++)
		{
			if (deliverySystem[x][y].cnt > 0) // if package exists in that coordinate
			{
				fprintf("%d %d %d %d %s %s", &x, &y, &deliverySystem[x][y].building, &deliverySystem[x][y].room, deliverySystem[x][y].passwd, deliverySystem[x][y].context);
			}
		}
	}
	
	// file close
	fclose(fp); 
	
}


//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {
	
	// variable settings
	int N,M;
	int i; // variable for repeat
	int row, column; 
	char passwd[PASSWD_LEN+1];
	char *context;
	
	// file open
	FILE *fp = NULL;
	 
	fp = fopen(filepath, "r");
	if (fp == NULL)
	 printf("can't open the file");
	
	
	//get two numbers (row of deliverySystem and column of deliverySystem) in the file at the first line
	fscanf(fp, "%d %d", &systemSize[0], &systemSize[1]);
	// get masterPassword from the file at the second line
	fscanf(fp, "%s",  masterPassword);
	
	// create delivery system on the double pointer deliverySystem
	deliverySystem = (storage_t**)malloc(systemSize[0] * sizeof (storage_t*));
	for(i=0; i<systemSize[0]; i++)
		deliverySystem[i] = (storage_t*)malloc(systemSize[1] * sizeof (storage_t));
	
	// 	
	for(N=0;N<systemSize[0];N++) 
	{
		for(M=0;M<systemSize[1];M++)
			deliverySystem[N][M].context = (char *)malloc(10 * sizeof(char));
	} 
	
	for(N=0; N<systemSize[0]; N++)
	{
		for(M=0; M<systemSize[1]; M++)
		{
			deliverySystem[N][M].cnt =0;
			
		}
	} 
	 	
	
	// (x,y)에 택배가 존재한다면
	 // initStorage 초기화 = 동,룸번호, 내용을 등 매치.
	while( fscanf(fp, "%d %d", &row, &column) == 2 ) // 파일끝까지 ------------------------------------> 수정필요 
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
	
	int i,j;
	
	for (i=0; i<systemSize[0]; i++)
		free (deliverySystem[i]);
	free(deliverySystem);	
	
	/*
	for (i=0; i<systemSize[0]; i++)
		free(deliverySystem[i].context);
	free(deliverySystem.context);
	*/
	// free(deliverySystem.context);

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
