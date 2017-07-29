
uniform sampler2D ColorTex;
uniform sampler2D HeightMapTex;

varying	vec3 LightDir; // vector to light in tangent space
varying	vec3 ViewDir; // vector to eye in tangent space

uniform float Scale; // scale to control amount of parallax
uniform float Bias;

void main (void)
{    
	// get height
    float h = Scale * (1.0 - texture2D(HeightMapTex, gl_TexCoord[0]).a) + Bias;

    // now offset texture coordinates with height 
    vec2 tex = gl_TexCoord[0].xy + ViewDir.xy * h / ViewDir.z;

    gl_FragColor = vec4(texture2D(ColorTex, tex).rgb, 1.0);
	
	//float alp = texture(HeightMapTex, gl_TexCoord[0]).a;
	//gl_FragColor = vec4(alp, alp, alp, 1.0);

}