#include <raylib.h>
#include <stdio.h>
#include <string.h>

#include "city_manipulation.h"
#include "drawing.h"

void drawFloor(Floor *floor, float positionX, float altitude, float positionZ,
               int isSelected) {
  Vector3 position = (Vector3){positionX + 0.5f, altitude, positionZ + 0.5f};
  Model floorModel = *(floor->model);
  float radius = 1.;
  printf("model valid %d\n", IsModelValid(floorModel));
  if (isSelected == 1)
    DrawModel(floorModel, position, radius, (Color){210, 210, 210, 255});
  else
    DrawModel(floorModel, position, radius, WHITE);
}

void drawBuilding(Building *building, int isSelected) {
  Vector3 supportPosition = (Vector3){building->positionX + 0.5f, -0.11765f,
                                      building->positionZ + 0.5f};
  Model groundModel = *(building->groundModel);
  float radius = 1.;
  if (isSelected == 1)
    DrawModel(groundModel, supportPosition, radius,
              (Color){210, 210, 210, 255});
  else
    DrawModel(groundModel, supportPosition, radius, WHITE);
  float currentFloorHeight = 0.0;
  for (int i = 0; i < building->nbFloors; i++) {
    drawFloor(building->floors[i], (float)building->positionX,
              currentFloorHeight, (float)building->positionZ, isSelected);
    currentFloorHeight += building->floors[i]->height;
  }
}

void drawStash(Building *stash, int tileX, int tileZ) {
  float currentFloorHeight = 0.0;
  for (int i = 0; i < stash->nbFloors; i++) {
    drawFloor(stash->floors[i], tileX, 1.5f + currentFloorHeight, tileZ, 0);
    currentFloorHeight += stash->floors[i]->height;
  }
}

void drawCity(City *city, int selectedTileX, int selectedTileZ) {

  int selectedBuildingNb = findBuildingNb(city, selectedTileX, selectedTileZ);
  for (int i = 0; i < city->nbBuildings; i++) {
    printf("Drawing building %d\n", i);
    if (i == selectedBuildingNb)
      drawBuilding(city->buildings[i], 1);
    else
      drawBuilding(city->buildings[i], 0);
  }
}

void drawPoints(int points) {
  int characterX = GetScreenWidth() - 40;
  if (points <= 0) {
    DrawRectangle(characterX - 10, 20, 40, 40, WHITE);
    DrawText("0", characterX, 20, 40, BLACK);
  } else {
    while (points > 0) {
      DrawRectangle(characterX - 10, 20, 40, 40, WHITE);
      int lastDigit = points % 10;
      char stringDigit[1];
      stringDigit[0] = (char)(lastDigit + (int)'0');
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
  Vector3 supportPosition = (Vector3){posX + 0.5f, posY - 0.2f, posZ + 0.5f};
  Color supportColor;
  Color wireColor;
  switch (objective->state) {
  case 1:
    supportColor = (Color){41, 230, 70, 255};
    wireColor = (Color){100, 100, 100, 255};
    break;
  case 0:
    supportColor = (Color){200, 200, 200, 255};
    wireColor = (Color){100, 100, 100, 255};
    break;
  default:
    supportColor = (Color){50, 50, 50, 255};
    wireColor = (Color){150, 150, 150, 255};
  }
  DrawCube(supportPosition, 1., 0.4, 1., supportColor);
  DrawCubeWires(supportPosition, 1., 0.4, 1., wireColor);
  float currentFloorHeight = 0.0;
  for (int i = 0; i < objective->building->nbFloors; i++) {
    drawFloor(objective->building->floors[i], posX, posY + currentFloorHeight,
              posZ, 0);
    currentFloorHeight += objective->building->floors[i]->height;
  }
}

void drawObjectiveRow(ObjectiveRow *objectiveRow, float posX, float posY,
                      float posZ) {
  for (int i = 0; i < objectiveRow->nbObjectives; i++) {
    drawObjective(objectiveRow->objectives[i], posX + i, posY, posZ - i);
  }
}

void drawAllObjectives(AllObjectives *allObjectives) {
  for (int i = 0; i < allObjectives->nbRows; i++) {
    drawObjectiveRow(allObjectives->objectiveRows[i], 2.0f * i, 0.0f, 2.0f * i);
  }
}
