#include <stdio.h>
#include <string.h>

#include "city_manipulation.h"

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

City* buildWholeCity() {
  Floor *floor1 = makeFloor(8, 7, 0);
  floor1->model = "floor_8to7.obj";
  Floor *floor2 = makeFloor(7, 6, 0);
  floor2->model = "floor_7to6.obj";
  Floor *floor3 = makeFloor(6, 5, 0);
  floor3->model = "floor_6to5.obj";
  Floor *floor4 = makeFloor(5, 4, 0);
  floor4->model = "floor_5to4.obj";
  Floor *floor5 = makeFloor(4, 3, 0);
  floor5->model = "floor_4to3.obj";
  Floor *floor6 = makeFloor(3, 2, 0);
  floor6->model = "floor_3to2.obj";
  Floor *floor7 = makeFloor(2, 1, 0);
  floor7->model = "floor_2to1.obj";
  Floor *floor8 = makeFloor(1, 0, 0);
  floor8->model = "floor_1to0.obj";
  Building *building1 = makeBuilding(0, 0, 8);
  Building *building2 = makeBuilding(0, 1, 0);
  Building *building3 = makeBuilding(0, 3, 0);
  Building *building4 = makeBuilding(1, 0, 0);
  Building *building5 = makeBuilding(1, 2, 0);
  Building *building6 = makeBuilding(1, 3, 0);
  building1->floors[0] = floor1;
  building1->floors[1] = floor2;
  building1->floors[2] = floor3;
  building1->floors[3] = floor4;
  building1->floors[4] = floor5;
  building1->floors[5] = floor6;
  building1->floors[6] = floor7;
  building1->floors[7] = floor8;
  City *city1 = makeCity(6);
  city1->buildings[0] = building1;
  city1->buildings[1] = building2;
  city1->buildings[2] = building3;
  city1->buildings[3] = building4;
  city1->buildings[4] = building5;
  city1->buildings[5] = building6;
  return city1;
}

Building* addFloor(Building *building, Floor *floor) {
  int nbFloors = building->nbFloors;
  Building *newBuilding = makeBuilding(building->positionX, building->positionZ, nbFloors + 1);
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
  Building *newBuilding = makeBuilding(building->positionX, building->positionZ, nbFloors - 1);
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

int findBuildingNb(City *city, int buildingX, int buildingZ) {
  for (int i = 0; i < city->nbBuildings; i++) {
    Building *current = city->buildings[i];
    if (current->positionX == buildingX && current->positionZ == buildingZ) {
      return i;
    }
  }
  return -1;
}

Building* stashFloor(City *city, int buildingX, int buildingZ, Building *stash, int maxStashSize) {
  if (stash->nbFloors >= maxStashSize) {
    printf("ERROR: Can't stash more floors\n");
    return stash;
  }
  int buildingNb = findBuildingNb(city, buildingX, buildingZ);
  if (city->buildings[buildingNb]->nbFloors <= 0) {
    printf("ERROR: No floor in specified building\n");
    return stash;
  }
  Building **updatedBuildings = malloc(2 * sizeof(Building*));
  moveFloor(updatedBuildings, city->buildings[buildingNb], stash);
  city->buildings[buildingNb] = updatedBuildings[0];
  return updatedBuildings[1];
}

Building* dropFloor(City *city, int buildingX, int buildingZ, Building *stash) {
  int buildingNb = findBuildingNb(city, buildingX, buildingZ);
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

Building* makeBuildingNFloors(int n) {
  Building *building = makeBuilding(0, 0, n);
  int floorSize = n;
  for (int i = 0; i < n; i++) {
    Floor *newFloor = makeFloor(floorSize, floorSize-1, 0);
    char modelName[100] = "floor_";
    char nb1[2];
    char nb2[2];
    nb1[0] = (char) (floorSize + (int) '0');
    nb1[1] = '\0';
    nb2[0] = (char) (floorSize - 1 + (int) '0');
    nb2[1] = '\0';
    strcat(modelName, nb1);
    strcat(modelName, "to");
    strcat(modelName, nb2);
    strcat(modelName, ".obj");
    strcpy(newFloor->model, modelName);
    building->floors[i] = newFloor;
    floorSize--;
  }
  return building;
}
