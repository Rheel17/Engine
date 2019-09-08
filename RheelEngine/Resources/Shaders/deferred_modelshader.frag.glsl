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
in vec4 vf_Material;
in vec4 vf_Color;

uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

void main(void) {
	frag_Color = vec4(0, 0, 0, -1);
	frag_Position = vf_Position;
	frag_Normal = normalize(vf_Normal);
	
	if (vf_Color.a < 0) {
		frag_Ambient = vf_Material.x * texture(ambientTexture, vf_Texture).rgb;
		frag_Diffuse = vf_Material.y * texture(diffuseTexture, vf_Texture).rgb;
		frag_Specular = vf_Material.z * texture(specularTexture, vf_Texture).rgb;
	} else {
		vec3 albedo = vf_Color.rgb;

		frag_Ambient = vf_Material.x * albedo;
		frag_Diffuse = vf_Material.y * albedo;
		frag_Specular = vf_Material.z * albedo;
	}

	frag_MaterialParameters = vec4(vf_Material.w, 0.0, 0.0, 0.0);
}
