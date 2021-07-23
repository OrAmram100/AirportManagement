#ifndef __AIR_MANAGER__
#define __AIR_MANAGER__

#include "Airport.h"
#include "ListGen.h"

typedef struct
{
	LIST	listAirport;
	int			count;
}AirportManager;

int		initManager(AirportManager* pManager);
int		addAirport(AirportManager* pManager);
void	setAirport(Airport* pPort, AirportManager* pManager);
Airport* findAirportByCode(const AirportManager* pManager, const char* code);
int		checkUniqeCode(const char* code, const AirportManager* pManager);
void	printAirports(const AirportManager* pManager);
void	freeManager(AirportManager* pManager);
int compareAirportsByIata(const void* a, const void* b);
int readFromText(char* pf, AirportManager* pManager);
void saveToFileText(const AirportManager* pManager);
int readAirportFromTextFile(FILE* fp, Airport* airport);
#endif