// Blinn fragment shader

varying	vec3 LightVector;
varying	vec3 HalfVector;
varying	vec3 Normal;
varying vec3 EyeVector;

uniform sampler2D Tex;

void main(void)
{
	vec4 diffColor = gl_FrontMaterial.diffuse;
	vec4 specColor = gl_FrontMaterial.specular;
	float specPower = gl_FrontMaterial.shininess;

	vec4 texColor = texture(Tex, gl_TexCoord[0]);

	// normalize vectors after interpolation
	vec3 n = normalize(Normal);
	vec3 l = normalize (LightVector);
	vec3 h = normalize (HalfVector);
	
	vec4 diff = diffColor * max(dot(n, l), 0.0);
	vec4 spec = specColor * pow(max(dot(n, h), 0.0), specPower);

	gl_FragColor = diff * texColor + spec;
	//gl_FragColor = diff * gl_Color + spec;
}
