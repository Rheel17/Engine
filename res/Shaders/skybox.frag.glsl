#version 330 core

in vec3 vf_Position;
in vec2 vf_TextureCoordinates;
flat in int vf_TextureIndex;

out vec4 frag_Color;

uniform sampler2D textures[6];

void main(void) {
    frag_Color = texture2D(textures[vf_TextureIndex], vf_TextureCoordinates);
}
