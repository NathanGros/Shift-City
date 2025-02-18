#include <stdlib.h>
#include "structures.h"

Floor* makeFloor(int bottomSize, int topSize, int nbLinks) {
    Floor *floor = malloc(sizeof(Floor));
    floor->model = malloc(100 * sizeof(char));
    floor->bottomSize = bottomSize;
    floor->topSize = topSize;
    floor->nbLinks = nbLinks;
    floor->links = malloc(nbLinks * sizeof(Floor*));
    return floor;
}

void freeFloor(Floor *floor) {
    /*free(floor->model); // segfault ???*/
    free(floor->links);
    free(floor);
}

Building* makeBuilding(int positionX, int positionZ, int nbFloors) {
    Building *building = malloc(sizeof(Building));
    building->positionX = positionX;
    building->positionZ = positionZ;
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

Objective* makeObjective(int reward) {
  Objective *objective = malloc(sizeof(Objective));
  objective->state = -1;
  objective->reward = reward;
  objective->building = malloc(sizeof(Building));
  return objective;
}

void freeObjective(Objective *objective) {
  freeBuilding(objective->building);
  free(objective);
}

ObjectiveRow *makeObjectiveRow(int nbObjectives) {
  ObjectiveRow *objectiveRow = malloc(sizeof(ObjectiveRow));
  objectiveRow->nbObjectives = nbObjectives;
  objectiveRow->objectives = malloc(nbObjectives * sizeof(Objective*));
  return objectiveRow;
}

void freeObjectiveRow(ObjectiveRow *objectiveRow) {
  for (int i = 0; i < objectiveRow->nbObjectives; i++) {
    freeObjective(objectiveRow->objectives[i]);
  }
  free(objectiveRow->objectives);
  free(objectiveRow);
}

AllObjectives *makeAllObjectives(int nbRows) {
  AllObjectives *allObjectives = malloc(sizeof(AllObjectives));
  allObjectives->nbRows = nbRows;
  allObjectives->objectiveRows = malloc(nbRows * sizeof(ObjectiveRow));
  return allObjectives;
}

void freeAllObjectives(AllObjectives *allObjectives) {
  for (int i = 0; i < allObjectives->nbRows; i++) {
    freeObjectiveRow(allObjectives->objectiveRows[i]);
  }
  free(allObjectives->objectiveRows);
  free(allObjectives);
}
