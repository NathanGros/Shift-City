#include <stdio.h>
#include <string.h>

#include "city_manipulation.h"

/*void printFloor(Floor *floor) {*/
/*  printf("Floor : bottomSize %d, topSize %d, nbLinks %d\n", floor->bottomSize,
 * floor->topSize, floor->nbLinks);*/
/*}*/

/*void printBuilding(Building *building) {*/
/*  printf("Building : X %d, Z %d, nbFloors %d\n", building->positionX,
 * building->positionZ, building->nbFloors);*/
/*  for (int i = 0; i < building->nbFloors; i++) {*/
/*    printf("\t%d ", i);*/
/*    printFloor(building->floors[i]);*/
/*  }*/
/*}*/

City *buildWholeCity() {
  Building *building1 = makeBuilding(0, 0, 8);
  // building1->groundModel = "ground_pine_pool.obj";
  Building *building2 = makeBuilding(0, 1, 0);
  // building2->groundModel = "ground_pool.obj";
  Building *building3 = makeBuilding(0, 3, 0);
  // building3->groundModel = "ground.obj";
  Building *building4 = makeBuilding(1, 0, 0);
  // building4->groundModel = "ground.obj";
  Building *building5 = makeBuilding(1, 2, 0);
  // building5->groundModel = "ground_pine_pool.obj";
  Building *building6 = makeBuilding(1, 3, 0);
  for (int i = 0; i < 8; i++) {
    building1->floors[i] = makeFloor(8 - i, 7 - i, 0);
  }
  City *city1 = makeCity(6);
  city1->buildings[0] = building1;
  city1->buildings[1] = building2;
  city1->buildings[2] = building3;
  city1->buildings[3] = building4;
  city1->buildings[4] = building5;
  city1->buildings[5] = building6;
  return city1;
}

Building *addFloor(Building *building, Floor *floor) {
  int nbFloors = building->nbFloors;
  Building *newBuilding =
      makeBuilding(building->positionX, building->positionZ, nbFloors + 1);
  for (int i = 0; i < nbFloors; i++) {
    newBuilding->floors[i] = building->floors[i];
  }
  newBuilding->floors[nbFloors] = floor;
  newBuilding->groundModel = building->groundModel;
  free(building->floors);
  free(building);
  return newBuilding;
}

Building *removeFloor(Building *building) {
  int nbFloors = building->nbFloors;
  Building *newBuilding =
      makeBuilding(building->positionX, building->positionZ, nbFloors - 1);
  for (int i = 0; i < nbFloors - 1; i++) {
    newBuilding->floors[i] = building->floors[i];
  }
  newBuilding->groundModel = building->groundModel;
  free(building->floors);
  free(building);
  return newBuilding;
}

void moveFloor(Building **updatedBuildings, Building *buildingSrc,
               Building *buildingDst) {
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

Building *stashFloor(City *city, int buildingX, int buildingZ, Building *stash,
                     int maxStashSize) {
  if (stash->nbFloors >= maxStashSize) {
    printf("ERROR: Can't stash more floors\n");
    return stash;
  }
  int buildingNb = findBuildingNb(city, buildingX, buildingZ);
  if (city->buildings[buildingNb]->nbFloors <= 0) {
    printf("ERROR: No floor in specified building\n");
    return stash;
  }
  Building **updatedBuildings = malloc(2 * sizeof(Building *));
  moveFloor(updatedBuildings, city->buildings[buildingNb], stash);
  city->buildings[buildingNb] = updatedBuildings[0];
  return updatedBuildings[1];
}

Building *dropFloor(City *city, int buildingX, int buildingZ, Building *stash) {
  int buildingNb = findBuildingNb(city, buildingX, buildingZ);
  if (stash->nbFloors <= 0) {
    printf("ERROR: No floor stashed\n");
    return stash;
  }
  Building *foundBuilding = city->buildings[buildingNb];
  if (foundBuilding->nbFloors != 0 &&
      (stash->floors[stash->nbFloors - 1]->bottomSize >
       foundBuilding->floors[foundBuilding->nbFloors - 1]->topSize)) {
    printf("ERROR: Floor is too big to be placed there\n");
    return stash;
  }
  Building **updatedBuildings = malloc(2 * sizeof(Building *));
  moveFloor(updatedBuildings, stash, city->buildings[buildingNb]);
  city->buildings[buildingNb] = updatedBuildings[1];
  return updatedBuildings[0];
}

Building *makeBuildingNFloors(int n) {
  Building *building = makeBuilding(0, 0, n);
  int floorSize = n;
  for (int i = 0; i < n; i++) {
    Floor *newFloor = makeFloor(floorSize, floorSize - 1, 0);
    building->floors[i] = newFloor;
    floorSize--;
  }
  return building;
}
