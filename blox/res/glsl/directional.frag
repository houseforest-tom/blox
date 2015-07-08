#version 400 core

//GBuffer textures
uniform sampler2D uGBufferDiffuse;
uniform sampler2D uGBufferNormal;
uniform sampler2D uGBufferDepth;

//Shadow map for this light
uniform sampler2D uShadowMap;

//Directional light characteristics
uniform struct DirectionalLight
{
	vec3 direction;
	vec3 color;
} uDirectionalLight;

//Texture coordinates received from vertex shader
in vec2 fTexCoord;

//Pixel color
out vec4 pColor;

//Zero if inside shadow, 1 if not
float getShadowFactor()
{	
	//No shadows yet
	return 1.0f;
}

//Calculate intensity based on dot product between normal & light direction
float getIntensity()
{
	//Surface normal
	vec3 normal = normalize(texture(uGBufferNormal, fTexCoord).xyz);
	return max(0.0f, dot(uDirectionalLight.direction, -normal));
}

//Multiply light color with diffuse texture color
vec3 getColor()
{
	//Return product of diffuse and light color
	return uDirectionalLight.color * texture(uGBufferDiffuse, fTexCoord).rgb;
}

void main( void )
{	
	pColor = vec4(getShadowFactor() * getIntensity() * getColor(), 1.0f);
}