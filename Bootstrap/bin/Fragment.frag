#version 410
	in vec4 vColour;
	out vec4 fragColour;
	void main()
		{
			float d = max(0, dot( normalize(vColour.xyz), vec3(0,1,0)));
			//fragColour = vec4(d, d,d,1);
			fragColour = vColour;
		}