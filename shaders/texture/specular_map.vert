attribute vec3 Tangent;

varying	vec3 LightDir; // vector to light in tangent space
varying	vec3 ViewDir; // vector to eye in tangent space

void main(void)
{
	vec3 norm = normalize(gl_NormalMatrix * gl_Normal);
	vec3 tang = normalize(gl_NormalMatrix * Tangent);
	vec3 binorm = normalize(cross(norm, tang));

	mat3 TBN = mat3(tang, binorm, norm);
	TBN = transpose(TBN);

	vec3 pos = vec3(gl_ModelViewMatrix * gl_Vertex);

	LightDir = normalize(TBN * normalize(vec3(gl_LightSource[0].position) - pos));
	ViewDir = normalize(TBN * normalize(-pos));

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
