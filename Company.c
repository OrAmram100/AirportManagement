#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Company.h"
#include "Airport.h"
#include "General.h"
#include "Date.h"
#include "ListGen.h"
#include <ctype.h>

const char* sortTypeStr[eNofSort] =
{ "According To Hour", "According To Date ", "according To Iata Soucre Code", "according To Dest Code" };

void	initCompany(Company* pComp)
{
	LIST lst;
	L_init(&lst);
	pComp->listOfDates = lst;
	pComp->flightArr = NULL;
	pComp->flightCount = 0;
	printf("-----------  Init Airline Company\n");
		if (readCompanyFromBinaryFile(pComp) == 1)
			return;
	pComp->name = getStrExactName("Enter company name");
	pComp->typeOfSorting = eNotSortes;

}


int	addFlight(Company* pComp,const AirportManager* pManager)
{
	if (pManager->count < 2)
	{
		printf("There are not enoght airport to set a flight\n");
		return 0;
	}
	pComp->flightArr = (Flight**)realloc(pComp->flightArr, (pComp->flightCount+1) * sizeof(Flight*));
	if (!pComp->flightArr)
		return 0;
	pComp->flightArr[pComp->flightCount] = (Flight*)calloc(1,sizeof(Flight));
	if (!pComp->flightArr[pComp->flightCount])
		return 0;
	initFlight(pComp->flightArr[pComp->flightCount],pManager);
	if(checkIfDateExist(pComp->flightArr,pComp->flightArr[pComp->flightCount],pComp->flightCount))
		L_insert(&pComp->listOfDates,convertToChar(&pComp->flightArr[pComp->flightCount]->date),NULL);
	pComp->flightCount++;
	return 1;
}
int compareByHour(const void* f1,const void* f2)
{
	const Flight* flight1 = *(Flight**)f1;
	const Flight* flight2 = *(Flight**)f2;
	return flight1->hour - flight2->hour;
}
int compareByDate(const void* f1, const void* f2)
{
	const Flight* flight1 = *(Flight**)f1;
	const Flight* flight2 = *(Flight**)f2;
	// All cases when true should be returned 
	if (flight1->date.year < flight2->date.year)
		return -1;
	if (flight2->date.year < flight1->date.year)
		return 1;
	if (flight1->date.month < flight2->date.month)
		return -1;
	if (flight2->date.month < flight1->date.month)
		return 1;
	if (flight1->date.day < flight2->date.day)
		return -1;
	if (flight2->date.day < flight1->date.day)
		return 1;
	return 0;
}
int compareByIataCodeSource(const void* f1, const void* f2)
{
	const Flight* flight1 = *(Flight**)f1;
	const Flight* flight2 = *(Flight**)f2;
	return strcmp(flight1->originCode, flight2->originCode);
}
int compareByIataCodeDest(const void* f1, const void* f2)
{
	const Flight* flight1 = *(Flight**)f1;
	const Flight* flight2 = *(Flight**)f2;
	return strcmp(flight1->destCode, flight2->destCode);
}
void saveCompanyToFile(Company* pComp)
{
	int count;
	FILE* fp;
	fp = fopen("company.bin", "wb");
	if (!fp)
		return;
	int nameLen = (int)strlen(pComp->name) + 1;
	if (fwrite(&nameLen, sizeof(int), 1, fp) != 1)
		return;
	if (fwrite(pComp->name, sizeof(char), nameLen, fp) != nameLen)
		return;
	if (fwrite(&pComp->flightCount, sizeof(int), 1, fp) != 1)
		return;
	if (fwrite(&pComp->typeOfSorting, sizeof(sortType), 1, fp) != 1)
		return;
	for (int i = 0; i < pComp->flightCount; i++) {
		if (printFlightToFile(fp, pComp->flightArr[i]) == 0)
			return;
	}

}
	
	int readCompanyFromBinaryFile(Company* pComp)
	{
		FILE* fp = fopen(BIN_FILE, "rb");
		if (!fp)
			return 0;
		int len;
		if (fread(&len, sizeof(int), 1, fp) != 1)
			return 0;
		pComp->name = (char*)malloc(len * sizeof(char));
		if (pComp->name == NULL)
			return 0;
		if (fread(pComp->name, sizeof(char), len, fp) != len)
			return 0;
		int temp;
		if (fread(&temp, sizeof(int), 1, fp) != 1)
			return 0;
		if (fread(&pComp->typeOfSorting, sizeof(sortType), 1, fp) != 1)
			return 0;
		for (int i = 0; i < temp; i++)
		{
			Flight* flight = (Flight*)malloc(sizeof(Flight));
			if (flight == NULL)
				return 0;
			if (readFlightFromBFile(fp, flight) == 0)
				return 0;
			if (addFlightFromBinary(pComp, flight) == 0)
				return 0;
		}
		pComp->flightCount = temp;
		return 1;
	}
	int addFlightFromBinary(Company*pComp, Flight* pFlight)
	{
		if (pFlight == NULL)
			return 0;
		pComp->flightArr = (Flight**)realloc(pComp->flightArr, (pComp->flightCount + 1) * sizeof(Flight*));
		if (!pComp->flightArr)
			return 0;
		pComp->flightArr[pComp->flightCount] = pFlight;
		if (checkIfDateExist(pComp->flightArr, &pComp->flightArr[pComp->flightCount]->date, pComp->flightCount))
			L_insert(&pComp->listOfDates, convertToChar(&pComp->flightArr[pComp->flightCount]->date), NULL);
		pComp->flightCount++;
		return 1;
	}

