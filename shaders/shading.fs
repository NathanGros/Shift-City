#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D sceneTex;  // Full rendered scene
uniform sampler2D depthTex;  // Depth texture
uniform vec2 resolution;
uniform vec3 lightPos;       // Light position in screen-space
uniform float time;

void main() {
    vec4 color = texture(sceneTex, fragTexCoord);   // Get scene color
    float depth = texture(depthTex, fragTexCoord).g; // Read depth (green value) (0 = near, 1 = far)

    // Add a light beam effect
    vec2 dir = fragTexCoord - lightPos.xy;
    float dist = length(dir);
    float lightRay = exp(-dist * 10.0) * smoothstep(0.4, 0.1, depth);

    color.rgb += lightRay * vec3(1.0, 1.0, 1.0); // Warm light color

    finalColor = color;
}
