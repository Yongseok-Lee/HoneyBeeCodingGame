#version 430

in vec4 vPosition; // vertex position in the modeling coordinate system (MS)
in vec4 vColor; // vertex color
in vec2 vTexcoord; // texture coordinates
in vec4 vNormal; // normal in the MS

out vec3 fPosition;
out vec4 fColor;
out vec3 fNormal;
out vec2 fTexcoord; // texture coordinates

uniform mat4 P; // projection matrix
uniform mat4 V; // viewing transformation matrix
uniform mat4 VM; // VM = V*M
uniform mat4 U; // Transformation matrix for normals (= transpose(inverse(VM))) 

void main() { 
	vec3 vPosition_cs = vec3(VM * vPosition);
	gl_Position = P * vec4(vPosition_cs, 1);
	fPosition = vPosition_cs;
	fNormal = vec3(normalize(U * vNormal));
	fColor = vColor;
	fTexcoord = vTexcoord;
}
