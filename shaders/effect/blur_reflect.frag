
varying	vec3 LightVector;
varying	vec3 HalfVector;
varying	vec3 Normal;
varying vec3 EyeVector;

uniform sampler2D DiffTexture;
uniform sampler2D Reflection;

varying vec4 ProjCoords;

varying vec2 v_blurTexCoords[14];

void main()
{

	vec4 surfColor;
	vec2 uv2 =  ProjCoords.xy / ProjCoords.w;
	uv2 = (uv2 + 1.0) * 0.5;
	
	// BLUR
	surfColor = vec4(0.0);
	uv2 = (v_blurTexCoords[0] + 1.0) * 0.5;
    surfColor += texture2D(Reflection, uv2)*0.0044299121055113265;
	uv2 = (v_blurTexCoords[1] + 1.0) * 0.5;
    surfColor += texture2D(Reflection, uv2)*0.00895781211794;
	uv2 = (v_blurTexCoords[2] + 1.0) * 0.5;
    surfColor += texture2D(Reflection, uv2)*0.0215963866053;
	uv2 = (v_blurTexCoords[3] + 1.0) * 0.5;
    surfColor += texture2D(Reflection, uv2)*0.0443683338718;
	uv2 = (v_blurTexCoords[4] + 1.0) * 0.5;
    surfColor += texture2D(Reflection, uv2)*0.0776744219933;
	uv2 = (v_blurTexCoords[5] + 1.0) * 0.5;
    surfColor += texture2D(Reflection, uv2)*0.115876621105;
	uv2 = (v_blurTexCoords[6] + 1.0) * 0.5;
    surfColor += texture2D(Reflection, uv2)*0.147308056121;

    uv2 =  ProjCoords.xy / ProjCoords.w;
	uv2 = (uv2 + 1.0) * 0.5;
	surfColor += texture2D(Reflection, uv2)*0.159576912161;
    
	uv2 = (v_blurTexCoords[7] + 1.0) * 0.5;
	surfColor += texture2D(Reflection, uv2)*0.147308056121;
    uv2 = (v_blurTexCoords[8] + 1.0) * 0.5;
	surfColor += texture2D(Reflection, uv2)*0.115876621105;
    uv2 = (v_blurTexCoords[9] + 1.0) * 0.5;
	surfColor += texture2D(Reflection, uv2)*0.0776744219933;
    uv2 = (v_blurTexCoords[10] + 1.0) * 0.5;
	surfColor += texture2D(Reflection, uv2)*0.0443683338718;
    uv2 = (v_blurTexCoords[11] + 1.0) * 0.5;
	surfColor += texture2D(Reflection, uv2)*0.0215963866053;
    uv2 = (v_blurTexCoords[12] + 1.0) * 0.5;
	surfColor += texture2D(Reflection, uv2)*0.00895781211794;
    uv2 = (v_blurTexCoords[13] + 1.0) * 0.5;
	surfColor += texture2D(Reflection, uv2)*0.0044299121055113265;

	vec4 diffColor = vec4(0.8, 0.8, 0.9, 1);
	vec4 specColor = vec4(0.3, 0.3, 0.35, 1);
	float specPower = 20;

	// normalize vectors after interpolation
	vec3 n = normalize(Normal);
	vec3 l = normalize(LightVector);
	vec3 h = normalize(HalfVector);
	
	vec4 diff = diffColor * max(dot(n, l), 0.0);
	vec4 spec = specColor * pow(max(dot(n, h), 0.0), specPower);

	vec4 color_tex = texture2D(DiffTexture, gl_TexCoord[0]);
	gl_FragColor =  diff * surfColor * color_tex + spec;

	//if ((refl.a == 1))
	//	gl_FragColor = vec4(0,0,0,1);
	//else
	//	gl_FragColor = mix(color, refl, 0.5);
	
}
