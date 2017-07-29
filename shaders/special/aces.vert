attribute vec3 Tangent;

varying	vec3 LightVector;
varying	vec3 HalfVector;
varying vec3 Normal;

varying	vec3 LightVectorTBN;
varying	vec3 HalfVectorTBN;

varying vec3 ReflectDir;

void main(void)
{
	vec3 vertexPos = vec3(gl_ModelViewMatrix * gl_Vertex);

	//////////////////// Simple Blinn model lighting. Transform to view coordinate system ////////////////////
	
	LightVector = normalize(vec3(gl_LightSource[2].position) - vertexPos); // vector to light source
	vec3 eyeVector = normalize(-vertexPos);	// vector to the eye
	HalfVector = normalize(LightVector + eyeVector); // half vector
	Normal = normalize(gl_NormalMatrix * gl_Normal); // noraml vector

	///////////////////////////// Bumpmapping. Transform to TBN coordinate system /////////////////////////////
	
	vec3 norm = normalize(gl_NormalMatrix * gl_Normal);
	vec3 tang = normalize(gl_NormalMatrix * Tangent);
	vec3 binorm = normalize(cross(norm, tang));

	mat3 TBN = mat3(tang, binorm, norm);
	TBN = transpose(TBN);

	LightVectorTBN = normalize(TBN * LightVector);
	vec3 eyeVectorTBN = normalize(TBN * eyeVector);
	HalfVectorTBN = normalize(LightVectorTBN + eyeVectorTBN);
	
	///////////////////////////// Reflecrions. Parameters for reflectons /////////////////////////////
	
	// Compute reflect direction
	vec4 viewEye = vec4(0.0, 0.0, 0.0, 1.0); 

	vec3 modelPos = vec3(gl_Vertex); 
	vec3 modelNorm = normalize(gl_Normal); 
	
	vec3 modelEye = vec3(gl_ModelViewMatrixInverse * viewEye); 
	vec3 modelView = normalize(modelEye - modelPos); 
	
	//vec3 modelReflectDir = vec3( RotateMatrix * vec4(reflect(-modelView, modelNorm), 0.0));
	vec3 modelReflectDir = reflect(-modelView, modelNorm);
	
	ReflectDir = modelReflectDir;
	
	//////////////////////////////////////// Out ////////////////////////////////////////
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
	gl_BackColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
