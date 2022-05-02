#version 330 core

// ORION_FRAGMENT_SHADER_LIGHTING

// ---------------------
// Orion Basic Resources
//      Shader Presets
//          Lighting-enabled Fragment Shader
// ---------------------

struct Material {
    sampler2D tex;                              // texture
    vec3 colour;                                // colour of the material
    int shininess;                              // specular exponent ('shininess') of the material

    sampler2D specularTex;                      // specular map texture
};

struct LightSource {
    vec3 colour;                                // colour of the light source
    vec3 pos;                                   // position of the light source

    float brightness;                           // general intensity ('brightness') of the light source
    float ambientIntensity;                     // intensity of ambient light
    float diffuseIntensity;                     // intensity of diffused light
    float specularIntensity;                    // intensity of specular highlights
};

struct Viewport {
    vec3 pos;                                   // world position of the hypothetical 'camera', for specular highlights
};

out vec4 fragColour;                            // final colour of the fragment

in vec2 texCoord;                               // texture coordinate
in vec3 normal;                                 // normal vector output
in vec3 fragPos;                                // world position of the fragment (vertex position multiplied by model matrix)

uniform Material material;
uniform LightSource lightSource;
uniform Viewport view;

void main() {
    // ----------------------------
    // DIFFUSE LIGHT
    // ----------------------------

    // normalised normal vector
    vec3 normalisedNormal = normalize(normal);
    // direction vector between the fragment and the light source
    vec3 dirToLight = normalize(lightSource.pos - fragPos);

    // Get the diffuse impact of the light on the current fragment (clamped to absolute values), based on the direction from the fragment to the light source.
    // Use this to get the diffuse light vector.
    float diffuse = max(dot(normalisedNormal, dirToLight), 0.0) * lightSource.diffuseIntensity;

    // ----------------------------
    // SPECULAR LIGHT
    // ----------------------------

    // direction vector between the fragment and the eye ('camera')
    vec3 dirToEye = normalize(view.pos - fragPos);
    // get the reflect vector taking the direction to the light source as the vector of incidence
    vec3 reflectedDirToLight = reflect(-dirToLight, normalisedNormal);

    // calculate the absolute dot product of the direction to the eye and the light source direction reflect vector
    // raise this to the specular exponent to replicate material shininess.
    float specular = pow(abs(dot(dirToEye, reflectedDirToLight)), material.shininess) * lightSource.specularIntensity;

    // ----------------------------
    // FINAL COLOUR AND APPLICATION
    // ----------------------------

    vec3 result = (
        lightSource.ambientIntensity    * material.colour    * vec3(texture(material.tex, texCoord)) +
        diffuse                         * material.colour    * vec3(texture(material.tex, texCoord)) +
        specular                        * material.colour    * vec3(texture(material.specularTex, texCoord))
    ) * lightSource.colour              * lightSource.brightness;

    fragColour = texture(material.tex, texCoord) * vec4(result, 1.0);
}
