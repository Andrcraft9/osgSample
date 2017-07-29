uniform float osg_FrameTime;

varying vec4 ProjCoords;
varying vec3 LightDir, EyeDir;
varying vec2 FlowCoords, RippleCoords;

varying vec3 ReflectDir;

void main()
{
	vec3 T = vec3(0.0, 1.0, 0.0);
	vec3 N = vec3(0.0, 0.0, 1.0);
	vec3 B = vec3(1.0, 0.0, 0.0);
	T = normalize(gl_NormalMatrix * T);
	B = normalize(gl_NormalMatrix * B);
	N = normalize(gl_NormalMatrix * N);

	mat3 TBNmat;
	TBNmat[0][0] = T[0]; TBNmat[1][0] = T[1]; TBNmat[2][0] = T[2];
	TBNmat[0][1] = B[0]; TBNmat[1][1] = B[1]; TBNmat[2][1] = B[2];
	TBNmat[0][2] = N[0]; TBNmat[1][2] = N[1]; TBNmat[2][2] = N[2];
	TBNmat = transpose(TBNmat);

	vec3 vertexInEye = vec3(gl_ModelViewMatrix * gl_Vertex);
	LightDir =  gl_LightSource[0].position.xyz - vertexInEye;
	LightDir = normalize(TBNmat * LightDir);
	EyeDir = normalize(TBNmat * (-vertexInEye));

	// Compute cube maps reflect
	vec4 viewEye = vec4(0.0, 0.0, 0.0, 1.0); 
	vec3 modelPos = vec3(gl_Vertex); 

	vec3 modelNorm = normalize(gl_Normal); 
	vec3 modelEye = vec3(gl_ModelViewMatrixInverse * viewEye); 
	vec3 modelView = normalize(modelEye - modelPos); 
	ReflectDir = reflect(-modelView, modelNorm);

	vec2 t1 = vec2(osg_FrameTime*0.02, osg_FrameTime*0.02);
	vec2 t2 = vec2(osg_FrameTime*0.05, osg_FrameTime*0.05);
	FlowCoords = gl_MultiTexCoord0.xy * 5.0 + t1;
	RippleCoords = gl_MultiTexCoord0.xy * 10.0 + t2;

	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	gl_Position = ftransform();

	ProjCoords = gl_Position;

}