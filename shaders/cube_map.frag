
varying vec3 ReflectDir;
varying vec3 RefractDir;

uniform samplerCube CubeMapTex;

uniform float ReflectFactor;

void main (void)  
{
	vec4 reflectColor = texture(CubeMapTex, ReflectDir);
	vec4 refractColor = texture(CubeMapTex, RefractDir);

	gl_FragColor = mix(refractColor, reflectColor, ReflectFactor);
	//gl_FragColor = mix(vec4(0,0,0,1), reflectColor, ReflectFactor);
}  

