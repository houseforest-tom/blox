/*****************************************************************
 **' Main.cpp
 *****************************************************************
 **' Created on: 11.06.2015
 **' Author: HAUSWALD, Tom.
 *****************************************************************
 *****************************************************************/

#include <fuel/core/Game.h>
#include "PointLightComponent.h"
#include "Cube.h"

namespace blox
{
	class EngineTest : public fuel::Game, public fuel::GameComponent
	{
	private:
		// Cubes
		shared_ptr<Cube> m_pCubes[2];

		// Point lights
		shared_ptr<PointLightComponent> m_pPointLights[2];

	public:
		/**
		 * Load resources and setup scene.
		 */
		void setup(void) override
		{
			// Load textures
			getTextureManager().add("grass", "res/textures/grass.png");

			// Setup cubes
			m_pCubes[0] = make_shared<Cube>();
			m_pCubes[0]->getTransform().setPosition({-3.75f, 0.0f, 0.0f});
			m_pCubes[1] = make_shared<Cube>();
			m_pCubes[1]->getTransform().setPosition({3.75f, 0.0f, 0.0f});

			// Setup point lights
			m_pPointLights[0] = make_shared<PointLightComponent>(glm::vec3(-2, 0, 0), glm::vec3(1, 0, 0), 0.05f, 15.0f);
			m_pPointLights[1] = make_shared<PointLightComponent>(glm::vec3(+2, 0, 0), glm::vec3(0, 0, 1), 0.05f, 15.0f);

			getCamera().getTransform().setPosition({0, 0, 8});

			// Start scene
			setSceneRoot(this);
			addChild("cubes[0]", m_pCubes[0]);
			addChild("cubes[1]", m_pCubes[1]);
			addChild("pointlights[0]", m_pPointLights[0]);
			addChild("pointlights[1]", m_pPointLights[1]);
		}
	};
}

int main(int argc, char **argv)
{
	blox::EngineTest test;
	test.run();
}
