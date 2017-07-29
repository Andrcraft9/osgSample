varying vec3 LightDir;
varying vec3 ViewDir;

uniform sampler2D ColorTex;
uniform sampler2D NormalMapTex;
uniform sampler2D SpecularMapTex;

void main(void)  
{
	vec4 diffColor = gl_FrontMaterial.diffuse;
	vec4 specColor = gl_FrontMaterial.specular;
	float specPower = gl_FrontMaterial.shininess;

	vec4 texColor = texture(ColorTex, gl_TexCoord[0]);
	vec3 n = texture2D(NormalMapTex, gl_TexCoord[0].xy).xyz;
    n = normalize(n * 2.0 - 1.0);

	vec3 l = normalize(LightDir);
	vec3 v = normalize(ViewDir);
	vec3 r = reflect(-v, n);

	// Lighting model by Phong with Specular Map
	specColor = texture2D(SpecularMapTex, gl_TexCoord[0]);
	
	vec4 diff = diffColor * max(dot(n, l), 0.0);

	vec4 spec = vec4(0,0,0,1);
	if (dot(n, l) > 0)
		spec = specColor * pow(max(dot(l, r), 0.0), specPower);
	
	gl_FragColor = diff * texColor + spec;
		
}  

