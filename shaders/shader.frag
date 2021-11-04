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

uniform vec3 cameraPosition;

void main() {
    // We do lighting currently in world space but is more common to do it in view space as you get the view/camera position for free. It is always (0, 0, 0) in view space.
    // ambient lighting
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse lighting (normalize vectors so the calculation gets easier)
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(lightPosition - fragPosition);
    // calculate the light angle and multiply it with the light color
    vec3 diffuse = max(dot(norm, lightDirection), 0.0) * lightColor;

    // specular lighting
    float specularStrength = 0.5;
    int shininess = 32;
    vec3 viewDirection = normalize(cameraPosition - fragPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // create phong lighting
    vec3 lighting = (ambient + diffuse + specular) * objectColor;

    // multiply lighting and only one texture
    fragColor = vec4(lighting, 1.0) * texture(texture1, texCoord);

    // multiply lighting and multiple combined textures
    // fragColor = vec4(lighting, 1.0) * mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
}