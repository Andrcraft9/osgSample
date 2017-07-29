// Blinn vertex shader

varying	vec3 LightVector;
varying	vec3 HalfVector;
varying	vec3 Normal;
varying vec3 EyeVector;

uniform int LightNum;

void main(void)
{
	vec3 vertexPos = vec3(gl_ModelViewMatrix * gl_Vertex);	// transformed vertex to view system coordinates

	LightVector = normalize(vec3(gl_LightSource[LightNum].position) - vertexPos); // vector to light source
	EyeVector = normalize(- vertexPos);	// vector to the eye
	HalfVector = normalize(LightVector + EyeVector); // half vector
	Normal = normalize(gl_NormalMatrix * gl_Normal); // noraml vector

	gl_TexCoord[0] = gl_MultiTexCoord0;
	//gl_FrontColor = gl_Color;
	//gl_BackColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

}
