uniform vec3 lightPosition;  
varying vec3 n, eyeVec, lightVec, vertPos, lightPos;  
   
void main()  
{  
    vec4 pos = gl_ModelViewProjectionMatrix * gl_Vertex;  

    n        = gl_NormalMatrix * gl_Normal;  
	lightVec = normalize ( lightPosition - pos.xyz );
	vertPos  = pos.xyz;  
	lightPos = lightPosition;  
	eyeVec   = normalize ( -pos.xyz );
   
     gl_Position    = pos;  
}  
