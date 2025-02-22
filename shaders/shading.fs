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
    float depth = texture(depthTex, fragTexCoord).r; // Read depth (0 = near, 1 = far)

    // Apply soft shadows based on depth
    float shadowFactor = smoothstep(0.0, 1.0, depth * 5.0);
    color.rgb *= shadowFactor;

    // Add a light beam effect
    vec2 dir = fragTexCoord - lightPos.xy;
    float dist = length(dir);
    float lightRay = exp(-dist * 2.0) * smoothstep(0.4, 0.1, depth);

    color.rgb += lightRay * vec3(1.0, 0.8, 0.5); // Warm light color

    finalColor = color;
}
