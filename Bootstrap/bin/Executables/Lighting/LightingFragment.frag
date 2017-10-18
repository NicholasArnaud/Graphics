#version 410 

uniform vec3 CameraPos;


uniform	vec3 direction;
uniform	vec3 Id;
uniform vec3 Is;
uniform	vec3 Ia;



uniform	vec3 Ka;
uniform	vec3 Kd;
uniform vec3 Ks;

uniform float specPow;

in vec4 vNormal;
in vec4 vPosition;
in vec4 vColor;
in vec2 vUV;
in vec4 vTangent;


out vec4 FragColor;

void main() 
{ 
	//Ambient

	vec3 ambient = Ka * Ia;

	//Diffuse

	vec3 L = normalize(direction.xyz);
	vec3 N = normalize(vNormal.xyz);

	float lambertDiffuse = max(0.0f,dot(N, -L));
	vec3 diffuse = Kd * Id* lambertDiffuse;

	//Specular

	vec3 R = reflect(L,N);
	vec3 V = normalize(CameraPos.xyz - vPosition.xyz);
	//float specularPower = 20;

	float influence = pow(max(0,dot(R, V)), specPow);
	vec3 specular = Ks * Is * influence;

	//Lighting

	FragColor = vec4(ambient + diffuse+ specular,1);
}