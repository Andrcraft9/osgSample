//
// Simple anisotropic lighting vertex shader
// Using lookup table for diffuse and specular coefficients
//

varying	vec3 lt;
varying	vec3 ht;

uniform	vec4	lightPos;
uniform	vec4	eyePos;

void main(void)
{
	vec3	p = vec3      ( gl_ModelViewMatrix * gl_Vertex );			// transformed point to world space
	vec3	l = normalize ( vec3 ( lightPos ) - p );					// vector to light source
	vec3	v = normalize ( vec3 ( eyePos )   - p );					// vector to the eye
	vec3	h = normalize ( l + v );
	vec3	n = gl_NormalMatrix * gl_Normal;							// transformed n
	vec3 	t = gl_NormalMatrix * gl_MultiTexCoord1.xyz;				// transformed t
	vec3 	b = gl_NormalMatrix * gl_MultiTexCoord2.xyz;				// transformed b

																		// now remap l, and h into tangent space
	lt = vec3 ( dot ( l, t ), dot ( l, b ), dot ( l, n ) );
	ht = vec3 ( dot ( h, t ), dot ( h, b ), dot ( h, n ) );

	gl_Position     = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord [0] = gl_MultiTexCoord0;
}
