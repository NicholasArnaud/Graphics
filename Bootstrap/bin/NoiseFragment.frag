#version 410
in vec2 vTexCoord; 
out vec4 fragColour;
uniform sampler2D perlinTexture; 
void main() 
{ 
	fragColour = texture(perlinTexture, vTexCoord).rrrr; 
	fragColour.a = 1; 
}