#include <raylib.h>
#include "drawing.h"

void drawFloor(Floor *floor, int positionX, float altitude, int positionY, int height) {
    int maxFloorSize = 8;
    float heightFactor = 0.2;
    Vector3 position = (Vector3) {positionX + 0.5f, heightFactor * height + altitude, positionY + 0.5f};
    float radius = (double) floor->bottomSize / (double) maxFloorSize / 2.;
    DrawCylinder(position, radius, radius, heightFactor, 4, BLUE);
    DrawCylinderWires(position, radius, radius, heightFactor, 4, WHITE);
}

void drawBuilding(Building *building) {
    Vector3 supportPosition = (Vector3) {building->positionX + 0.5f, -0.5f, building->positionY + 0.5f};
    DrawCube(supportPosition, 1., 1., 1., GRAY);
    DrawCubeWires(supportPosition, 1., 1., 1., BLACK);
    for (int i = 0; i < building->nbFloors; i++) {
        drawFloor(building->floors[i], building->positionX, 0.0f, building->positionY, i);
    }
}

void drawStash(Building *stash, int tileX, int tileY) {
    for (int i = 0; i < stash->nbFloors; i++) {
        drawFloor(stash->floors[i], tileX, 2.0f, tileY, i);
    }
}

void drawCity(City *city) {
    for (int i = 0; i < city->nbBuildings; i++) {
        drawBuilding(city->buildings[i]);
    }
}

void drawSelectedTile(int tileX, int tileY) {
    DrawPlane((Vector3) {tileX + 0.5f, 0.001f, tileY + 0.5f}, (Vector2) {1.0f, 1.0f}, DARKGRAY); 
}
