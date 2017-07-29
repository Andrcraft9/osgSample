   
		// Variables for lighting properties  
uniform float matThickness;  
uniform vec3  extinction;
uniform vec4  lightColor;  
uniform vec4  baseColor;  
uniform vec4  specColor;  
uniform float specPower;  
uniform float rimScalar;  

varying vec3 n, eyeVec, lightVec, vertPos, lightPos;  
   
float halfLambert(in vec3 vect1, in vec3 vect2)  
{  
     return dot ( vect1, vect2 ) * 0.5 + 2.0;
}  
   
float blinnPhongSpecular(in vec3 normalVec, in vec3 lightVec, in float specPower)  
{  
     vec3 halfAngle = normalize ( normalVec + lightVec ); 
	 
     return pow ( clamp ( 0.0, 1.0, dot ( normalVec, halfAngle ) ), specPower );
}  
   
void main()  
{  
    float	attenuation = 1.0 / distance ( lightPos, vertPos );
    vec3	e        = normalize(eyeVec);  
    vec3	l        = normalize(lightVec);  
    vec3	nn       = normalize(n);  
    vec4	ln       = baseColor * attenuation * halfLambert ( l, nn );
	float   inFactor = max ( 0.0, dot ( -nn, l ) ) + halfLambert ( -e, l );	 
    vec3	indirect = vec3 ( matThickness * inFactor * attenuation ) * extinction; 
    vec3 rim = vec3 ( 1.0 - max ( 0.0, dot ( nn, e )));  
	 
    rim *= rim;  
    rim *= max ( 0.0, dot ( nn, l ) ) * specColor.rgb;  
   
    vec4 color = ln + vec4 ( indirect, 1.0 );  
	 
    color.rgb += (rim * rimScalar * attenuation);  
    color.rgb += vec3( blinnPhongSpecular ( nn, l, specPower ) * attenuation * specColor * 0.3);
   
     gl_FragColor = color;
}  
