#version 330 core

in vec3 vf_Position;
in vec2 vf_TextureCoordinates;
flat in int vf_TextureIndex;

out vec4 frag_Color;

uniform sampler2DArray textures;

void main(void) {
    frag_Color = texture(textures, vec3(vf_TextureCoordinates, vf_TextureIndex));
}
