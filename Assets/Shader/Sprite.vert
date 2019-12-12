#version 430
 
in vec4 vPosition;
in vec4 vColor;
in vec2 vTexcoord; // texture coordinates
out vec4 fColor;
out vec2 fTexcoord; // texture coordinates

uniform mat4 T; 
 
void main() { 
	gl_Position = T * vPosition; 
	fColor = vColor;
	fTexcoord = vTexcoord;
}
