#version 410 

uniform vec3 CameraPos;


vec3 direction;
vec3 Id;
vec3 Is;
vec3 Ia;



vec3 Ka;
vec3 Kd;
vec3 Ks;

float specPow;
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec4 vNormal;
in vec4 vPosition;
in vec4 vColor;
in vec2 vUV;
in vec4 vTangent;


out vec4 FragColor;

void main() 
{ 
	Id,Ia,Is = vec3(1);
	direction = vec3(1);
	Ka = vec3(1,1,0);
	Kd, Ks = vec3(1);
	specPow = 5;

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

	float influence = pow(max(0,dot(R, V)), specPow);
	vec3 specular = Ks * Is * influence;

	//Lighting w/ Texture

	FragColor =mix(texture(texture1, vUV), texture(texture2, vUV), .5f) * vec4(ambient + diffuse+ specular,1);

}