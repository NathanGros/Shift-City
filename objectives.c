#include "city_manipulation.h"

int compareFloor(Floor *floor1, Floor *floor2) { //TODO when adding shared floors
  if (floor1->bottomSize == floor2->bottomSize
      && floor1->topSize == floor2->topSize
      && floor1->nbLinks == floor2->nbLinks) return 1;
  return 0;
}

int compareBuilding(Building *building1, Building *building2) {
  if (building1->nbFloors != building2->nbFloors) return 0;
  for (int i = 0; i < building1->nbFloors; i++) {
    if (compareFloor(building1->floors[i], building2->floors[i]) == 0) return 0;
  }
  return 1;
}

void checkObjectiveCompletion(Building *building, AllObjectives *allObjectives, int *points) {
  for (int i = 0; i < allObjectives->nbRows; i++) {
    ObjectiveRow *row = allObjectives->objectiveRows[i];
    for (int j = 0; j < row->nbObjectives; j++) {
      Objective *objective = row->objectives[j];
      if (objective->state != 0)
        continue;
      if (compareBuilding(building, objective->building) == 1) {
        // change objective state and next objective state
        objective->state = 1;
        if (j < row->nbObjectives - 1)
          row->objectives[j+1]->state = 0;
        // give points
        *points += objective->reward;
        break;
      }
    }
  }
}

AllObjectives* BuildAllObjectives() {
  // making objectives
  Objective *objective1 = makeObjective(1);
  Building *objective1Building = makeBuildingNFloors(1);
  objective1->building = objective1Building;
  objective1->state = 0;

  Objective *objective2 = makeObjective(1);
  Building *objective2Building = makeBuildingNFloors(3);
  objective2->building = objective2Building;

  Objective *objective3 = makeObjective(1);
  Building *objective3Building = makeBuildingNFloors(4);
  objective3->building = objective3Building;

  Objective *objective4 = makeObjective(1);
  Building *objective4Building = makeBuildingNFloors(6);
  objective4->building = objective4Building;

  Objective *objective5 = makeObjective(1);
  Building *objective5Building = makeBuildingNFloors(2);
  objective5->building = objective5Building;
  objective5->state = 0;

  Objective *objective6 = makeObjective(1);
  Building *objective6Building = makeBuildingNFloors(5);
  objective6->building = objective6Building;

  Objective *objective7 = makeObjective(1);
  Building *objective7Building = makeBuilding(0, 0, 2);
  Floor *floor1 = makeFloor(4, 3, 0);
  floor1->model = "floor_4to3.obj";
  Floor *floor2 = makeFloor(1, 0, 0);
  floor2->model = "floor_1to0.obj";
  objective7Building->floors[0] = floor1;
  objective7Building->floors[1] = floor2;
  objective7->building = objective7Building;
  objective7->state = 0;

  // rows
  ObjectiveRow *row1 = makeObjectiveRow(4);
  ObjectiveRow *row2 = makeObjectiveRow(2);
  ObjectiveRow *row3 = makeObjectiveRow(1);
  row1->objectives[0] = objective1;
  row1->objectives[1] = objective2;
  row1->objectives[2] = objective3;
  row1->objectives[3] = objective4;
  row2->objectives[0] = objective5;
  row2->objectives[1] = objective6;
  row3->objectives[0] = objective7;
  AllObjectives *allObjectives = makeAllObjectives(3);
  allObjectives->objectiveRows[0] = row1;
  allObjectives->objectiveRows[1] = row2;
  allObjectives->objectiveRows[2] = row3;
  return allObjectives;
}
