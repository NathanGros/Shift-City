#include <stdio.h>

#include <raylib.h>
#include "drawing.h"

void drawFloor(Floor *floor, int positionX, float altitude, int positionZ, int height) {
  int maxFloorSize = 8;
  float heightFactor = 0.2;
  Vector3 position = (Vector3) {positionX + 0.5f, heightFactor * height + altitude, positionZ + 0.5f};
  float radius = (double) floor->bottomSize / (double) maxFloorSize / 2.;
  DrawCylinder(position, radius, radius, heightFactor, 4, BLUE);
  DrawCylinderWires(position, radius, radius, heightFactor, 4, WHITE);
}

void drawBuilding(Building *building) {
  Vector3 supportPosition = (Vector3) {building->positionX + 0.5f, -0.5f, building->positionZ + 0.5f};
  DrawCube(supportPosition, 1., 1., 1., GRAY);
  DrawCubeWires(supportPosition, 1., 1., 1., BLACK);
  for (int i = 0; i < building->nbFloors; i++) {
    drawFloor(building->floors[i], building->positionX, 0.0f, building->positionZ, i);
  }
}

void drawStash(Building *stash, int tileX, int tileZ) {
  for (int i = 0; i < stash->nbFloors; i++) {
    drawFloor(stash->floors[i], tileX, 2.0f, tileZ, i);
  }
}

void drawObjective(Building *objective, Camera3D camera) {
  Vector3 pos = GetScreenToWorldRay((Vector2) {100, 100}, camera).position;
  printf("%f, %f, %f\n", pos.x, pos.y, pos.z);
  DrawCubeWires(pos, 1., 1., 1., RED);
  Vector3 supportPosition = (Vector3) {0.5f, -0.5f, -1.5f};
  DrawCube(supportPosition, 1., 1., 1., RED);
  DrawCubeWires(supportPosition, 1., 1., 1., BLACK);
  for (int i = 0; i < objective->nbFloors; i++) {
    drawFloor(objective->floors[i], 0.0f, 0.0f, -2.0f, i);
  }
}

void drawCity(City *city) {
  for (int i = 0; i < city->nbBuildings; i++) {
    drawBuilding(city->buildings[i]);
  }
}

void drawSelectedTile(int tileX, int tileZ) {
  DrawPlane((Vector3) {tileX + 0.5f, 0.001f, tileZ + 0.5f}, (Vector2) {1.0f, 1.0f}, DARKGRAY); 
}

void drawPoints(int points) {
  int characterX = GetScreenWidth() - 40;
  if (points <= 0) {
    DrawText("0", characterX, 20, 40, BLACK);
  }
  else {
    while (points > 0) {
      int lastDigit = points % 10;
      char stringDigit[1];
      stringDigit[0] = (char) (lastDigit + (int) '0');
      DrawText(stringDigit, characterX, 20, 40, BLACK);
      characterX -= 30;
      points = (points - lastDigit) / 10;
    }
  }
}
