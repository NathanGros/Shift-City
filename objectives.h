#ifndef OBJECTIVES_H
#define OBJECTIVES_H

#include "structures.h"

int compareFloor(Floor *floor1, Floor *floor2);
int compareBuilding(Building *building1, Building *building2);
void checkObjectiveCompletion(Building *building, AllObjectives *allObjectives, int *points);
AllObjectives* BuildAllObjectives();

#endif
