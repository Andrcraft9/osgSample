//
// Simple anisotropic Ward lighting fragment shader
//

varying vec3 vt;
varying	vec3 lt;
varying	vec3 ht;

uniform sampler2D tangentMap;
uniform sampler2D decalMap;

void main (void)
{
	const vec4	specColor = vec4 ( 0, 0, 1, 0 );
	const vec3	n         = vec3 ( 0, 0, 1 );
	const float	roughness = 5.0;

	vec4	color = texture2D ( decalMap, gl_TexCoord [0].xy );
	vec3	tang  = normalize ( 2.0 * texture2D ( tangentMap, gl_TexCoord [0].xy ).xyz - vec3 ( 1.0 ) );
	float	dot1  = dot  ( ht, tang ) * roughness;
	float	dot2  = dot  ( ht, n );
	float	p     = dot1 / dot2;

	gl_FragColor.rgb = color.rgb + specColor.rgb * exp ( -p*p );
	gl_FragColor.a   = 1.0;
}
