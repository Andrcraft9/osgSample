// Phong fragment shader

varying	vec3 LightDir;
varying	vec3 ViewDir;
varying	vec3 Normal;

uniform sampler2D Tex;

void main (void)
{
	vec4 diffColor = gl_FrontMaterial.diffuse;
	vec4 specColor = gl_FrontMaterial.specular;
	float specPower = gl_FrontMaterial.shininess;

	vec4 texColor = texture(Tex, gl_TexCoord[0]);

	vec3 n = normalize(Normal);
	vec3 l = normalize(LightDir);
	vec3 v = normalize(ViewDir);
	vec3 r = reflect (-v, n);
	vec4 diff = diffColor * max(dot(n, l), 0.0);
	vec4 spec = specColor * pow(max(dot(l, r), 0.0), specPower);

	gl_FragColor = diff * texColor + spec;
}
