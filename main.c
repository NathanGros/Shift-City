#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <raylib.h>



struct Floor {
  int bottomSize;
  int topSize;
  int nbLinks;
  struct Floor **links; // pointer to a list of other floors' pointers (the grouped floors)
};
typedef struct Floor Floor;

Floor* makeFloor(int bottomSize, int topSize, int nbLinks) {
  Floor *floor = malloc(sizeof(Floor));
  floor->bottomSize = bottomSize;
  floor->topSize = topSize;
  floor->nbLinks = nbLinks;
  floor->links = malloc(nbLinks * sizeof(Floor*));
  return floor;
}

void freeFloor(Floor *floor) {
  free(floor->links);
  free(floor);
}

typedef struct {
  int positionX;
  int positionY;
  int nbFloors;
  Floor **floors; // pointer to a list of floor pointers (the stacked floors in order)
} Building;

Building* makeBuilding(int positionX, int positionY, int nbFloors) {
  Building *building = malloc(sizeof(Building));
  building->positionX = positionX;
  building->positionY = positionY;
  building->nbFloors = nbFloors;
  building->floors = malloc(nbFloors * sizeof(Floor*));
  return building;
}

void freeBuilding(Building *building) {
  for (int i = 0; i < building->nbFloors; i++) {
    freeFloor(building->floors[i]);
  }
  free(building->floors);
  free(building);
}

typedef struct {
  int nbBuildings;
  Building **buildings;
} City;

City* makeCity(int nbBuildings) {
  City *city = malloc(sizeof(City));
  city->nbBuildings = nbBuildings;
  city->buildings = malloc(nbBuildings * sizeof(Building*));
  return city;
}

void freeCity(City *city) {
  for (int i = 0; i < city->nbBuildings; i++) {
    freeBuilding(city->buildings[i]);
  }
  free(city->buildings);
  free(city);
}



void Init(Color backgroundColor) {
  SetConfigFlags(FLAG_WINDOW_TOPMOST || FLAG_WINDOW_RESIZABLE);
  InitWindow(0, 0, "Shift City");
  ClearBackground(backgroundColor);
  SetTargetFPS(60);
}

City* buildWholeCity() {
  Floor *floor1 = makeFloor(8, 7, 0);
  Floor *floor2 = makeFloor(7, 6, 0);
  Floor *floor3 = makeFloor(6, 5, 0);
  Floor *floor4 = makeFloor(5, 4, 0);
  Floor *floor5 = makeFloor(4, 3, 0);
  Floor *floor6 = makeFloor(3, 2, 0);
  Floor *floor7 = makeFloor(2, 1, 0);
  Floor *floor8 = makeFloor(1, 0, 0);
  Building *building1 = makeBuilding(0, 0, 8);
  Building *building2 = makeBuilding(0, 1, 0);
  Building *building3 = makeBuilding(0, 2, 0);
  building1->floors[0] = floor1;
  building1->floors[1] = floor2;
  building1->floors[2] = floor3;
  building1->floors[3] = floor4;
  building1->floors[4] = floor5;
  building1->floors[5] = floor6;
  building1->floors[6] = floor7;
  building1->floors[7] = floor8;
  City *city1 = makeCity(3);
  city1->buildings[0] = building1;
  city1->buildings[1] = building2;
  city1->buildings[2] = building3;
  return city1;
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
  *targetDistance *= 1. - 0.2 * GetMouseWheelMove();
  if (*targetDistance <= 2.) *targetDistance = 2.;
  if (*targetDistance >= 30.) *targetDistance = 30.;

  // camera position
  camera->position.x = camera->target.x + *targetDistance * cos(*horizontalAngle) * cos(*verticalAngle);
  camera->position.y = camera->target.y + *targetDistance * sin(*verticalAngle);
  camera->position.z = camera->target.z + *targetDistance * sin(*horizontalAngle) * cos(*verticalAngle);
}

void printFloor(Floor *floor) {
  printf("Floor : bottomSize %d, topSize %d, nbLinks %d\n", floor->bottomSize, floor->topSize, floor->nbLinks);
}

void printBuilding(Building *building) {
  printf("Building : X %d, Y %d, nbFloors %d\n", building->positionX, building->positionY, building->nbFloors);
  for (int i = 0; i < building->nbFloors; i++) {
    printf("\t%d ", i);
    printFloor(building->floors[i]);
  }
}

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

Building* addFloor(Building *building, Floor *floor) {
  int nbFloors = building->nbFloors;
  Building *newBuilding = makeBuilding(building->positionX, building->positionY, nbFloors + 1);
  for (int i = 0; i < nbFloors; i++) {
    newBuilding->floors[i] = building->floors[i];
  }
  newBuilding->floors[nbFloors] = floor;
  free(building->floors);
  free(building);
  return newBuilding;
}

