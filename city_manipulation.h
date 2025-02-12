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
int compareFloor(Floor *floor1, Floor *floor2);
int compareBuilding(Building *building1, Building *building2);
Building* makeObjective();
Building* makeNewObjective(Building *objective);

#endif
