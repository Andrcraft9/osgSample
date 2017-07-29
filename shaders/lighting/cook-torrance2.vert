
varying	vec3 LightDir;
varying	vec3 HalfVec;
varying	vec3 Normal;
varying vec3 ViewDir;

void main(void)
{
	vec3 pos = vec3(gl_ModelViewMatrix * gl_Vertex);

	LightDir = normalize(vec3(gl_LightSource[0].position) - pos);
	ViewDir = normalize(-pos);
	HalfVec = normalize(LightDir + ViewDir);
	Normal = normalize(gl_NormalMatrix * gl_Normal);

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
