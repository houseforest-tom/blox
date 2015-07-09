/*****************************************************************
 **' Main.cpp
 *****************************************************************
 **' Created on: 11.06.2015
 **' Author: HAUSWALD, Tom.
 *****************************************************************
 *****************************************************************/

#include <fuel/core/Game.h>
#include <fuel/core/GameComponent.h>
#include "PointLightComponent.h"
#include "WaterVolume.h"
#include "Cube.h"

namespace blox
{
	class EngineTest : public fuel::Game, public fuel::GameComponent
	{
	private:
		/**
		 * Load all GLSL shader resources.
		 */
		void loadShaders(void)
		{
			auto &shaderMgr = getShaderManager();

			// Deferred shader (targeting multiple output textures: gbuffer)
			// for textured objects
			shaderMgr.add("DeferredPositionNormalTexcoord",
						  "res/glsl/pos_norm_tex.vert",
						  "res/glsl/deferred_pos_norm_tex.frag");
			{
				auto &sh = shaderMgr.get("DeferredPositionNormalTexcoord");
				sh.bindVertexAttribute(0, "vPosition");
				sh.bindVertexAttribute(1, "vNormal");
				sh.bindVertexAttribute(2, "vTexCoord");
				sh.link();
				sh.registerUniform("uWVP");
				sh.registerUniform("uWorld");
				sh.registerUniform("uDiffuseTexture");
				sh.getUniform("uDiffuseTexture").set(0);
			}

			// Deferred shader (targeting multiple output textures: gbuffer)
			// for plain colored objects
			shaderMgr.add("DeferredPositionNormalColor",
						  "res/glsl/pos_norm_col.vert",
						  "res/glsl/deferred_pos_norm_col.frag");
			{
				auto &sh = shaderMgr.get("DeferredPositionNormalColor");
				sh.bindVertexAttribute(0, "vPosition");
				sh.bindVertexAttribute(1, "vNormal");
				sh.bindVertexAttribute(2, "vColor");
				sh.link();
				sh.registerUniform("uWVP");
				sh.registerUniform("uWorld");
			}

			// Ambient light shader
			shaderMgr.add("DeferredAmbient",
						  "res/glsl/fullscreen.vert",
						  "res/glsl/deferred_ambient.frag");
			{
				auto &sh = shaderMgr.get("DeferredAmbient");
				sh.bindVertexAttribute(0, "vPosition");
				sh.bindVertexAttribute(1, "vTexCoord");
				sh.link();
				sh.registerUniform("uGBufferDiffuse");
				sh.registerUniform("uAmbientLight.color");
				sh.getUniform("uGBufferDiffuse").set(0);
				sh.getUniform("uAmbientLight.color").set(glm::vec3(1.0f, 0.9f, 0.5f) / 16.0f);
			}

			// Directional light shader
			shaderMgr.add("DeferredDirectional",
						  "res/glsl/fullscreen.vert",
						  "res/glsl/deferred_directional.frag");
			{
				auto &sh = shaderMgr.get("DeferredDirectional");
				sh.bindVertexAttribute(0, "vPosition");
				sh.bindVertexAttribute(1, "vTexCoord");
				sh.link();
				sh.registerUniform("uDirectionalLight.direction");
				sh.registerUniform("uDirectionalLight.color");
				sh.registerUniform("uGBufferDiffuse");
				sh.registerUniform("uGBufferNormal");
				sh.registerUniform("uGBufferDepth");
				sh.getUniform("uGBufferDiffuse").set(0);
				sh.getUniform("uGBufferNormal").set(1);
				sh.getUniform("uGBufferDepth").set(2);
				sh.getUniform("uDirectionalLight.color").set(glm::vec3(1.0f, 0.9f, 0.5f) / 1.75f);
				sh.getUniform("uDirectionalLight.direction").set(glm::normalize(glm::vec3(1, -1, -1)));
			}

			// Point light shader
			shaderMgr.add("DeferredPointlight",
						  "res/glsl/fullscreen.vert",
						  "res/glsl/deferred_pointlight.frag");
			{
				auto &sh = shaderMgr.get("DeferredPointlight");
				sh.bindVertexAttribute(0, "vPosition");
				sh.bindVertexAttribute(1, "vTexCoord");
				sh.link();
				sh.registerUniform("uViewProjection");
				sh.registerUniform("uInverseViewProjection");
				sh.registerUniform("uCameraPosition");
				sh.registerUniform("uGBufferDiffuse");
				sh.registerUniform("uGBufferNormal");
				sh.registerUniform("uGBufferDepth");
				sh.registerUniform("uPointLight.position");
				sh.registerUniform("uPointLight.color");
				sh.registerUniform("uPointLight.radius");
				sh.getUniform("uGBufferDiffuse").set(0);
				sh.getUniform("uGBufferNormal").set(1);
				sh.getUniform("uGBufferDepth").set(2);
			}
		}

		/**
		 * Load all texture resources.
		 */
		void loadTextures(void)
		{
			auto &txrMgr = getTextureManager();
			txrMgr.add("Grass", "res/textures/grass.png");
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

			// Setup water
			//addChild("Water", std::make_shared<fuel::WaterVolume>(glm::vec3(0, -5, 0), glm::vec3(1, 1, 1)));
			fuel::WaterWave wave;
			wave.amplitude = 5;
			wave.direction = {0.707f, 0.707f};
			wave.speed = 1.0f;
			wave.steepness = 0.5f;
			wave.wavelength = 20.0f;
			//((fuel::WaterVolume *)getChild("Water").get())->addWave(wave);

			addChild("Cube", std::make_shared<Cube>());
			((Cube *)getChild("Cube").get())->getTransform().setPosition({0, 0, 5});

			// Setup point lights
			addChild("Lamp", std::make_shared<PointLightComponent>(glm::vec3(0, 0, 10), glm::vec3(1, 1, 1), 10.0f));

			// Move camera
			getCamera().getTransform().setPosition({0, 0, 10});

			// Start scene
			setSceneRoot(this);
		}

		void update(Game &game, float dt) override
		{
			glm::mat4 viewProj = game.calculateViewProjectionMatrix();
			game.getShaderManager().get("DeferredPointlight").getUniform("uViewProjection").set(viewProj);
			game.getShaderManager().get("DeferredPointlight").getUniform("uInverseViewProjection").set(glm::inverse(viewProj));
			game.getShaderManager().get("DeferredPointlight").getUniform("uCameraPosition").set(game.getCamera().getTransform().getPosition());

			GameComponent::update(game, dt); //update children
		}

		void fullscreenPass(Game &game) override
		{
			auto &ambientShader = game.getShaderManager().get("DeferredAmbient");
			ambientShader.use();
			ambientShader.getUniform("uAmbientLight.color").set(glm::vec3(0.1f, 0.1f, 0.16f));
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
