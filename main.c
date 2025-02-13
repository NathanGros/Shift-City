#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <raylib.h>

#include "structures.h"
#include "drawing.h"
#include "input_management.h"
#include "city_manipulation.h"



void Init(Color backgroundColor) {
  SetConfigFlags(FLAG_WINDOW_TOPMOST || FLAG_WINDOW_RESIZABLE);
  InitWindow(0, 0, "Shift City");
  ClearBackground(backgroundColor);
  SetTargetFPS(60);
}

/*void printFloor(Floor *floor) {*/
/*  printf("Floor : bottomSize %d, topSize %d, nbLinks %d\n", floor->bottomSize, floor->topSize, floor->nbLinks);*/
/*}*/

/*void printBuilding(Building *building) {*/
/*  printf("Building : X %d, Z %d, nbFloors %d\n", building->positionX, building->positionZ, building->nbFloors);*/
/*  for (int i = 0; i < building->nbFloors; i++) {*/
/*    printf("\t%d ", i);*/
/*    printFloor(building->floors[i]);*/
/*  }*/
/*}*/



int main() {
  // init
  Color backgroundColor = (Color) {200, 200, 200, 255};
  Init(backgroundColor);
  ToggleFullscreen();

  // camera control
  float pi = 3.141592;
  float verticalAngle = pi / 6.;
  float horizontalAngle = pi;
  float targetDistance = 4.;
  float speed = 0.1;
  Camera3D camera = {0};
  camera.position = (Vector3) {0.0f, 0.0f, 0.0f};
  camera.target = (Vector3) {0.0f, 0.0f, 1.0f};
  camera.up = (Vector3) {0.0f, 1.0f, 0.0f}; // Camera up vector (rotation towards target)
  camera.fovy = 70.0f; // Camera field-of-view Y
  camera.projection = CAMERA_PERSPECTIVE;

  // score related
  int points = 0;
  Building *objective = makeObjective();

  // cursor control
  int cursorTileX = 0;
  int cursorTileZ = 0;

  // initialize space
  Building *stash = makeBuilding(0, 0, 0);
  City *city1 = buildWholeCity();
  int maxStashSize = 1;
  
  while (!WindowShouldClose()) {
    updateCamera(&camera, pi, speed, &verticalAngle, &horizontalAngle, &targetDistance);
    updateCursorBuildingCoordinates(camera, city1, &cursorTileX, &cursorTileZ);

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      stash = stashFloor(city1, cursorTileX, cursorTileZ, stash, maxStashSize);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      Building *newStash = dropFloor(city1, cursorTileX, cursorTileZ, stash);
      if (newStash != stash && compareBuilding(city1->buildings[findBuildingNb(city1, cursorTileX, cursorTileZ)], objective) == 1) {
        points++;
        objective = makeNewObjective(objective);
      }
      stash = newStash;
    }

    // drawing
    BeginDrawing();
      ClearBackground(backgroundColor);
      BeginMode3D(camera);
        drawCity(city1);
        drawSelectedTile(cursorTileX, cursorTileZ);
        drawStash(stash, cursorTileX, cursorTileZ);
      EndMode3D();
      drawPoints(points);

      BeginMode3D(camera);
      drawObjective(objective, camera);
      EndMode3D();
    EndDrawing();
  }
  CloseWindow();

  // de-init
  freeCity(city1);
  freeBuilding(stash);
  freeBuilding(objective);

  return 0;
}
