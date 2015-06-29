#version 400 core

//GBuffer textures
uniform sampler2D uDiffuseTexture;

//Ambient light characteristics
uniform vec3  uColor;

//Input: Texture coordinates received from vertex shader
in vec2 fTexCoord;

//Output: Pixel color
out vec4 pColor;

vec3 getColor()
{
	//Return product of diffuse and light color
	return uColor * texture(uDiffuseTexture, fTexCoord).rgb;
}

void main( void )
{	
	pColor = vec4(getColor(), 1.0f);
}