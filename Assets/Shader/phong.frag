#version 430

in vec2 fTexcoord; 
in vec3 fPosition;
in vec3 fNormal;
in vec4 fColor;
out vec4 FragColor;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float n;
uniform vec3 LightPos_cs;
uniform vec3 Il;
uniform vec3 Ia;
uniform vec3 c;
uniform sampler2D sampler;

uniform int ColorMode;

vec4 shading(vec3 vPosition_cs, vec3 vNormal_cs)
{
	vec3 N = normalize(vNormal_cs);
	vec3 L = LightPos_cs - vPosition_cs;
	float d = length(L); L = L/d; 
	vec3 V = normalize(vec3(0.0) - vPosition_cs); 
	vec3 R = reflect(-L, N);

	float fatt = min(1.0 / (c[0] + c[1]*d + c[2]*d*d), 1.0);

	float cos_theta = max(dot(N,L),0); 
	float cos_alpha = max(dot(V,R),0);

	vec3 I = Ia * Ka + fatt * Il * (Kd * cos_theta + Ks * pow(cos_alpha, n)) * vec3(fColor);
	
	return vec4(I,1);
}

void main() {
	if (ColorMode==1)
		FragColor = fColor;
	if (gl_FrontFacing)
		FragColor = shading(fPosition,  fNormal);
	else
		FragColor = shading(fPosition,  -fNormal);
	FragColor *= texture(sampler, fTexcoord);
}