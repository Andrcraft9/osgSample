//
// Vertex shader for Gooch shading
// Based on 3Dlabs code
//

uniform vec4	lightPos;
uniform	vec4	eyePos;

varying float NdotL;
varying vec3  reflectVec;
varying vec3  viewVec;

void main(void)
{
    vec3 p        = vec3      ( gl_ModelViewMatrix * gl_Vertex );
    vec3 norm     = normalize ( gl_NormalMatrix    * gl_Normal );
    vec3 lightVec = normalize ( lightPos.xyz - p.xyz );


    reflectVec  = normalize ( reflect ( -lightVec, norm ) );
    viewVec     = normalize ( eyePos.xyz -p.xyz );
    NdotL       = ( dot ( lightVec, norm ) + 1.0 ) * 0.5;
    gl_Position = ftransform();
}