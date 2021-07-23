#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ListGen.h"
#include "General.h"
#include "AirportManager.h"

int	initManager(AirportManager* pManager)
{
	L_init((LIST*)&pManager->listAirport);
	printf("-----------  Init airport Manager\n");
	pManager->count = 0;
	if (readFromText(TEXT_FILE, pManager) == 1)
	{
		return 1;
	}

	int count=0;
	do {
		printf("How many airport?\t");
		scanf("%d", &count);
	} while (count < 0);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	if (count == 0)
		return 1;
	for (int i = 0; i <count;i++)
	{
		addAirport(pManager);
	}
	return 1;
}

int readFromText(char* pf,AirportManager* pManager)
{
	FILE* fp = fopen(pf, "r");
	if (!fp)
		return 0;
	if (!L_init(&pManager->listAirport))
	{
		fclose(fp);
		return 0;
	}
	fscanf(fp, "%d\n", &pManager->count);
	for (int i = 0; i < pManager->count; i++)
	{
		Airport* airport = (Airport*)malloc(sizeof(Airport));
		if (readAirportFromTextFile(fp, airport) == 0)
			return 0;
		L_insert(&pManager->listAirport.head, airport, compareAirportsByIata);
	}
	return 1;
	fclose(fp);
}
int readAirportFromTextFile(FILE* fp, Airport* airport)
{
	if (!fp)
		return 0;
	char tempName[MAX_STR_LEN];
	char tempCountry[MAX_STR_LEN];
	char tempIATA[MAX_STR_LEN];
	if (fgets(tempName, MAX_STR_LEN, fp) == NULL)
		return 0;
	tempName[strlen(tempName) - 1] = '\0';
	if (fgets(tempCountry, MAX_STR_LEN, fp) == NULL)
		return 0;
	tempCountry[strlen(tempCountry) - 1] = '\0';
	if (fscanf(fp, "%s\n", tempIATA) != 1)
		return 0;
	airport->name = _strdup(tempName);
	airport->country = _strdup(tempCountry);
	strcpy(airport->code, tempIATA);
	return 1;
}

int compareAirportsByIata(const void* a, const void* b)
{
	const Airport* airport1 = (Airport*)a;
	const Airport* airport2 = (Airport*)b;
	return strcmp(airport1->code, airport2->code);
}

int	addAirport(AirportManager* pManager)
{
	Airport* airport;
	airport = (Airport*)malloc(sizeof(Airport));
	if (!airport)
		return 0;
	setAirport(airport, pManager);
	L_insert(&pManager->listAirport, airport, compareAirportsByIata);
	pManager->count++;
	//free(airport);
	return 1;
}

void  setAirport(Airport* pPort, AirportManager* pManager)
{
	while (1)
	{
		getAirportCode(pPort->code);
		if (checkUniqeCode(pPort->code, pManager))
			break;

		printf("This code already in use - enter a different code\n");
	}

	initAirportNoCode(pPort);
}

Airport* findAirportByCode(const AirportManager* pManager, const char* code)
{
	NODE* tmp;
	 tmp = (&(pManager->listAirport))->head.next;
	for (int i = 0; i < pManager->count,tmp;tmp=tmp->next,i++)
	{
		if (isAirportCode(tmp->key, code))     //check if need to do a casting
			return tmp->key;

	}
	return NULL;
}

int checkUniqeCode(const char* code,const AirportManager* pManager)
{
	Airport* port = findAirportByCode(pManager, code);

	if (port != NULL)
		return 0;

	return 1;
}
void saveToFileText(const AirportManager* pManager)
{
	FILE* fp;
	fp = fopen(TEXT_FILE, "w");
	if (!fp)
		return;

	fprintf(fp, "%d\n", pManager->count);
	L_printToFile((LIST*)&(pManager->listAirport), fp, printAirportToFile);
	fclose(fp);
}


void printAirports(const AirportManager* pManager)
{

	printf("there are %d airports\n", pManager->count);
	L_print((LIST*)&pManager->listAirport, printAirport);

}

void freeManager(AirportManager* pManager)
{
	L_free(&pManager->listAirport,freeAirport);
}