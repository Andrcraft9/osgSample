//
// Rim lighting fragment shader
//
varying	vec3 l;
varying	vec3 h;
varying vec3 v;
varying vec3 n;

uniform	sampler2D	decalMap;

void main (void)
{
	const vec4  diffColor = vec4 ( 0.5, 0.0, 0.0, 1.0 );
	const vec4  specColor = vec4 ( 0.7, 0.7, 0.0, 1.0 );
	const float specPower = 30.0;
	const float rimPower  = 8.0;
	const float bias      = 0.3;

	vec3	n2   = normalize ( n );
	vec3	l2   = normalize ( l );
	vec3	h2   = normalize ( h );
	vec3	v2   = normalize ( v );
	vec4	diff = diffColor * max ( dot ( n2, l2 ), 0.0 );
	vec4	spec = specColor * pow ( max ( dot ( n2, h2 ), 0.0 ), specPower );
	float	rim  = pow ( 1.0 + bias - max ( dot ( n2, v2 ), 0.0 ), rimPower );

	gl_FragColor = diff + rim * vec4 ( 0.5, 0.0, 0.2, 1.0 ) + spec * specColor;
}
