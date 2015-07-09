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
#define RANDF(MIN, MAX) ((MIN) + ((MAX)-(MIN)) * rand() / (float)RAND_MAX)

namespace blox
{
	class PointLightComponent : public fuel::GameComponent, public fuel::PointLight
	{
	private:
		float m_radius;

	public:
		PointLightComponent(const glm::vec3 &position, const glm::vec3 &color, float radius)
			:m_radius(radius)
		{
			this->position = position;
			this->color = color;
		}

		void update(fuel::Game &game, float dt) override
		{
			//position.x = sinf(glfwGetTime()) * 3.0f;
		}

		void fullscreenPass(fuel::Game &game) override
		{
			auto &pointlightShader = game.getShaderManager().get("DeferredPointlight");
			pointlightShader.use();
			pointlightShader.getUniform("uPointLight.position").set(position);
			pointlightShader.getUniform("uPointLight.color").set(color);
			pointlightShader.getUniform("uPointLight.radius").set(m_radius);
			game.getWindow().renderFullscreenQuad();
		}
	};
}


#endif // POINTLIGHTCOMPONENT_H_
