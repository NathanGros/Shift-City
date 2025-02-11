#ifndef INPUT_MANAGEMENT_H
#define INPUT_MANAGEMENT_H

#include <raylib.h>
#include "structures.h"

void updateCamera(Camera3D *camera, float pi, float speed, float *verticalAngle, float *horizontalAngle, float *targetDistance);
void updateCursorBuildingCoordinates(Camera camera, City *city, int *cursorTileX, int *cursorTileZ);

#endif
