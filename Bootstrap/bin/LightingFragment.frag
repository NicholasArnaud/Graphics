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
	vec3 L = normalize(direction);
	vec3 N = normalize(vNormal.xyz);


	vec3 ambient = Ka * Ia;

	float lambertDiffuse = max(0.0f,dot(N, -L));
	vec3 diffuse = Kd * Id* lambertDiffuse;


	vec3 R = reflect(L,N);
	vec3 V = normalize(CameraPos.xyz - vPosition.xyz);

	//float SpecSharp = pow(max(0.0f,dot(R, V)), SpecPow);
	//vec3 specular = Ks * Is * SpecSharp;
	//Ks = vec3(1);
    //Is = vec3(1);
    //int specularPower = 20; 
    //vec3 cam = vec3(10, 0, 1);
    //specular not using blinn-phong
    //R = reflect(L, N);
    //V = normalize(cam - vPosition.xyz);
    
    //float RdotV = dot(R, V);
    //float influnce = max(0, RdotV);
    //float specularTerm = pow(influnce, specularPower);
    //vec3 specular_p = Ks * specularTerm * Is;

	FragColor = vec4(ambient + diffuse+ specular,1);
}