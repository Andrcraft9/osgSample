
uniform float RimPower;
uniform float Bias;

varying	vec3 l;
varying	vec3 h;
varying vec3 v;
varying vec3 n;

void main (void)
{
	vec4 diffColor = gl_FrontMaterial.diffuse;
	vec4 specColor = gl_FrontMaterial.specular;
	float specPower = gl_FrontMaterial.shininess;

    vec3 n2 = normalize(n);
    vec3 l2 = normalize(l);
    vec3 h2 = normalize(h);
    vec3 v2 = normalize(v);
    
	vec4  diff = diffColor *  max(dot(n2, l2), 0.0);
    vec4  spec = specColor * pow(max(dot(n2, h2), 0.0), specPower);
    float rim  = pow(1.0 + Bias - max(dot(n2, v2), 0.0), RimPower);

    gl_FragColor = diff * gl_Color + rim * vec4(0.5, 0.5, 0.6, 1.0) + spec * specColor;
}