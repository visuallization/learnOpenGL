#version 330 core

in vec2 texCoord;
in vec3 normal;
in vec3 fragPosition;

out vec4 fragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPosition;

void main() {
    // ambient lighting
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse lighting (normalize vectors so the calculation gets easier)
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(lightPosition - fragPosition);
    // calculate the light angle and multiply it with the light color
    vec3 diffuse = max(dot(norm, lightDirection), 0.0) * lightColor;

    // multiply lighting and texture
    vec3 lighting = (ambient + diffuse) * objectColor;
    fragColor = vec4(lighting, 1.0) * mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
}