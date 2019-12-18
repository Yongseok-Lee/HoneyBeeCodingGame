#version 430


flat out vec4 fColorFront;
flat out vec4 fColorBack;
out vec2 fTexcoord; // texture coordinates

in vec4 vPosition;
in vec4 vNormal;
in vec4 vColor;
in vec2 vTexcoord; // texture coordinates

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float n;
uniform vec3 LightPos_cs;
uniform vec3 Il;
uniform vec3 Ia;
uniform vec3 c;

uniform mat4 P;
uniform mat4 V;
uniform mat4 VM;
uniform mat4 U;
uniform int ColorMode;
vec4 shading(vec3 vPosition_cs, vec3 vNormal_cs);

void main()
{
	vec3 vNormal_cs = vec3(normalize(U * vNormal));
	vec3 vPosition_cs = vec3(VM * vPosition);
	gl_Position = P * vec4(vPosition_cs, 1);
	fTexcoord = vTexcoord;

	if (ColorMode == 1) {
		fColorFront = vColor;
		return;
	}
	fColorFront = shading(vPosition_cs, vNormal_cs);
	fColorBack = shading(vPosition_cs, -vNormal_cs);
}

vec4 shading(vec3 vPosition_cs, vec3 vNormal_cs)
{
	vec3 N = normalize(vNormal_cs);
	vec3 L = LightPos_cs-vPosition_cs;
	float d = length(L); L = L/d;
	vec3 V = normalize(vec3(0.0) -vPosition_cs);
	vec3 R = reflect(-L, N);
	float fatt = min(1.0/ (c[0] + c[1]*d + c[2]*d*d), 1.0);
	float cos_theta= max(dot(N,L),0);
	float cos_alpha= max(dot(V,R),0);
	vec3 I = Ia* Ka+ fatt* Il *
	(Kd* cos_theta+ Ks * pow(cos_alpha, n)) * vec3(vColor);
	return vec4(I,1);
}