#ifndef __COMP__
#define __COMP__

#include "Flight.h"
#include "AirportManager.h"
#include "ListGen.h"
typedef enum { eHour, eDate, eIataSourceCode, eIataDesCode, eNotSortes, eNofSort} sortType;
const char* sortTypeStr[eNofSort];
typedef struct
{
	sortType	typeOfSorting;
	LIST		listOfDates;
	char*		name;
	int			flightCount;
	Flight**	flightArr;
}Company;

int readCompanyFromBinaryFile(Company* pComp);
void	initCompany(Company* pComp);
int checkIfDateExist(Flight** pFlight, Flight* flight, int size);
int		addFlight(Company* pComp,const AirportManager* pManager);
void	printCompany(const Company* pComp);
void	printFlightsCount(const Company* pComp);
void	printFlightArr(Flight** pFlight, int size);
void	freeFlightArr(Flight** arr, int size);
void	freeCompany(Company* pComp);
int		compareByDate(const void* f1, const void* f2);
int		compareByHour(const void* f1, const void* f2);
void	getSortTypeFromUser(Company* pComp);
int		compareByIataCodeSource(const void* f1, const void* f2);
int		compareByIataCodeDest(const void* f1, const void* f2);
void	saveCompanyToFile(Company* pComp);
void findFlightBySearchMethod(Company* pComp);
int addFlightFromBinary(Company* pComp, Flight* pFlight);
#endif

