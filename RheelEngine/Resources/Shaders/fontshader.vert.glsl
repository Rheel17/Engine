#version 330 core

// per-vertex data
layout (location = 0) in vec2 vert_Position;

// per-instance data
layout (location = 1) in vec4 inst_TextureData;
layout (location = 2) in vec4 inst_CharacterData;

out vec2 vf_Texture;

void main(void) {
	vec2 position = vert_Position * inst_CharacterData.zw + inst_CharacterData.xy;

	vf_Texture = vert_Position;
	vf_Texture.y = 1 - vf_Texture.y;
	vf_Texture *= inst_TextureData.zw;
	vf_Texture += inst_TextureData.xy;

	gl_Position = vec4(position, 0.0, 1.0);
}
