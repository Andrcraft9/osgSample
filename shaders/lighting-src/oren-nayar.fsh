//
// Oren-Nayar fragment shader
//

varying	vec3 l;
varying vec3 v;
varying vec3 n;

uniform sampler2D	lookupMap;
uniform	float		a, b;

void main (void)
{
	const vec4	diffColor = vec4 ( 0.5, 0.0, 0.0, 1.0 );

	vec3	n2   = normalize ( n );
	vec3	l2   = normalize ( l );
	vec3	v2   = normalize ( v );

	float	nl    = dot ( n2, l2 );
	float	nv    = dot ( n2, v2 );
	vec3	lProj = normalize ( l2 - n2 * nl );
	vec3	vProj = normalize ( v2 - n2 * nv );
	float	cx    = max ( dot ( lProj, vProj ), 0.0 );
//	float	dx    = texture2D ( lookupMap, vec2 ( nl, nv ) ).x;		// lookup for sin(alpha)*tan(beta)
	
	float	cosAlpha = nl > nv ? nl : nv;				// max (cl, cv )
	float	cosBeta  = nl > nv ? nv : nl;				// min(cl, cv)
	float	dx       = sqrt ( ( 1.0 - cosAlpha * cosAlpha ) * ( 1.0 - cosBeta * cosBeta ) ) / cosBeta;
	
	gl_FragColor = max ( 0.0, nl ) * diffColor * (a + b * cx * dx);
}
