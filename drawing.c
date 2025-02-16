#include <stdio.h>

#include <raylib.h>
#include "drawing.h"

void drawFloor(Floor *floor, float positionX, float altitude, float positionZ, int height) {
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
    drawFloor(building->floors[i], (float) building->positionX, 0.0f, (float) building->positionZ, i);
  }
}

void drawStash(Building *stash, int tileX, int tileZ) {
  for (int i = 0; i < stash->nbFloors; i++) {
    drawFloor(stash->floors[i], tileX, 2.0f, tileZ, i);
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
    DrawRectangle(characterX - 10, 20, 40, 40, WHITE);
    DrawText("0", characterX, 20, 40, BLACK);
  }
  else {
    while (points > 0) {
      DrawRectangle(characterX - 10, 20, 40, 40, WHITE);
      int lastDigit = points % 10;
      char stringDigit[1];
      stringDigit[0] = (char) (lastDigit + (int) '0');
      DrawText(stringDigit, characterX, 20, 40, BLACK);
      characterX -= 30;
      points = (points - lastDigit) / 10;
    }
  }
}

void drawObjectiveButton() {
  DrawRectangle(20, 20, 180, 45, WHITE);
  DrawText("Objectives", 30, 30, 30, BLACK);
}

void drawObjective(Objective *objective, float posX, float posY, float posZ) {
  Vector3 supportPosition = (Vector3) {posX + 0.5f, posY - 0.2f, posZ + 0.5f};
  Color supportColor;
  Color wireColor;
  switch (objective->state) {
    case 1:
      supportColor = (Color) {0, 200, 100, 255};
      wireColor = (Color) {100, 100, 100, 255};
      break;
    case 0:
      supportColor = (Color) {200, 200, 200, 255};
      wireColor = (Color) {100, 100, 100, 255};
      break;
    default:
      supportColor = (Color) {50, 50, 50, 255};
      wireColor = (Color) {150, 150, 150, 255};
  }
  DrawCube(supportPosition, 1., 0.4, 1., supportColor);
  DrawCubeWires(supportPosition, 1., 0.4, 1., wireColor);
  for (int i = 0; i < objective->building->nbFloors; i++) {
    drawFloor(objective->building->floors[i], posX, posY, posZ, i);
  }
}

void drawObjectiveRow(ObjectiveRow *objectiveRow, float posX, float posY, float posZ) {
  for (int i = 0; i < objectiveRow->nbObjectives; i++) {
    drawObjective(objectiveRow->objectives[i], posX + i, posY, posZ - i);
  }
}

void drawAllObjectives(AllObjectives *allObjectives) {
  for (int i = 0; i < allObjectives->nbRows; i++) {
    drawObjectiveRow(allObjectives->objectiveRows[i], 2.0f * i, 0.0f, 2.0f * i);
  }
}
