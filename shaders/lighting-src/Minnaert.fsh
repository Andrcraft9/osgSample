//
// Minnaert lighting model fragment shader
//
varying	vec3 l;
varying	vec3 h;
varying vec3 v;
varying vec3 n;

void main (void)
{
	const vec4	diffColor = vec4 ( 1.0, 1.0, 0.0, 1.0 );
	const float	k = 0.8;

	vec3	n2 = normalize ( n );
	vec3	l2 = normalize ( l );
	vec3	v2 = normalize ( v );
	float	d1 = pow ( max ( dot ( n2, l2 ), 0.0 ), 1.0 + k );
	float	d2 = pow ( 1.0 - dot ( n2, v2 ), 1.0 - k );

	gl_FragColor = diffColor * d1 * d2;
}
