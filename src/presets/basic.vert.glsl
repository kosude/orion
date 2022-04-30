#version 330 core

// ORION_VERTEX_SHADER_BASIC

// ---------------------
// Orion Basic Resources
//      Shader Presets
//          Basic Vertex Shader
// ---------------------

struct Transform {
    mat4 mvp;                                   // model-view-projection matrix (multiplied in reverse)
};

layout (location = 0) in vec3 vertexPosIn;      // normalised position of the vertex
layout (location = 1) in vec2 texCoordIn;       // texture coordinate for the vertex
layout (location = 2) in vec4 vertexColourIn;   // colour value of the vertex

out vec2 texCoord;                              // texture coordinate
out vec4 vertexColour;                          // colour value

uniform Transform transform;

void main() {
    // Output data
    texCoord = texCoordIn;
    vertexColour = vertexColourIn;

    // Output position
    gl_Position = transform.mvp * vec4(vertexPosIn, 1.0);
}
