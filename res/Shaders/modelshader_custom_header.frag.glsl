/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#version 330 core

out vec4 frag_Color;

// model inputs
in vec3 _vf_ModelPosition;
in vec3 _vf_Position;
in vec3 _vf_Normal;
in vec2 _vf_Texture;
in vec4 _vf_Material;
in vec4 _vf_Color;

// material parameters
uniform sampler2D _ambientTexture;
uniform sampler2D _diffuseTexture;
uniform sampler2D _specularTexture;

#pragma include Shaders_lights_glsl
