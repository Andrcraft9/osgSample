//
// Lommel-Seeliger fragment shader
//

varying	vec3 l;
varying	vec3 h;
varying vec3 v;
varying vec3 n;

void main (void)
{
	const vec4	diffColor = vec4 ( 0.5, 0.0, 0.0, 1.0 );
	const vec4	specColor = vec4 ( 0.7, 0.7, 0.0, 1.0 );
	const float	specPower = 30.0;

	vec3	n2   = normalize ( n );
	vec3	l2   = normalize ( l );
	vec3	v2   = normalize ( v );
	float	a    = max ( 0.0, dot       ( n2, l2 ) );
	float	b    = max ( 0.0, dot       ( n2, v2 ) );

	gl_FragColor = diffColor * a / (a + b);
}
