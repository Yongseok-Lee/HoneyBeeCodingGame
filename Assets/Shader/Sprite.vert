#version 430
 
in vec4 vPosTex;
in vec4 vColor;
out vec4 fColor;
out vec2 fTexcoord; // texture coordinates

uniform mat4 T; 

void main() {
	gl_Position = T * vec4(vPosTex.xy, 0.0, 1.0);
	fColor = vColor;
	fTexcoord = vPosTex.zw;
}
