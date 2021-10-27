#version 330 core

in vec3 color;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
    // ambient light
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
    vec3 light = ambient * objectColor;

    // multiply light and texture
    fragColor = vec4(light, 1.0) * mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
}