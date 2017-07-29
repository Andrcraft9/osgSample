
varying	vec3 LightVector;
varying	vec3 HalfVector;
varying	vec3 Normal;
varying vec3 EyeVector;

varying vec4 ProjCoords;

varying vec2 v_blurTexCoords[14];
 

void main()
{
	vec3 vertexPos = vec3(gl_ModelViewMatrix * gl_Vertex);	// transformed vertex to view system coordinates

	LightVector = normalize(vec3(gl_LightSource[1].position) - vertexPos); // vector to light source
	EyeVector = normalize(- vertexPos);	// vector to the eye
	HalfVector = normalize(LightVector + EyeVector); // half vector
	Normal = normalize(gl_NormalMatrix * gl_Normal); // noraml vector

    gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
	ProjCoords = gl_Position;

	// BLUR 
	vec2 v_texCoord = ProjCoords.xy / ProjCoords.w;
    v_blurTexCoords[ 0] = v_texCoord + vec2(-0.028, 0.0);
    v_blurTexCoords[ 1] = v_texCoord + vec2(-0.024, 0.0);
    v_blurTexCoords[ 2] = v_texCoord + vec2(-0.020, 0.0);
    v_blurTexCoords[ 3] = v_texCoord + vec2(-0.016, 0.0);
    v_blurTexCoords[ 4] = v_texCoord + vec2(-0.012, 0.0);
    v_blurTexCoords[ 5] = v_texCoord + vec2(-0.008, 0.0);
    v_blurTexCoords[ 6] = v_texCoord + vec2(-0.004, 0.0);
    v_blurTexCoords[ 7] = v_texCoord + vec2( 0.004, 0.0);
    v_blurTexCoords[ 8] = v_texCoord + vec2( 0.008, 0.0);
    v_blurTexCoords[ 9] = v_texCoord + vec2( 0.012, 0.0);
    v_blurTexCoords[10] = v_texCoord + vec2( 0.016, 0.0);
    v_blurTexCoords[11] = v_texCoord + vec2( 0.020, 0.0);
    v_blurTexCoords[12] = v_texCoord + vec2( 0.024, 0.0);
    v_blurTexCoords[13] = v_texCoord + vec2( 0.028, 0.0);
}