#version 400 core

//Billboard effect
const float cBillboardRadius = 0.0625f;
const float cAspectRatio = 16.0f / 9.0f;
const float cCameraMaxDistance = 16.0f;

//ViewProjection matrices
uniform mat4 uViewProjection;
uniform mat4 uInverseViewProjection;

//Camera position in world space
uniform vec3 uCameraPosition;

//GBuffer textures
uniform sampler2D uGBufferDiffuse;
uniform sampler2D uGBufferPosition;
uniform sampler2D uGBufferNormal;

//Point light characteristics
struct PointLight
{
	vec3 position;
	vec3 color;	
	float radius;
};
uniform PointLight uPointLight;

//Input: Texture coordinates received from vertex shader
in vec2 fTexCoord;

//Output: Pixel color
out vec4 pColor;

float getIntensity( void )
{
	//Offset vector from light source
	vec3 offset = uPointLight.position - texture(uGBufferPosition, fTexCoord).xyz;
	//Distance from light to camera
	float distance = length(offset);
	if(distance > uPointLight.radius) discard;

	//Surface normal
	vec3 normal = normalize(texture(uGBufferNormal, fTexCoord).xyz);

	float i = 1.0f - distance / uPointLight.radius;
	i *= dot(offset, normal);
	return max(i, 0.0f);
}

vec3 getColor( void )
{	
	//Return product of diffuse and light color
	return uPointLight.color * texture(uGBufferDiffuse, fTexCoord).rgb;
}

void main( void )
{
	pColor = vec4(getColor() * getIntensity(), 1.0f);

	//Billboard
	vec4  clipSpaceLightPos   = (uViewProjection * vec4(uPointLight.position, 1.0f));
	vec3  screenSpaceLightPos = clipSpaceLightPos.xyz / clipSpaceLightPos.w;
		 screenSpaceLightPos.y /= cAspectRatio;
	vec2  screenSpaceTexCoord = 2.0f * fTexCoord - 1.0f;
		 screenSpaceTexCoord.y /= cAspectRatio;
	float screenSpaceDistance = length(screenSpaceLightPos.xy - screenSpaceTexCoord);
	
	float cameraDistance     = length(uCameraPosition - uPointLight.position);
	float adaptedBillboardSz = cBillboardRadius * (1.0f - cameraDistance / cCameraMaxDistance);

	if(screenSpaceDistance <= adaptedBillboardSz )
		pColor += vec4(uPointLight.color * (1.0f - screenSpaceDistance / adaptedBillboardSz ), 1.0f);
}