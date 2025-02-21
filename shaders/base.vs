#version 330

in vec3 vertexPosition;  // Input vertex position
in vec3 vertexNormal;    // Input vertex normal

out vec3 fragNormal;     // Pass normal to fragment shader
out vec3 fragPosition;   // Pass position to fragment shader

uniform mat4 mvp;        // Model-View-Projection matrix

void main() {
    fragNormal = normalize(vertexNormal);
    fragPosition = vertexPosition;
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
