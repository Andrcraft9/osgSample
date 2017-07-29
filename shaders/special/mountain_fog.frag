varying vec3 LightDir;
varying vec3 ViewDir;
varying vec4 ViewWorld;

uniform sampler2D ColorTex;
uniform sampler2D NormalMapTex;
uniform sampler2D SpecularMapTex;

uniform mat4 CameraToWorld;

vec3 applyNCDFog(vec3  rgb,      // original color of the pixel
                 float distance, // camera to point distance
				 float height,   
				 float b, float c)  
{
	distance = 5000 + distance / 2;
	
	height = abs(height + 200) / 2000;
	//height = 0 + abs(height) / 10000;
	
	float be = (height) * 0.0004; 
	float bi = (height) * 0.0001; 
	
 
	float ext = b * exp(-distance * be);
	float insc = c * exp(-distance * bi);
	ext = clamp(ext, 0.0, 1.0);
	insc = clamp(insc, 0.0, 1.0);
	
	vec3 fogColor = vec3(0.5,0.5,0.6);
	
	//float fogFactor = ext * (1.0 - insc); // / (viewSpace.y);
	//return mix(rgb, fogColor, fogFactor);
	
	return rgb * ext + fogColor * (1 - insc);
}

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
	
	float dist = length(ViewWorld);
	float height = (CameraToWorld * ViewWorld).z;
	gl_FragColor = vec4( applyNCDFog((diff * texColor + spec).rgb, dist, height, 1, 1), 1 );
		 
	//gl_FragColor = vec4(normalize(vec3(CameraToWorld[1][0], CameraToWorld[1][1], CameraToWorld[1][2])), 1);
	
	//gl_FragColor = diff * texColor + spec;
	
}  

