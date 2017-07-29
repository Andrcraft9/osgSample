//
// Fragment shader for Gooch shading
//
// Based on 3Dlabs code by Randi Rost
//

uniform vec3  SurfaceColor; // (0.75, 0.75, 0.75)
uniform vec3  WarmColor;    // (0.6, 0.6, 0.0)
uniform vec3  CoolColor;    // (0.0, 0.0, 0.6)
uniform float DiffuseWarm;  // 0.45
uniform float DiffuseCool;  // 0.45

varying float NdotL;
varying vec3  reflectVec;
varying vec3  viewVec;

void main (void)
{
	const	vec3  surfaceColor = vec3 ( 0.75, 0.75, 0.75 );
	const	vec3  warmColor    = vec3 ( 0.6, 0.6, 0.0 );
	const	vec3  coolColor    = vec3 ( 0.0, 0.0, 0.6 );
	const	float diffuseWarm  = 0.45;
	const	float diffuseCool  = 0.45;

    vec3 kCool    = min ( coolColor + diffuseCool * surfaceColor, 1.0 );
    vec3 kWarm    = min ( warmColor + diffuseWarm * surfaceColor, 1.0 );
    vec3 kFinal   = mix ( kCool, kWarm, NdotL );

    vec3  r    = normalize ( reflectVec );
    vec3  v    = normalize ( viewVec    );
    float spec = pow ( max ( dot ( r, v ), 0.0 ), 32.0 );

    gl_FragColor = vec4 ( min ( kFinal + spec, 1.0 ), 1.0 );
}