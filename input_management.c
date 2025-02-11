#include <raylib.h>
#include <math.h>
#include "input_management.h"

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

void updateCursorBuildingCoordinates(Camera camera, City *city, int *cursorTileX, int *cursorTileZ) {
  for (int i = 0; i < city->nbBuildings; i++) {
    Building *current = city->buildings[i];
    int bX = current->positionX;
    int bZ = current->positionZ;
    Vector2 vertexArray[4] = {
      GetWorldToScreen((Vector3) {(float) bX, 0.0f, (float) bZ}, camera),
      GetWorldToScreen((Vector3) {(float) bX+1, 0.0f, (float) bZ}, camera),
      GetWorldToScreen((Vector3) {(float) bX+1, 0.0f, (float) bZ+1}, camera),
      GetWorldToScreen((Vector3) {(float) bX, 0.0f, (float) bZ+1}, camera)
    };
    if (CheckCollisionPointPoly(GetMousePosition(), vertexArray, 4)) {
      *cursorTileX = bX;
      *cursorTileZ = bZ;
      break;
    }
  }
}
