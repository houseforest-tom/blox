#version 400 core

//Input: Clip space position
in vec3 fPosition;

//Input: World space normal
in vec3 fNormal;

//Input: Color
in vec3 fColor;

//Output: GBuffer diffuse
out vec3 pDiffuse;

//Output: GBuffer normal
out vec3 pNormal;

void main( void )
{
	pDiffuse = fColor;
	pNormal = normalize(fNormal);
}