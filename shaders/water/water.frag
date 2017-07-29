
uniform sampler2D Default;
uniform sampler2D Reflection;
uniform sampler2D Refraction;
uniform sampler2D NormalTex;
uniform samplerCube CubeMapTexture;

varying vec4 ProjCoords;
varying vec3 LightDir, EyeDir;
varying vec2 FlowCoords, RippleCoords;

varying vec3 ReflectDir;

void main()
{
	vec4 reflectColor = texture(CubeMapTexture, ReflectDir);

	vec2 rippleEffect = 0.02 * texture2D(Refraction, RippleCoords * 0.1).xy;
	//rippleEffect = vec2(0,0);
	vec4 N = texture2D(NormalTex, FlowCoords + rippleEffect);
	N = N * 2.0 - vec4(1.0);
	N.a = 1.0; 
	N = normalize(N);

	vec3 refVec = normalize(reflect(-LightDir, vec3(N)));
	float refAngle = clamp(dot(EyeDir, refVec), 0.0, 1.0);
	vec4 specular = vec4(0.6, 0.6, 0.6, 1) * pow(refAngle, 30.0);
	
	vec3 dist = texture2D(Refraction, FlowCoords + rippleEffect).rgb;
	dist = (dist * 2.0 - vec3(1.0)) * 0.1;
	
	vec3 uv = ReflectDir;
	uv = uv + dist;

	vec4 base = texture(CubeMapTexture, uv);

	vec2 uv2 =  ProjCoords.xy / ProjCoords.w;
	uv2 = (uv2 + 1.0) * 0.5;
	uv2 = clamp(uv2 + dist, 0.0, 1.0);
	vec4 refl = texture2D(Reflection, uv2);

	if (refl.r == 0 && refl.g == 0 && refl.b == 0 && refl.a == 0)
		;
	else
		base = refl;
	gl_FragColor = mix(base, specular, 0.6);
	gl_FragColor = base + specular;

	//gl_FragColor = mix(base, refl + specular, 0.6);
	//gl_FragColor = refl;
	//gl_FragColor = mix(base, specular, 0.2);
}
