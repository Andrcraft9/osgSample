// Blinn fragment shader

varying	vec3 LightVector;
varying	vec3 HalfVector;
varying vec3 Normal;

varying	vec3 LightVectorTBN;
varying	vec3 HalfVectorTBN;

varying vec3 ReflectDir;

uniform sampler2D NormalMapTex;
uniform samplerCube CubeMapTex;

uniform float ReflectFactor;
uniform vec4 MaterialColor;

void main(void)
{
	vec4 diffColor = gl_FrontMaterial.diffuse;
	vec4 specColor = gl_FrontMaterial.specular;
	float specPower = gl_FrontMaterial.shininess;
	
	if (specPower >= 1.0)
	{
		// CubeMap reflections
		vec4 reflectColor = texture(CubeMapTex, ReflectDir);
		gl_FragColor = mix(MaterialColor, reflectColor, ReflectFactor);
	}
	else if (specPower >= 0.5)
	{
		// Bump mapping
		// Blinn Lighting

		// normalize vectors after interpolation
		const vec4	diffColor = vec4 ( 0.3, 0.3, 0.3, 1.0 );
		const vec4	specColor = vec4 ( 0.5, 0.5, 0.5, 1.0 );
		const float	specPower = 10.0;
	
		vec3 n = texture2D(NormalMapTex, gl_TexCoord[0].xy).xyz;
		n = normalize(n * 2.0 - 1.0);
		
		//vec4 texC = texture(NormalMapTex, gl_TexCoord[0]);

		vec3 l = normalize(LightVectorTBN);
		vec3 h = normalize(HalfVectorTBN);

		vec4 diff = diffColor * max(dot(n, l), 0.0);

		vec4 spec = vec4(0,0,0,1);
		if (dot(n, l) > 0)
			spec = specColor * pow(max(dot(n, h), 0.0), specPower);
		
		gl_FragColor = diff * gl_Color + spec;
		//gl_FragColor = diff * vec4(0,0,0,1)  + spec;
		//gl_FragColor = texC;
	}
	else
	{
		// Blinn Lighting
		// normalize vectors after interpolation
		const vec4	diffColor = vec4 ( 0.3, 0.3, 0.3, 1.0 );
		const vec4	specColor = vec4 ( 0.5, 0.5, 0.5, 1.0 );
		const float	specPower = 10.0;
	
		vec3 n = normalize(Normal);
		vec3 l = normalize (LightVector);
		vec3 h = normalize (HalfVector);

		vec4 diff = diffColor * max(dot(n, l), 0.0);
		vec4 spec = specColor * pow(max(dot(n, h), 0.0), specPower);
		
		gl_FragColor = diff * gl_Color + spec;
	}
}
