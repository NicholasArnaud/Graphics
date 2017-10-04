#version 410
 	layout(location=0) in vec4 Position;
	layout(location=1) in vec4 Colour;
	layout(location=2) in vec4 Normal;
	layout(location=3) in vec2 UV;
	layout(location=4) in vec4 Tangent;
uniform mat4 projectionView; 
out vec2 vTexCoord; 
uniform sampler2D perlinTexture; 
void main() 
{ 
	vTexCoord = UV;
	vec4 pos = Position; 
	//pos.y += texture(perlinTexture, vTexCoord).r * 5;

	gl_Position = projectionView * pos;
}