#version 330 core
in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D texture1;

void main() {
	vec4 texColor = texture(texture1, texCoords);

	if (texColor.a < 0.1) {
		// ensures the fragment will not be further processed and thus not end up into the color buffer
		//discard;
	}

	fragColor = texColor;
}
