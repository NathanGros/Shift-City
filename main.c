#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <raylib.h>



typedef struct {
  int x; // position x
  int y; // position y
} Plot;

Plot* makePlot(int x, int y) {
  Plot *plot = malloc(sizeof(Plot));
  plot->x = x;
  plot->y = y;
  return plot;
}

void freePlot(Plot *plot) {
  free(plot);
}

typedef struct {
  int nbPlots;
  Plot **plots; // list of plot pointers
} City;

City* makeCity(int nbPlots) {
  City *city = malloc(sizeof(City));
  city->nbPlots = nbPlots;
  city->plots = malloc(nbPlots * sizeof(Plot*));
  return city;
}

void freeCity(City *city) {
  for (int i = 0; i < city->nbPlots; i++) {
    freePlot(city->plots[i]);
  }
  free(city);
}

typedef struct {
  int bottomSize; // floor bottom size
  int topSize; // floor top size
  int height; // floor height compared to ground level
  Plot *plot; // the plot on which the floor is built
} Floor;

Floor* makeFloor(int bottomSize, int topSize, int height, Plot *plot) {
  Floor *floor = malloc(sizeof(Floor));
  floor->bottomSize = bottomSize;
  floor->topSize = topSize;
  floor->height = height;
  floor->plot = plot;
  return floor;
}

void freeFloor(Floor *floor) {
  freePlot(floor->plot);
  free(floor);
}

typedef struct {
  int nbFloors;
  Floor **floors; // list of floor pointers
} FloorGroup;

FloorGroup* makeFloorGroup(int nbFloors) {
  FloorGroup *floorGroup = malloc(sizeof(FloorGroup));
  floorGroup->nbFloors = nbFloors;
  floorGroup->floors = malloc(nbFloors * sizeof(Floor*));
  return floorGroup;
}

void freeFloorGroup(FloorGroup *floorGroup) {
  for (int i = 0; i < floorGroup->nbFloors; i++) {
    freeFloor(floorGroup->floors[i]);
  }
  free(floorGroup);
}

typedef struct {
  int nbFloors;
  Floor **floors; // list of floor pointers
} FloorList;

FloorList* makeFloorList(int nbFloors) {
  FloorList *floorList = malloc(sizeof(FloorList));
  floorList->nbFloors = nbFloors;
  floorList->floors = malloc(nbFloors * sizeof(Floor*));
  return floorList;
}

void freeFloorList(FloorList *floorList) {
  for (int i = 0; i < floorList->nbFloors; i++) {
    freeFloor(floorList->floors[i]);
  }
  free(floorList);
}



void Init(Color backgroundColor) {
  SetConfigFlags(FLAG_WINDOW_TOPMOST || FLAG_WINDOW_RESIZABLE);
  InitWindow(0, 0, "Shift City");
  ClearBackground(backgroundColor);
  SetTargetFPS(60);
}

void updateCamera(Camera3D *camera, float pi, float speed, float *verticalAngle, float *horizontalAngle, float *targetDistance) {
  float cameraVectorX = camera->target.x - camera->position.x;
  float cameraVectorZ = camera->target.z - camera->position.z;
  float cameraDistanceToTarget = sqrt(pow(cameraVectorX, 2.) + pow(cameraVectorZ, 2.));
  if (IsKeyDown(KEY_W)) {
    camera->target.x += speed * cameraVectorX / cameraDistanceToTarget;
    camera->target.z += speed * cameraVectorZ / cameraDistanceToTarget;
  }
  if (IsKeyDown(KEY_S)) {
    camera->target.x -= speed * cameraVectorX / cameraDistanceToTarget;
    camera->target.z -= speed * cameraVectorZ / cameraDistanceToTarget;
  }
  if (IsKeyDown(KEY_A)) {
    camera->target.x += speed * cameraVectorZ / cameraDistanceToTarget;
    camera->target.z += -1. * speed * cameraVectorX / cameraDistanceToTarget;
  }
  if (IsKeyDown(KEY_D)) {
    camera->target.x += -1. * speed * cameraVectorZ / cameraDistanceToTarget;
    camera->target.z += speed * cameraVectorX / cameraDistanceToTarget;
  }

  // camera angle
  if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
    *horizontalAngle += GetMouseDelta().x * 0.01;
    *verticalAngle += GetMouseDelta().y * 0.01;
    if (*verticalAngle >= pi / 2.) *verticalAngle = pi / 2.;
    if (*verticalAngle <= 0.) *verticalAngle = 0.;
  }

  // camera zoom
  *targetDistance *= 1. - 0.3 * GetMouseWheelMove();
  if (*targetDistance <= 2.) *targetDistance = 2.;
  if (*targetDistance >= 30.) *targetDistance = 30.;

  // camera position
  camera->position.x = camera->target.x + *targetDistance * cos(*horizontalAngle) * cos(*verticalAngle);
  camera->position.y = camera->target.y + *targetDistance * sin(*verticalAngle);
  camera->position.z = camera->target.z + *targetDistance * sin(*horizontalAngle) * cos(*verticalAngle);
}

void drawFloor(Floor *floor) {
  int maxFloorSize = 8;
  Plot *plot = floor->plot;
  Vector3 position = (Vector3) {plot->x, 0.5 * floor->height, plot->y};
  float radius = (double) floor->bottomSize / (double) maxFloorSize;
  DrawCylinder(position, radius, radius, 0.5, 4, BLUE);
  DrawCylinderWires(position, radius, radius, 0.5, 4, WHITE);
}



int main() {
  // init
  Color backgroundColor = (Color) {200, 200, 200, 255};
  Init(backgroundColor);
  ToggleFullscreen();
  DisableCursor();
  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();

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

  // making the city
  Plot *plot1 = makePlot(0, 0);
  Plot *plot2 = makePlot(0, 1);
  Plot *plot3 = makePlot(0, 2);
  City *city1 = makeCity(3);
  city1->plots[0] = plot1;
  city1->plots[1] = plot2;
  city1->plots[2] = plot3;
  Floor *floor1 = makeFloor(3, 2, 0, plot1);
  Floor *floor2 = makeFloor(2, 1, 1, plot1);
  Floor *floor3 = makeFloor(2, 1, 0, plot3);
  FloorList *floorList = makeFloorList(3);
  floorList->floors[0] = floor1;
  floorList->floors[1] = floor2;
  floorList->floors[2] = floor3;
  
  while (!WindowShouldClose()) {
    // resize window
    if (IsWindowResized()) {
      screenWidth = GetScreenWidth();
      screenHeight = GetScreenHeight();
    }

    updateCamera(&camera, pi, speed, &verticalAngle, &horizontalAngle, &targetDistance);

    // drawing
    BeginDrawing();
      ClearBackground(backgroundColor);
      BeginMode3D(camera);
        DrawGrid(1000, 1.0f);
        for (int i = 0; i < floorList->nbFloors; i++) {
          drawFloor(floorList->floors[i]);
        }
      EndMode3D();
    EndDrawing();
  }
  CloseWindow();

  // de-init
  freeFloor(floor1);
  freeFloor(floor2);
  freeFloor(floor3);
  freeCity(city1);
  freeFloorList(floorList);

  return 0;
}
