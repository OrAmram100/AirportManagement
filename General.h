#ifndef __GENERAL__
#define __GENERAL__

typedef void* DATA;					// a type for data (easy to change)
typedef enum { False, True } BOOL;	// a boolean type
#define MAX_STR_LEN 255
#define CODE_LENGTH 3
#define TEXT_FILE "airport_authority.txt"
#define BIN_FILE "company.bin"
char*	getStrExactName(const char* msg);
char*	myGets(char* buffer, int size);
char*	getDynStr(char* str);
char**	splitCharsToWords(char* str,int* pCount,int* pTotalLength);
void generalArrayFunction(void* arr, int size, int typeSize, void f(void* element));
#endif

