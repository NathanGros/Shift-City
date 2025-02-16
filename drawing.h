#ifndef DRAWING_H
#define DRAWING_H

#include <raylib.h>
#include "structures.h"

void drawFloor(Floor *floor, float positionX, float altitude, float positionZ, int height);
void drawBuilding(Building *building);
void drawStash(Building *stash, int tileX, int tileZ);
void drawCity(City *city);
void drawSelectedTile(int tileX, int tileZ);
void drawPoints(int points);
void drawObjectiveButton();
void drawObjective(Objective *objective, float posX, float posY, float posZ);
void drawObjectiveRow(ObjectiveRow *objectiveRow, float posX, float posY, float posZ);
void drawAllObjectives(AllObjectives *allObjectives);

#endif
