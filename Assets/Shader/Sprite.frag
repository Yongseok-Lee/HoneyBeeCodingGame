#version 430

in vec2 fTexcoord; 
in vec4 fColor;
out vec4 FragColor;

uniform int wire_mode;
uniform sampler2D sampler;

void main()
{
	switch(wire_mode)
	{
		case 0:   // pass the input fragment color as it is.  
		FragColor = fColor;
		FragColor *= texture(sampler, fTexcoord);
		break; 

		case 1:		// wireframe color
		{
			FragColor = vec4(0, 0, 0, fColor.a);
		}
		break; 
	}
} 