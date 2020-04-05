/*
 * Copyright (c) 2020 Levi van Rheenen
 */
uniform vec3 parameters;
uniform vec4 color;

void main(void) {
	vec2 uv = vf_Coordinates;
	uv.x = pow(abs(uv.x), parameters.z);
	uv.y = pow(abs(uv.y), parameters.z);

	float f = smoothstep(parameters.x, parameters.y, dot(uv, uv));

	frag_Color = color;
	frag_Color.a *= f;
}