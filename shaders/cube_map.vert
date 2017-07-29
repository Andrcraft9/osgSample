
varying vec3 ReflectDir;
varying vec3 RefractDir;

uniform float Eta;

void main()
{
	vec4 viewEye = vec4(0.0, 0.0, 0.0, 1.0); 

	vec3 modelPos = vec3(gl_Vertex); 
	vec3 modelNorm = normalize(gl_Normal); 
	vec3 modelEye = vec3(gl_ModelViewMatrixInverse * viewEye); 
	vec3 modelView = normalize(modelEye - modelPos); 
	vec3 modelReflectDir = reflect(-modelView, modelNorm); 
	vec3 modelRefractDir = refract(-modelView, modelNorm, Eta);

	ReflectDir = modelReflectDir;
	RefractDir = modelRefractDir;
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
