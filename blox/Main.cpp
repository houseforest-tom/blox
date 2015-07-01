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
		vector<shared_ptr<Cube>> m_cubes;

		// Point lights
		vector<shared_ptr<PointLightComponent>> m_pointLights;

		/**
		 * Load all GLSL shader resources.
		 */
		void loadShaders(void)
		{
			auto &shaderMgr = getShaderManager();

			// Deferred shader (targeting multiple output textures: gbuffer)
			shaderMgr.add("textured", "res/glsl/geometry.vert", "res/glsl/textured.frag");
			{
				auto &sh = shaderMgr.get("textured");
				sh.bindVertexAttribute(0, "vPosition");
				sh.bindVertexAttribute(1, "vNormal");
				sh.bindVertexAttribute(2, "vTexCoord");
				sh.link();
				sh.registerUniform("uWVP");
				sh.registerUniform("uWorld");
				sh.registerUniform("uDiffuseTexture");
				sh.getUniform("uDiffuseTexture").set(0);
			}

			// Ambient ligh shader
			shaderMgr.add("ambient", "res/glsl/fullscreen.vert", "res/glsl/ambient.frag");
			{
				auto &sh = shaderMgr.get("ambient");
				sh.bindVertexAttribute(0, "vPosition");
				sh.bindVertexAttribute(1, "vTexCoord");
				sh.link();
				sh.registerUniform("uGBufferDiffuse");
				sh.registerUniform("uAmbientLight.color");
				sh.getUniform("uGBufferDiffuse").set(0);
				sh.getUniform("uAmbientLight.color").set(glm::vec3(1.0f, 0.9f, 0.5f) / 16.0f);
			}

			// Directional light shader
			shaderMgr.add("directional", "res/glsl/fullscreen.vert", "res/glsl/directional.frag");
			{
				auto &sh = shaderMgr.get("directional");
				sh.bindVertexAttribute(0, "vPosition");
				sh.bindVertexAttribute(1, "vTexCoord");
				sh.link();
				sh.registerUniform("uDirectionalLight.direction");
				sh.registerUniform("uDirectionalLight.color");
				sh.registerUniform("uGBufferDiffuse");
				sh.registerUniform("uGBufferNormal");
				sh.registerUniform("uGBufferDepth");
				sh.getUniform("uGBufferDiffuse").set(0);
				sh.getUniform("uGBufferNormal").set(2);
				sh.getUniform("uGBufferDepth").set(3);
				sh.getUniform("uDirectionalLight.color").set(glm::vec3(1.0f, 0.9f, 0.5f) / 1.75f);
				sh.getUniform("uDirectionalLight.direction").set(glm::normalize(glm::vec3(1, -1, -1)));
			}

			// Point light shader
			shaderMgr.add("pointlight", "res/glsl/fullscreen.vert", "res/glsl/pointlight.frag");
			{
				auto &sh = shaderMgr.get("pointlight");
				sh.bindVertexAttribute(0, "vPosition");
				sh.bindVertexAttribute(1, "vTexCoord");
				sh.link();
				sh.registerUniform("uViewProjection");
				sh.registerUniform("uCameraPosition");
				sh.registerUniform("uGBufferDiffuse");
				sh.registerUniform("uGBufferPosition");
				sh.registerUniform("uGBufferNormal");
				sh.registerUniform("uPointLight.position");
				sh.registerUniform("uPointLight.color");
				sh.registerUniform("uPointLight.radius");
				sh.getUniform("uGBufferDiffuse").set(0);
				sh.getUniform("uGBufferPosition").set(1);
				sh.getUniform("uGBufferNormal").set(2);
			}
		}

		/**
		 * Load all texture resources.
		 */
		void loadTextures(void)
		{
			auto &txrMgr = getTextureManager();
			txrMgr.add("grass", "res/textures/grass.png");
		}

	public:
		/**
		 * Load resources and setup scene.
		 */
		void setup(void) override
		{
			// Load shaders
			loadShaders();

			// Load textures
			loadTextures();

			// Setup cubes
			for(unsigned cubeNum = 0; cubeNum < 64; ++cubeNum)
			{
				m_cubes.push_back(make_shared<Cube>());
				m_cubes.back()->getTransform().setPosition({RANDF(-15, 15), RANDF(-15, 15), RANDF(-15, 0)});

				char name[16];
				sprintf(name, "cube%05u", cubeNum);
				addChild(string(name), m_cubes.back());
			}

			// Setup point lights
			m_pointLights.push_back(make_shared<PointLightComponent>(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), 1.5f));
			addChild("pointlight", m_pointLights.back());

			// Move camera
			getCamera().getTransform().setPosition({0, 0, 8});

			// Start scene
			setSceneRoot(this);
		}

		void update(Game &game, float dt) override
		{
			game.getShaderManager().get("pointlight").getUniform("uViewProjection").set(game.calculateViewProjectionMatrix());
			game.getShaderManager().get("pointlight").getUniform("uCameraPosition").set(game.getCamera().getTransform().getPosition());

			GameComponent::update(game, dt); //update children
		}

		void fullscreenPass(Game &game) override
		{
			auto &ambientShader = game.getShaderManager().get("ambient");
			ambientShader.use();
			ambientShader.getUniform("uAmbientLight.color").set(glm::vec3(0.05f, 0.05f, 0.05f));
			game.getWindow().renderFullscreenQuad();

			GameComponent::fullscreenPass(game); //render children
		}
	};
}

int main(int argc, char **argv)
{
	blox::EngineTest test;
	test.run();
}
