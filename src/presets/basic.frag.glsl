#version 330 core

// ORION_FRAGMENT_SHADER_BASIC

// ---------------------
// Orion Basic Resources
// 		Shader Presets
//			Basic Fragment Shader
// ---------------------

struct Blend {
	sampler2D tex;								// texture
	vec4 colour;								// colour value of the overall geometry
	int mode;									// way to blend colours
};

out vec4 fragColour;							// final colour of the fragment

in vec2 texCoord;								// texture coordinate
in vec4 vertexColour;							// colour value of the vertex

uniform Blend blend;

void main() {
	// blend based on blend.mode
	// 1 = solid colour only
	// 2 = texture only
	// 3 = gradient (vertex colours) only
	// 4 = solid colour and texture
	// 5 = solid colour and gradient
	// 6 = texture and gradient
	// 7 = all colour sources
	switch (blend.mode) {
		case 1:
			fragColour = blend.colour;
			break;
		case 2:
			fragColour = texture(blend.tex, texCoord);
			break;
		case 3:
			fragColour = vertexColour;
			break;
		case 4:
			fragColour = blend.colour * texture(blend.tex, texCoord);
			break;
		case 5:
			fragColour = blend.colour * vertexColour;
			break;
		case 6:
			fragColour = texture(blend.tex, texCoord) * vertexColour;
			break;
		case 7:
			fragColour = blend.colour * texture(blend.tex, texCoord) * vertexColour;
			break;
		default:
			fragColour = vec4(1, 0, 1, 1);	// magenta
	}
}