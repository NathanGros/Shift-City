#include <stdio.h>
#include "city_manipulation.h"

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
