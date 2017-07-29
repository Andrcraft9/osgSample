uniform sampler2D texture;
 
varying vec4 vertColor;
 
varying vec4 vertTexCoord;
 
void main(void)
{
 
    float depth = gl_FragCoord.z / gl_FragCoord.w;
 
 
    float sampleOffset = depth/100000.0;
 
    vec4 col = vec4(0.0);
    col += texture2D(texture, vertTexCoord.st + vec2(-sampleOffset,-sampleOffset)) * 1.0;
    col += texture2D(texture, vertTexCoord.st + vec2( 0.0         ,-sampleOffset)) * 2.0;
    col += texture2D(texture, vertTexCoord.st + vec2( sampleOffset,-sampleOffset)) * 1.0;
 
    col += texture2D(texture, vertTexCoord.st + vec2(-sampleOffset, 0.0     )) * 2.0;
    col += texture2D(texture, vertTexCoord.st + vec2( 0.0         , 0.0     )) * 4.0;
    col += texture2D(texture, vertTexCoord.st + vec2( sampleOffset, 0.0     )) * 2.0;
 
    col += texture2D(texture, vertTexCoord.st + vec2( sampleOffset, sampleOffset)) * 1.0;
    col += texture2D(texture, vertTexCoord.st + vec2( 0.0         , sampleOffset)) * 2.0;
    col += texture2D(texture, vertTexCoord.st + vec2(-sampleOffset, sampleOffset)) * 1.0;
 
    col /= 16.0;
 
    gl_FragColor = col;
 
}