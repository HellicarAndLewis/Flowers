#version 120

uniform sampler2D u_particleImageTexture;

uniform int u_numTextures;

uniform vec4 u_particleStartColor;
uniform vec4 u_particleEndColor;

varying vec4 v_particleColor;

void main ()
{
    vec4 particleSpriteTexCol;
//    if( gl_PointCoord.x > 0.5 )
        particleSpriteTexCol = texture2D( u_particleImageTexture, gl_PointCoord.xy );
//    else
//        particleSpriteTexCol = texture2D( u_particleImageTexture2, gl_PointCoord.xy );

	
	vec4 color = particleSpriteTexCol * v_particleColor;
	//if (color.a <= 0.0) discard;
	
	gl_FragColor = color;
	//gl_FragColor = vec4(1.0,1.0,1.0,1.0);
}