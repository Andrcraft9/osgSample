// Phong vertex shader

varying	vec3 LightDir;
varying	vec3 ViewDir;
varying	vec3 Normal;

void main(void)
{
	vec3 pos = vec3(gl_ModelViewMatrix * gl_Vertex);

	LightDir = normalize(vec3(gl_LightSource[0].position) - pos);				
	ViewDir = normalize(-pos);					
	Normal = normalize(gl_NormalMatrix * gl_Normal);

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
