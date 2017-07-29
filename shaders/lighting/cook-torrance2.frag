
varying	vec3 LightDir;
varying	vec3 HalfVec;
varying	vec3 Normal;
varying vec3 ViewDir;

uniform sampler2D	Tex;

void main (void)
{
	float r0 = 0.5;
	float roughness = 0.5;

	vec4 diffColor = gl_FrontMaterial.diffuse;
	vec4 specColor = gl_FrontMaterial.specular;
	float specPower = gl_FrontMaterial.shininess;
	specColor = vec4((vec4(0.7,0.7,0.7,0) - diffColor).rgb, 1);

	const float	e = 2.7182818284;
	const float pi = 3.1415926;

	vec3 n = normalize(Normal);
	vec3 l = normalize(LightDir);
	vec3 v = normalize(ViewDir);
	vec3 h = normalize(HalfVec);

	float nh = dot(n, h);
	float nv = dot(n, v);
	float nl = dot(n, l);
	
	float r2 = roughness * roughness;
	float nh2 = nh * nh;
	float ex = -(1.0 - nh2) / (nh2 * r2);
	float d = pow(e, ex) / (r2 * nh2 * nh2); // Mircofacet distribution

	float f = mix(pow(1.0 - nv, 5.0), 1.0, r0);	// Fresnel
	
	float x = 2.0 * nh / dot(v, h);
	float g = min(1.0, min(x * nl, x * nv)); // Geometry attenuation
	
	float ct = d * f * g / nv;
	
	vec4 texColor = texture(Tex, gl_TexCoord[0]);
	vec4 diff = diffColor * max (0.0, nl);
	vec4 spec = specColor * max (0.0, ct);
	
	gl_FragColor = texColor * diff + spec;
}
