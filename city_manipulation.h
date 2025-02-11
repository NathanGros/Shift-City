#ifndef CITY_MANIPULATION_H
#define CITY_MANIPULATION_H

#include "structures.h"

City* buildWholeCity();
Building* addFloor(Building *building, Floor *floor);
Building* removeFloor(Building *building);
void moveFloor(Building **updatedBuildings, Building *buildingSrc, Building *buildingDst);
int findBuildingNb(City *city, int buildingX, int buildingZ);
Building* stashFloor(City *city, int buildingX, int buildingZ, Building *stash, int maxStashSize);
Building* dropFloor(City *city, int buildingX, int buildingZ, Building *stash);

#endif
