#ifndef DRAWING_H
#define DRAWING_H

#include <raylib.h>
#include "structures.h"

void drawFloor(Floor *floor, int positionX, float altitude, int positionY, int height);
void drawBuilding(Building *building);
void drawStash(Building *stash, int tileX, int tileY);
void drawCity(City *city);
void drawSelectedTile(int tileX, int tileY);

#endif
