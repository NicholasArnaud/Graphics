#version 410 

uniform vec3 CameraPos;
uniform float SpecPow;


uniform	vec3 direction;
uniform	vec3 Id;
uniform	vec3 Is;
uniform	vec3 Ia;



uniform	vec3 Ka;
uniform	vec3 Kd;
uniform	vec3 Ks;

in vec4 vNormal;
in vec4 vPosition;
in vec4 vColor;
in vec2 vUV;
in vec4 vTangent;


out vec4 FragColor;

void main() 
{ 
	vec4 L = vec4(direction,1);
	vec4 N = vNormal;

	//algorithm:  Ka*Ia + Kd(LmN)*Id + Ks(RmV)^a *Is\\

	vec4 ambient = vec4(vec4(Ka,1)* vec4(Ia,1));

	float lambertDiffuse = max(0,dot(-L,N));
	vec4 diffuse =  vec4(Kd,1) * lambertDiffuse * vec4(Id,1);


	//float lambertSpec = max(0,dot())

	FragColor = ambient + diffuse;
}