attribute vec3 Tangent;

varying	vec3 LightDir; // vector to light in tangent space
varying	vec3 ViewDir; // vector to eye in tangent space
varying vec3 HalfDir;

void main(void)
{
	vec3 norm = normalize(gl_NormalMatrix * gl_Normal);
	vec3 tang = normalize(gl_NormalMatrix * Tangent);
	vec3 binorm = normalize(cross(norm, tang));

	mat3 TBN = mat3(tang, binorm, norm);
	TBN = transpose(TBN);
	
    vec3 pos = vec3(gl_ModelViewMatrix * gl_Vertex);

	vec3 l = normalize(vec3(gl_LightSource[0].position) - pos);
	vec3 v = normalize(-pos);

	LightDir = normalize(TBN * l );
	ViewDir = normalize(TBN * v);
	HalfDir = TBN * (l + v);

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}