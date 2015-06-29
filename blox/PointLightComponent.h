/*****************************************************************
 * PointLight.h
 *****************************************************************
 * Created on: 27.06.2015
 * Author: HAUSWALD, Tom.
 *****************************************************************
 *****************************************************************/

#ifndef POINTLIGHTCOMPONENT_H_
#define POINTLIGHTCOMPONENT_H_

#include <fuel/core/Game.h>
#include <fuel/graphics/lighting/PointLight.h>

namespace blox
{
	class PointLightComponent : public fuel::GameComponent, public fuel::PointLight
	{
	public:
		PointLightComponent(const glm::vec3 &position, const glm::vec3 &color, float linearAttenuation, float radius)
		{
			this->position = position;
			this->color = color;
			this->setRadius(linearAttenuation, radius);
		}

		void update(fuel::Game &game, float dt) override
		{

		}

		void fullscreenPass(fuel::Game &game) override
		{
			auto &shader = game.getShaderManager().get("pointlight");
			shader.getUniform("uPosition").set(position);
			shader.getUniform("uColor").set(color);
			shader.getUniform("uRadius").set(getRadius());
			shader.getUniform("uLinearAttenuation").set(linearAttenuation);
			shader.getUniform("uQuadraticAttenuation").set(quadraticAttenuation);
			game.getWindow().renderFullscreenQuad();
		}
	};
}


#endif // POINTLIGHTCOMPONENT_H_
