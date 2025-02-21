#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0; // Scene texture
uniform vec2 lightPos; // Light position on screen (0-1)

void main() {
    vec4 sceneColor = texture(texture0, fragTexCoord);
    
    // Calculate distance from light source
    vec2 dir = fragTexCoord - lightPos;
    float dist = length(dir);
    
    // Create a radial light beam effect
    float intensity = exp(-dist * 10.0); // Adjust the 10.0 for different beam sizes
    vec3 lightColor = vec3(1.0, 0.9, 0.6); // Warm light

    // Add light beams to the scene
    sceneColor.rgb += intensity * lightColor;

    finalColor = sceneColor;
}
