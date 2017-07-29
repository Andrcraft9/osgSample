//
// Blinn vertex shader
//

varying	vec3 l;
varying	vec3 h;
varying	vec3 n;
varying vec3 v;

uniform	vec4	lightPos;
uniform	vec4	eyePos;

void main(void)
{
	vec3	p = vec3 ( gl_ModelViewMatrix * gl_Vertex );			// transformed point to world space

	l = normalize ( vec3 ( lightPos ) - p );					// vector to light source
	v = normalize ( vec3 ( eyePos )   - p );					// vector to the eye
	h = normalize ( l + v );
	n = normalize ( gl_NormalMatrix * gl_Normal );							// transformed n

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
