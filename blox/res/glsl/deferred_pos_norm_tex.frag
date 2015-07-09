#version 400 core

//Uniform: Texture to use
uniform sampler2D uDiffuseTexture;

//Input: World space position
in vec3 fPosition;

//Input: World space normal
in vec3 fNormal;

//Input: Texture coords
in vec2 fTexCoord;

//Output: GBuffer diffuse
out vec3 pDiffuse;

//Output: GBuffer normal
out vec3 pNormal;

void main( void )
{
	pDiffuse = texture(uDiffuseTexture, fTexCoord).rgb;
	pNormal = normalize(fNormal);
}