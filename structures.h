#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdlib.h>

typedef struct Floor {
  int bottomSize;
  int topSize;
  int nbLinks;
  struct Floor **links; // pointer to a list of other floors' pointers
} Floor;

Floor* makeFloor(int bottomSize, int topSize, int nbLinks);
void freeFloor(Floor *floor);

typedef struct {
  int positionX;
  int positionZ;
  int nbFloors;
  Floor **floors; // pointer to a list of floor pointers
} Building;

Building* makeBuilding(int positionX, int positionZ, int nbFloors);
void freeBuilding(Building *building);

typedef struct {
  int nbBuildings;
  Building **buildings;
} City;

City* makeCity(int nbBuildings);
void freeCity(City *city);

typedef struct {
  int state;
  int reward;
  Building *building;
} Objective;

Objective* makeObjective(int reward);
void freeObjective(Objective *objective);

typedef struct {
  int nbObjectives;
  Objective **objectives;
} ObjectiveRow;

ObjectiveRow *makeObjectiveRow(int nbObjectives);
void freeObjectiveRow(ObjectiveRow *objectiveRow);

typedef struct {
  int nbRows;
  ObjectiveRow **objectiveRows;
} AllObjectives;

AllObjectives *makeAllObjectives(int nbRows);
void freeAllObjectives(AllObjectives *allObjectives);

#endif
