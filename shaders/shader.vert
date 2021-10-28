#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 texCoord;
out vec3 normal;
out vec3 fragPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
	
void main() {
	// read the multiplication from right to left
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	texCoord = aTexCoord;
	// we need the world fragment position and the normal for diffuse light calculation
	// multiply the normal vector with a normal matrix so world space is also applied to the normal vector and non-uniform scaling works
	// NOTE: Inversing matrices is a costly operation for shaders, so wherever possible try to avoid doing inverse operations since they have to be done on each vertex of your scene. For learning purposes this is fine, but for an efficient application you'll likely want to calculate the normal matrix on the CPU and send it to the shaders via a uniform before drawing (just like the model matrix).
	normal = mat3(transpose(inverse(model))) * aNormal;
	fragPosition = vec3(model * vec4(aPos, 1.0));
}