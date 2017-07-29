
uniform sampler2D ColorTex;
uniform sampler2D HeightMapTex;

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
	
	const float numSteps = 10.0;
	//const float	specExp = 80.0;

	float step = 1.0 / numSteps;
	vec2 dtex = ViewDir.xy * Scale / (numSteps * ViewDir.z); // adjustment for one layer
	float height = 1.0;	// height of the layer
	vec2 tex = gl_TexCoord[0].xy;	// our initial guess
	float h = texture2D(HeightMapTex, tex).a; // get height

	while (h < height)
	{
		height -= step;
		tex += dtex;
		h = texture2D(HeightMapTex, tex).a;
	}
	
	// now add some iterations of binary search
	vec2 delta = 0.5 * dtex;
	vec2 mid = tex - delta;	// midpoint

	for(int i = 0; i < 5; i++)
	{
		delta *= 0.5;

		if (texture2D(HeightMapTex, mid).a < height)
			mid += delta;
		else
			mid -= delta;
	}

	tex = mid;
	
	// now offset texture coordinates with height
	vec3 color = texture2D(ColorTex, tex).rgb;
	vec3 n = normalize(texture2D(HeightMapTex, tex).rgb * 2.0 - vec3(1.0));
	float diff = max(dot(n, normalize(LightDir)), 0.0);
	float spec = pow(max(dot(n, normalize(HalfDir)), 0.0), specPower);

	gl_FragColor = vec4(color * diff + vec3(0.7 * spec), 1.0);
}