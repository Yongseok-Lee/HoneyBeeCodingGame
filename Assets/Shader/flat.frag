#version 430
flat in vec4 fColorFront;
flat in vec4 fColorBack;
in vec2 fTexcoord; 
uniform int ColorMode;
uniform sampler2D sampler;
out vec4 FragColor;
void main()
{
	if (ColorMode == 1) {
		FragColor = fColorFront;
		return;
	}
	if( gl_FrontFacing)
		FragColor= fColorFront; 
	else
		FragColor= fColorBack;
	FragColor *= texture(sampler, fTexcoord);
}