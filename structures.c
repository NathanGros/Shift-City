#include <stdlib.h>
#include "structures.h"

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
