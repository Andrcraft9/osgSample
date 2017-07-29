uniform sampler2D ColorTex;
uniform sampler2D NormalMapTex;
uniform sampler2D HeightMapTex;
uniform sampler2D SpecularMapTex;

varying	vec3 LightDir; // vector to light in tangent space
varying	vec3 ViewDir; // vector to eye in tangent space
varying vec3 HalfDir;


uniform float Scale; // scale to control amount of parallax
uniform float Bias;

void main (void)
{
	vec4 diffColor = gl_FrontMaterial.diffuse;
	vec4 specColor = gl_FrontMaterial.specular;
	float specPower = gl_FrontMaterial.shininess;
	
	const float numSteps = 20.0;
	//float specExp = 100.0;

	float step = 1.0 / numSteps;
	vec2 dtex = ViewDir.xy * Scale / (numSteps * ViewDir.z); // adjustment for one layer
	float height = 1.0;	// height of the layer
	vec2 tex = gl_TexCoord[0].xy;	// our initial guess
	float h = 1.0 - texture2D(HeightMapTex, tex).r; // get height

	while (h < height)
	{
		height -= step;
		tex += dtex;
		h = 1.0 - texture2D(HeightMapTex, tex).r;
	}
	
	// Now add some iterations of binary search
	vec2 delta = 0.5 * dtex;
	vec2 mid = tex - delta;	// midpoint

	for(int i = 0; i < 8; i++)
	{
		delta *= 0.5;

		if (1.0 - texture2D(HeightMapTex, mid).r < height)
			mid += delta;
		else
			mid -= delta;
	}

	tex = mid;
	
	// Now offset texture coordinates with height
	vec3 color = texture2D(ColorTex, tex).rgb;
	vec3 n = normalize(texture2D(NormalMapTex, tex).rgb * 2.0 - vec3(1.0));
	
	specColor =  texture2D(SpecularMapTex, tex);
	
	vec3 diff = diffColor * max(dot(n, normalize(LightDir)), 0.0);
	vec3 spec = specColor * pow(max(dot(n, normalize(HalfDir)), 0.0), specPower);

	gl_FragColor = vec4(color * diff + spec, 1.0);
	//gl_FragColor = specColor;
}