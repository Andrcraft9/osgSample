//
// Strauss fragment shader
//


uniform	float	smooth;
uniform	float	metal;
uniform	float	transp;
uniform	float	r0;


varying	vec3 l;
varying	vec3 h;
varying vec3 v;
varying vec3 n;

float	fresnel ( float x, float kf )
{
	float	dx  = x - kf;
	float	d1 = 1.0 - kf;
	float	kf2 = kf * kf;
	
	return (1.0 / (dx * dx) - 1.0 / kf2) / (1.0 / (d1 * d1) - 1.0 / kf2 );
	//return 1.0;
}

float	shadow ( float x, float ks )
{
	float	dx  = x - ks;
	float	d1 = 1.0 - ks;
	float	ks2 = ks * ks;
	
	//return (1.0 / (dx * dx) - 1.0 / ks2) / (1.0 / (d1 * d1) - 1.0 / ks2 );
	return 1.0;
}

void main (void)
{
	const vec4	diffColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );
	const vec4	specColor = vec4 ( 1.0, 1.0, 0.0, 1.0 );
	const float	k  = 0.1;
	const float kf = 1.12;
	const float ks = 1.01;
/*
const	float	smooth = 0.7;
const	float	metal  = 0.7;
const	float	transp = 0.5;
*/	
	vec3	n2 = normalize ( n );
	vec3	l2 = normalize ( l );
	vec3	v2 = normalize ( v );
	vec3	h2 = reflect   ( l2, n2 );
	float 	nl = dot( n2, l2 );
	float 	nv = dot( n2, v2 );
	float	hv = dot( h2, v2 );
	float	f  = fresnel( nl, kf );
	float	s3 = smooth * smooth * smooth;
 
			// diffuse term
	float	d    = ( 1.0 - metal * smooth );
	float	Rd   = ( 1.0 - s3 ) * ( 1.0 - transp );
	vec4	diff = nl * d * Rd * diffColor;
 
			// inputs into the specular term
	float	r       = (1.0 - transp) - Rd;
	float	j       = f * shadow ( nl, ks ) * shadow ( nv, ks );
	float	reflect = min ( 1.0, r + j * ( r + k ) );
	vec4	C1      = vec4 ( 1.0 );
	vec4 	Cs      = C1 + metal * (1.0 - f) * (diffColor - C1); 
	vec4	spec    = Cs * reflect;

	spec *= pow ( -hv, 3.0 / (1.0 - smooth) );
 
			// composite the final result, ensuring
	diff = max ( vec4 ( 0.0 ), diff );
	spec = max ( vec4 ( 0.0 ), spec );

	gl_FragColor = diff + spec*specColor;
}
