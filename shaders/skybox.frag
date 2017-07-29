

varying vec3 ReflectDir;

uniform samplerCube CubeMapTex;

void main (void)  
{
	vec4 reflect_color = texture(CubeMapTex, ReflectDir);
	
	gl_FragColor = reflect_color;
	
}  
