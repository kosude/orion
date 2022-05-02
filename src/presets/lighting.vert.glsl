#version 330 core

// ORION_VERTEX_SHADER_LIGHTING

// ---------------------
// Orion Basic Resources
// 		Shader Presets
//			Lighting-enabled Vertex Shader
// ---------------------

struct Transform {
    mat4 model;									// model matrix
    mat4 projView;								// view-projection matrix (multiplied in reverse)
};

layout (location = 0) in vec3 vertexPosIn;		// normalised position of the vertex
layout (location = 1) in vec2 texCoordIn;		// texture coordinate for the vertex
layout (location = 3) in vec3 normalIn;			// normal of the current surface

out vec2 texCoord;								// texture coordinate
out vec3 normal;								// normal vector output
out vec3 fragPos;								// world position of the fragment (vertex position multiplied by model matrix)

uniform Transform transform;

void main() {
    // output data
    texCoord = texCoordIn;
    
    // calculate normal and fragment position
    normal = mat3(transpose(inverse(transform.model))) * normalIn;
    fragPos = vec3(transform.model * vec4(vertexPosIn, 1.0));

    // output vertex position
    gl_Position = transform.projView * transform.model * vec4(vertexPosIn, 1.0);
}