Building* removeFloor(Building *building) {
  int nbFloors = building->nbFloors;
  Building *newBuilding = makeBuilding(building->positionX, building->positionY, nbFloors - 1);
  for (int i = 0; i < nbFloors - 1; i++) {
    newBuilding->floors[i] = building->floors[i];
  }
  free(building->floors);
  free(building);
  return newBuilding;
}

void moveFloor(Building **updatedBuildings, Building *buildingSrc, Building *buildingDst) {
  Floor *storedFloor = buildingSrc->floors[buildingSrc->nbFloors - 1];
  Building *newBuildingSrc = removeFloor(buildingSrc);
  Building *newBuildingDst = addFloor(buildingDst, storedFloor);
  updatedBuildings[0] = newBuildingSrc;
  updatedBuildings[1] = newBuildingDst;
}

int findBuildingNb(City *city, int buildingX, int buildingY) {
  for (int i = 0; i < city->nbBuildings; i++) {
    Building *current = city->buildings[i];
    if (current->positionX == buildingX && current->positionY == buildingY) {
      return i;
    }
  }
  return -1;
}

Building* stashFloor(City *city, int buildingX, int buildingY, Building *stash, int maxStashSize) {
  if (stash->nbFloors >= maxStashSize) {
    printf("ERROR: Can't stash more floors\n");
    return stash;
  }
  int buildingNb = findBuildingNb(city, buildingX, buildingY);
  if (city->buildings[buildingNb]->nbFloors <= 0) {
    printf("ERROR: No floor in specified building\n");
    return stash;
  }
  Building **updatedBuildings = malloc(2 * sizeof(Building*));
  moveFloor(updatedBuildings, city->buildings[buildingNb], stash);
  city->buildings[buildingNb] = updatedBuildings[0];
  return updatedBuildings[1];
}

Building* dropFloor(City *city, int buildingX, int buildingY, Building *stash) {
  int buildingNb = findBuildingNb(city, buildingX, buildingY);
  if (stash->nbFloors <= 0) {
    printf("ERROR: No floor stashed\n");
    return stash;
  }
  Building *foundBuilding = city->buildings[buildingNb];
  if (foundBuilding->nbFloors != 0 &&
      (stash->floors[stash->nbFloors - 1]->bottomSize > foundBuilding->floors[foundBuilding->nbFloors - 1]->topSize)) {
    printf("ERROR: Floor is too big to be placed there\n");
    return stash;
  }
  Building **updatedBuildings = malloc(2 * sizeof(Building*));
  moveFloor(updatedBuildings, stash, city->buildings[buildingNb]);
  city->buildings[buildingNb] = updatedBuildings[1];
  return updatedBuildings[0];
}

void updateCursorBuildingCoordinates(Camera camera, City *city, int *cursorBuildingX, int *cursorBuildingY) {
  for (int i = 0; i < city->nbBuildings; i++) {
    Building *current = city->buildings[i];
    int bX = current->positionX;
    int bY = current->positionY;
    Vector2 vertexArray[4] = {
      GetWorldToScreen((Vector3) {(float) bX, 0.0f, (float) bY}, camera),
      GetWorldToScreen((Vector3) {(float) bX+1, 0.0f, (float) bY}, camera),
      GetWorldToScreen((Vector3) {(float) bX+1, 0.0f, (float) bY+1}, camera),
      GetWorldToScreen((Vector3) {(float) bX, 0.0f, (float) bY+1}, camera)
    };
    if (CheckCollisionPointPoly(GetMousePosition(), vertexArray, 4)) {
      *cursorBuildingX = bX;
      *cursorBuildingY = bY;
      break;
    }
  }
}



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

  // cursor control
  int cursorBuildingX = 0;
  int cursorBuildingY = 0;

  // initialize space
  Building *stash = makeBuilding(0, 0, 0);
  City *city1 = buildWholeCity();
  int maxStashSize = 1;
  
  while (!WindowShouldClose()) {
    updateCamera(&camera, pi, speed, &verticalAngle, &horizontalAngle, &targetDistance);
    updateCursorBuildingCoordinates(camera, city1, &cursorBuildingX, &cursorBuildingY);

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      stash = stashFloor(city1, cursorBuildingX, cursorBuildingY, stash, maxStashSize);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      stash = dropFloor(city1, cursorBuildingX, cursorBuildingY, stash);
    }

    // drawing
    BeginDrawing();
      ClearBackground(backgroundColor);
      BeginMode3D(camera);
        drawCity(city1);
        drawSelectedTile(cursorBuildingX, cursorBuildingY);
        drawStash(stash, cursorBuildingX, cursorBuildingY);
      EndMode3D();
    EndDrawing();
  }
  CloseWindow();

  // de-init
  freeCity(city1);
  freeBuilding(stash);

  return 0;
}
