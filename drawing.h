#ifndef DRAWING_H
#define DRAWING_H

#include <raylib.h>
#include "structures.h"

void drawFloor(Floor *floor, float positionX, float altitude, float positionZ, int isSelected);
void drawBuilding(Building *building, int isSelected);
void drawStash(Building *stash, int tileX, int tileZ);
void drawCity(City *city, int selectedTileX, int selectedTileZ);
void drawPoints(int points);
void drawObjectiveButton();
void drawObjective(Objective *objective, float posX, float posY, float posZ);
void drawObjectiveRow(ObjectiveRow *objectiveRow, float posX, float posY, float posZ);
void drawAllObjectives(AllObjectives *allObjectives);

#endif
