//
// Vertex shader for Gooch shading
// Based on 3Dlabs code
//

varying float NdotL;
varying vec3  reflectVec;
varying vec3  viewVec;

void main(void)
{
    vec3 p        = vec3      ( gl_ModelViewMatrix * gl_Vertex );
    vec3 norm     = normalize ( gl_NormalMatrix    * gl_Normal );
    vec3 lightVec = normalize ( gl_LightSource[0].position - p.xyz );


    reflectVec  = normalize ( reflect ( -lightVec, norm ) );
    viewVec     = normalize ( gl_LightSource[0].position -p.xyz );
    NdotL       = ( dot ( lightVec, norm ) + 1.0 ) * 0.5;
    gl_Position = ftransform();
}