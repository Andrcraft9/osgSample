

varying	vec3 LightDir;
varying	vec3 ViewDir;
varying	vec3 Normal;

uniform sampler2D Tex;
uniform sampler2D Tex2;
uniform sampler2D Tex3;
uniform sampler2D Tex4;

void main()
{
	vec4 diffColor = gl_FrontMaterial.diffuse;
	vec4 specColor = gl_FrontMaterial.specular;
	float specPower = gl_FrontMaterial.shininess;
	specColor = vec4((vec4(0.7,0.7,0.7,0) - diffColor).rgb, 1);

    // set important material values
    float roughnessValue = 0.5; // 0 : smooth, 1: rough
    float F0 = 0.5; // fresnel reflectance at normal incidence
    float k = 0.2; // fraction of diffuse reflection (specular reflection = 1 - k)
    //vec3 lightColor = vec3(0.9, 0.1, 0.1);
    
    // interpolating normals will change the length of the normal, so renormalize the normal.
    vec3 normal = normalize(Normal);
    
    // do the lighting calculation for each fragment.
    float NdotL = max(dot(normal, LightDir), 0.0);
    
    float specular = 0.0;
    if(NdotL > 0.0)
    {
        vec3 eyeDir = normalize(ViewDir);

        // calculate intermediary values
        vec3 halfVector = normalize(LightDir + eyeDir);
        float NdotH = max(dot(normal, halfVector), 0.0); 
        float NdotV = max(dot(normal, eyeDir), 0.0); // note: this could also be NdotL, which is the same value
        float VdotH = max(dot(eyeDir, halfVector), 0.0);
        float mSquared = roughnessValue * roughnessValue;
        
        // geometric attenuation
        float NH2 = 2.0 * NdotH;
        float g1 = (NH2 * NdotV) / VdotH;
        float g2 = (NH2 * NdotL) / VdotH;
        float geoAtt = min(1.0, min(g1, g2));
     
        // roughness (or: microfacet distribution function)
        // beckmann distribution function
        float r1 = 1.0 / ( 4.0 * mSquared * pow(NdotH, 4.0));
        float r2 = (NdotH * NdotH - 1.0) / (mSquared * NdotH * NdotH);
        float roughness = r1 * exp(r2);
        
        // fresnel
        // Schlick approximation
        float fresnel = pow(1.0 - VdotH, 5.0);
        fresnel *= (1.0 - F0);
        fresnel += F0;
        
        specular = (fresnel * geoAtt * roughness) / (NdotV * NdotL * 3.14);
		specular = max(specular, 0.0);
    }
    
	vec4 texColor = texture(Tex, gl_TexCoord[0]);
	vec4 texColor2 = texture(Tex3, gl_TexCoord[0]);

    vec3 finalValue = vec3(texColor) * NdotL * vec3(diffColor) + specular * NdotL * vec3(specColor);
    gl_FragColor = vec4(finalValue, 1.0);

}