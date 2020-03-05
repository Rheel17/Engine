#version 330 core

layout (location = 0) in vec3 vert_Position;
layout (location = 1) in vec3 vert_Texture;

out vec3 vf_Position;
out vec2 vf_TextureCoordinates;
flat out int vf_TextureIndex;

uniform mat4 projectionMatrix;
uniform mat4 rotationMatrix;
uniform float renderDistance;
uniform float scale;

void main(void) {
    mat4 projMatrix = projectionMatrix;
    projMatrix[0][0] *= scale;
    projMatrix[1][1] *= scale;

    gl_Position = projMatrix * inverse(rotationMatrix) * vec4(vert_Position * renderDistance, 1.0);

    vf_Position = vert_Position;
    vf_TextureCoordinates = vert_Texture.xy;
    vf_TextureIndex = int(vert_Texture.z + 0.2);
}
