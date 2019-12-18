#version 430

in vec2 fTexcoord; 
in vec4 fColor;
out vec4 FragColor;

uniform int ColorMode;
uniform sampler2D sampler;

void main()
{
	switch(ColorMode)
	{
		case 0:   // pass the input fragment color as it is.  
		FragColor = fColor;
		FragColor *= texture(sampler, fTexcoord);
		break; 

		case 1:
		{
			FragColor = fColor;
		}
		break; 
	}
} 