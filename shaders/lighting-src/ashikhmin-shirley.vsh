//
// Ashikhmin-Shirley vertex shader
//

varying	vec3 l;
varying	vec3 h;
varying vec3 v;
varying	vec3 t;
varying vec3 b;
varying	vec3 n;

uniform	vec4	lightPos;
uniform	vec4	eyePos;

void main(void)
{
	vec3	p = vec3 ( gl_ModelViewMatrix * gl_Vertex );				// transformed point to world space
	
	n = normalize( gl_NormalMatrix * gl_Normal );				// transformed n
	t = normalize( gl_NormalMatrix * gl_MultiTexCoord1.xyz );	// transformed t
	b = normalize( gl_NormalMatrix * gl_MultiTexCoord2.xyz );	// transformed b
	l = normalize ( vec3 ( lightPos ) - p );					// vector to light source
	v = normalize ( vec3 ( eyePos )   - p );					// vector to the eye
	h = normalize ( l + v );

	gl_Position     = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord [0] = gl_MultiTexCoord0;
}