void findFlightBySearchMethod(Company* pComp)
{
	if (pComp->typeOfSorting == eNotSortes)
	{
		printf("the array isnt sorted!..you cant search a flight");
		return ;
	}
	Flight* pFlight;
	Flight* tmp;
	tmp=(Flight*)malloc(sizeof(Flight));
	pFlight= (Flight*)malloc(sizeof(Flight));
	if (pComp->typeOfSorting == eHour)
	{
		int num;
		printf("Please enter hour for flight search\n");
		scanf("%d", &num);
		tmp->hour = num;
		pFlight = (Flight*)bsearch(&tmp, pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareByHour);
	}
	if (pComp->typeOfSorting == eDate)
	{
		getCorrectDate(&tmp->date);
		pFlight = (Flight*)bsearch(&tmp, pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareByDate);
	}
	if (pComp->typeOfSorting == eIataSourceCode)
	{
		char iataSource[CODE_LENGTH +1 ];
		printf("Please enter Iata code source for flight search");
		myGets(iataSource, CODE_LENGTH+1);
		strcpy(tmp->originCode,iataSource);
		pFlight = (Flight*)bsearch(&tmp, pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareByIataCodeSource);
	}
	if (pComp->typeOfSorting == eIataDesCode)
	{
		char iataDest[CODE_LENGTH +1 ];
		printf("Please enter Iata code dest for flight search");
		myGets(iataDest, CODE_LENGTH+1);
		strcpy(tmp->destCode, iataDest);
		pFlight = (Flight*)bsearch(&tmp, pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareByIataCodeDest);
	}
	free(tmp);
	if(pFlight)
		printFlight(pFlight);
	else
		printf("There is no flight that matches the details entered");
}


void getSortTypeFromUser(Company* pComp)
{
	int temp;
		for (int i = 0; i < eNofSort-1; i++)
		{
			printf("enter %d for sort type %s\n", i, sortTypeStr[i]);
		}
		printf("To exit press -1\n");
		scanf("%d", &temp);
		switch (temp)
		{
		case eHour:
			qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareByHour);
			pComp->typeOfSorting = eHour;
			break;
		case eDate:
			qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareByDate);
			pComp->typeOfSorting = eDate;
			break;
		case  eIataSourceCode:
			qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareByIataCodeSource);
			pComp->typeOfSorting = eIataSourceCode;
			break;
		case eIataDesCode:
			qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareByIataCodeDest);
			pComp->typeOfSorting = eIataDesCode;
			break;
		case eNotSortes:
			break;
		}
}


int checkIfDateExist(Flight** pFlight,Flight* flight,int size)
{
	
	for (int i = 0; i < size; i++)
	{
		if (compareByDate(&pFlight[i],&flight)==0)
			return 0;
	}
	return 1;
}

void printCompany(const Company* pComp)
{
	printf("Company %s:\n", pComp->name);
	printf("Has %d flights\n",pComp->flightCount);
	generalArrayFunction(pComp->flightArr, pComp->flightCount, sizeof(Flight*), printFlight);
	printf("the Flights are on dates:\n");
	L_print((LIST*)&pComp->listOfDates, printDate);
}

void	printFlightsCount(const Company* pComp)
{
	char codeOrigin[CODE_LENGTH+1];
	char codeDestination[CODE_LENGTH + 1];
	
	if (pComp->flightCount == 0)
	{
		printf("No flight to search\n");
		return;
	}

	printf("Origin Airport\n");
	getAirportCode(codeOrigin);
	printf("Destination Airport\n");
	getAirportCode(codeDestination);

	int count = countFlightsInRoute(pComp->flightArr, pComp->flightCount, codeOrigin, codeDestination);
	if (count != 0)
		printf("There are %d flights ", count);
	else
		printf("There are No flights ");

	printf("from %s to %s\n",codeOrigin, codeDestination);
}



void	printFlightArr(Flight** pFlight, int size)
{
	generalArrayFunction(pFlight, size, sizeof(Flight*), printFlight);
}

void	freeFlightArr(Flight** arr, int size)
{
		
	generalArrayFunction(arr,size,sizeof(Flight*), freeFlight);
}

void	freeCompany(Company* pComp)
{
	freeFlightArr(pComp->flightArr, pComp->flightCount);
	free(pComp->flightArr);
	free(pComp->name);
	L_free(&pComp->listOfDates, NULL);

}