

varying vec3 ReflectDir;

void main()
{
	ReflectDir = vec3( gl_Vertex );
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
