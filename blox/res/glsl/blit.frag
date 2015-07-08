#version 400 core

//Uniform: Texture unit
uniform sampler2D uTextureUnit;

//Input: Texture coordinates
in vec2 fTexCoord;

//Output: Pixel color
out vec4 pColor;

void main( void )
{
	pColor = texture(uTextureUnit, fTexCoord);
}