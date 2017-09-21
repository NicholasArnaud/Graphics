#version 410 
in vec4 vNormal;
in vec4 vPosition;


uniform vec3 LightDir;
uniform vec3 LightColor;
uniform vec3 CameraPos;
uniform float SpecPow;


uniform	vec3 direction;
uniform	vec3 Id;
uniform	vec3 Is;
uniform	vec3 Ia;



uniform	vec3 Ka;
uniform	vec3 Kd;
uniform	vec3 Ks;
uniform	float a;


out vec4 FragColor;

void main() 
	{ 
		FragColor = vec4(Id,1);
	}