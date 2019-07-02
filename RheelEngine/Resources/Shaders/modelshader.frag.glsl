#version 330 core

out layout(location = 0) vec4 frag_Color;
out layout(location = 1) vec3 frag_Position;
out layout(location = 2) vec3 frag_Normal;
out layout(location = 3) vec3 frag_Ambient;
out layout(location = 4) vec3 frag_Diffuse;
out layout(location = 5) vec3 frag_Specular;
out layout(location = 6) vec4 frag_MaterialParameters;

in vec3 vf_Position;
in vec3 vf_Normal;
in vec2 vf_Texture;

void main(void) {
	frag_Color = vec4(0, 0, 0, -1);
	frag_Position = vf_Position;
	frag_Normal = normalize(vf_Normal);
	
	vec3 albedo = vec3(1.0);

	frag_Ambient = 0.01 * albedo;
	frag_Diffuse = 0.99 * albedo;
	frag_Specular = albedo;
	frag_MaterialParameters = vec4(100.0, 0.0, 0.0, 0.0);
}
