#version 410
 	layout(location=0) in vec4 position;
	layout(location=1) in vec4 colour;
	layout(location=2) in vec4 Normal;
	layout(location=3) in vec2 UV;
	layout(location=4) in vec4 Tangent;


	out vec4 vNormal;
	out vec4 vPosition;
	
	uniform mat4 projectionViewModel;
	
	
	void main()
	 {
		vNormal = colour;
		vPosition = position;
		gl_Position =	projectionViewModel * position;
	 }