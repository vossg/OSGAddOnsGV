
[VertexShader]

uniform vec4 scaleFactor;

uniform vec2 transitionWidth;
uniform vec2 activeRegionCenter;
uniform vec2 activeRegionMin;
uniform vec2 activeRegionMax;
uniform vec3 localViewerPos;

varying float alpha;
varying vec2 uv;

void main( void )
{	
	//float alphax = clamp( ( abs( gl_Vertex.x - localViewerPos.x ) - ( ( activeRegionMax.x - activeRegionMin.x ) / 2.0 - transitionWidth.x - 1.0 ) ) / transitionWidth.x, 0.0, 1.0 );
	//float alphay = clamp( ( abs( gl_Vertex.z - localViewerPos.z ) - ( ( activeRegionMax.y - activeRegionMin.y ) / 2.0 - transitionWidth.y - 1.0 ) ) / transitionWidth.y, 0.0, 1.0 );

	vec2 worldPos = gl_Vertex.xy * scaleFactor.xy + scaleFactor.zw;
	
	vec4 pos;
	
	pos.x = worldPos.x;
	pos.z = worldPos.y;		
	pos.y = 0.0;
	pos.w = 1.0;
	
	gl_Position = gl_ModelViewProjectionMatrix * pos;
}

[FragmentShader]

uniform vec3 baseColor;

varying float alpha;
varying vec2 uv;

void main( void )
{
	//vec3 diffuseColor = nDotVP * vec3( 0.5, 0.6, 0.3 );
	//vec3 diffuseColor = mix( baseColor0, baseColor1, alpha );
	//vec3 diffuseColor = baseColor0;
	 
	//gl_FragColor = vec4( baseColor, 1.0 );
  gl_FragColor = vec4( baseColor, 1.0 );
}
