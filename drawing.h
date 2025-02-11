#ifndef DRAWING_H
#define DRAWING_H

#include <raylib.h>
#include "structures.h"

void drawFloor(Floor *floor, int positionX, float altitude, int positionZ, int height);
void drawBuilding(Building *building);
void drawStash(Building *stash, int tileX, int tileZ);
void drawCity(City *city);
void drawSelectedTile(int tileX, int tileZ);

#endif
