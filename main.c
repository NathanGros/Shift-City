#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <raylib.h>



//Init

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
    camera->target.x += cameraVectorX / cameraDistanceToTarget;
    camera->target.z += cameraVectorZ / cameraDistanceToTarget;
  }
  if (IsKeyDown(KEY_S)) {
    camera->target.x -= cameraVectorX / cameraDistanceToTarget;
    camera->target.z -= cameraVectorZ / cameraDistanceToTarget;
  }
  if (IsKeyDown(KEY_A)) {
    camera->target.x += cameraVectorZ / cameraDistanceToTarget;
    camera->target.z += -1. * cameraVectorX / cameraDistanceToTarget;
  }
  if (IsKeyDown(KEY_D)) {
    camera->target.x += -1. * cameraVectorZ / cameraDistanceToTarget;
    camera->target.z += cameraVectorX / cameraDistanceToTarget;
  }

  // camera angle
  if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
    *horizontalAngle += GetMouseDelta().x * 0.03;
    *verticalAngle += GetMouseDelta().y * 0.03;
    if (*verticalAngle >= pi / 2.) *verticalAngle = pi / 2.;
    if (*verticalAngle <= 0.) *verticalAngle = 0.;
  }

  // camera zoom
  *targetDistance *= 1. - 0.3 * GetMouseWheelMove();
  if (*targetDistance <= 4.) *targetDistance = 4.;
  if (*targetDistance >= 50.) *targetDistance = 50.;

  // camera position
  camera->position.x = camera->target.x + *targetDistance * cos(*horizontalAngle) * cos(*verticalAngle);
  camera->position.y = camera->target.y + *targetDistance * sin(*verticalAngle);
  camera->position.z = camera->target.z + *targetDistance * sin(*horizontalAngle) * cos(*verticalAngle);
}



//Main

int main() {
  //Init
  Color backgroundColor = (Color) {200, 200, 200, 255};
  Init(backgroundColor);
  ToggleFullscreen();
  DisableCursor();
  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();

  // Camera control
  float pi = 3.141592;
  float verticalAngle = pi / 4.;
  float horizontalAngle = pi / 4.;
  float targetDistance = 20.;
  float speed = 1.;
  Camera3D camera = {0};
  camera.position = (Vector3) {0.0f, 0.0f, 0.0f};
  camera.target = (Vector3) {0.0f, 0.0f, 0.0f};
  camera.up = (Vector3) {0.0f, 1.0f, 0.0f}; // Camera up vector (rotation towards target)
  camera.fovy = 70.0f; // Camera field-of-view Y
  camera.projection = CAMERA_PERSPECTIVE;

  Vector3 cubePosition = {0.0f, 0.0f, 0.0f};

  while (!WindowShouldClose()) {
    //Resize
    if (IsWindowResized()) {
      screenWidth = GetScreenWidth();
      screenHeight = GetScreenHeight();
    }

    updateCamera(&camera, pi, speed, &verticalAngle, &horizontalAngle, &targetDistance);

    //Drawing
    BeginDrawing();
      ClearBackground(backgroundColor);
      BeginMode3D(camera);
        DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
        DrawGrid(1000, 1.0f);
      EndMode3D();
    EndDrawing();
  }
  CloseWindow();

  //De-init
  return 0;
}
