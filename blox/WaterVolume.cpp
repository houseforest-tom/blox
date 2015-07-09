/*****************************************************************
 * WaterSurface.cpp
 *****************************************************************
 * Created on: 09.07.2015
 * Author: HAUSWALD, Tom.
 *****************************************************************
 *****************************************************************/

#include "WaterVolume.h"
#include <cstdlib>
#include <fuel/core/Game.h>

namespace fuel
{
	WaterVolume::WaterVolume(const glm::vec3 &center, const glm::vec3 &size)
		:m_center(center), m_size(size), m_vertices(3), m_indices(GL_ELEMENT_ARRAY_BUFFER)
	{
		constexpr unsigned indexCount = WATER_SURFACE_TESSELATION * WATER_SURFACE_TESSELATION * 6;
		std::vector<uint32_t> indices(indexCount);

		unsigned i = 0;
		for (unsigned U = 0; U < WATER_SURFACE_TESSELATION; U++)
		{
			for (unsigned V = 0; V < WATER_SURFACE_TESSELATION; V++)
		   {
				unsigned offset = U * WATER_SURFACE_TESSELATION + V;
				indices[i++] = offset + 0;
				indices[i++] = offset + 1;
				indices[i++] = offset + WATER_SURFACE_TESSELATION;
				indices[i++] = offset + 1;
				indices[i++] = offset + WATER_SURFACE_TESSELATION + 1;
				indices[i++] = offset + WATER_SURFACE_TESSELATION;
			}
		}

		m_indices.write<uint32_t>(GL_STATIC_DRAW, indices);
	}

	glm::vec3 WaterVolume::calculateSurfacePosition(float u, float v, float t)
	{
		// Initial position
		glm::vec3 result(
			m_center.x + (u - 0.5f) * m_size.x,
			m_center.y + m_size.y / 2,
			m_center.z + (v - 0.5f) * m_size.z
		);

		float omega, phi, dot, contribution;
		for(const auto &wave : m_waves)
		{
			// Angular speed
			omega = 2 * glm::pi<float>() / wave.wavelength;
			phi = wave.speed * omega;

			// Dot product of wave direction and current position
			dot = glm::dot(wave.direction, {u, v});

			// X-directional contribution
			contribution = wave.steepness * wave.amplitude * cosf(omega * dot + phi * t);
			result.x += contribution * wave.direction.x;

			// Z-directional contribution
			result.z += contribution * wave.direction.y;

			// Height contribution
			result.y += wave.amplitude * sinf(omega * dot + phi * t);
		}

		return result;
	}

	glm::vec3 WaterVolume::calculateSurfaceTangent(float u, float v, float t)
	{
		glm::vec3 result;
		return result;
	}

	glm::vec3 WaterVolume::calculateSurfaceBitangent(float u, float v, float t)
	{
		glm::vec3 result;
		return result;
	}

	glm::vec3 WaterVolume::calculateSurfaceNormal(float u, float v, float t)
	{
		glm::vec3 result = {0, 1.0f, 0};
		return result;
	}

	unsigned WaterVolume::getWaveCount(void) const
	{
		return m_waves.size();
	}

	WaterWave &WaterVolume::getWave(unsigned num)
	{
		return m_waves[num];
	}

	void WaterVolume::addWave(const WaterWave &wave)
	{
		m_waves.push_back(wave);
	}

	void WaterVolume::clear(void)
	{
		m_waves.clear();
		m_waves.shrink_to_fit();
	}

	void WaterVolume::update(fuel::Game &game, float dt)
	{
		float time = glfwGetTime();
		constexpr unsigned vertices = WATER_SURFACE_TESSELATION * WATER_SURFACE_TESSELATION;
		std::vector<float> positions(3 * vertices), normals(3 * vertices), colors(3 * vertices);

		unsigned U, V; 		// [0, TESSELATION)
		float u, v; 		// [0, 1)
		unsigned index;		// Vertex data index (float-wise)
		glm::vec3 vector;	// Temporary value

		for(U = 0; U < WATER_SURFACE_TESSELATION; ++U)
		{
			for(V = 0; V < WATER_SURFACE_TESSELATION; ++V)
			{
				u = (float)U / WATER_SURFACE_TESSELATION;
				v = (float)V / WATER_SURFACE_TESSELATION;

				index = 3 * (U + V * WATER_SURFACE_TESSELATION);

				// Store positions
				vector = calculateSurfacePosition(u, v, time);
				positions[index + 0] = vector.x;
				positions[index + 1] = vector.y;
				positions[index + 2] = vector.z;

				// Store normals
				vector = calculateSurfaceNormal(u, v, time);
				normals[index + 0] = vector.x;
				normals[index + 1] = vector.y;
				normals[index + 2] = vector.z;

				// Store colors
				vector = {0, 0.5f, 0.8f};
				colors[index + 0] = vector.r;
				colors[index + 1] = vector.g;
				colors[index + 2] = vector.b;
			}
		}

		m_vertices.getAttributeList(0).write<float, 3>(GL_DYNAMIC_DRAW, GL_FLOAT, positions);
		m_vertices.getAttributeList(1).write<float, 3>(GL_DYNAMIC_DRAW, GL_FLOAT, normals);
		m_vertices.getAttributeList(2).write<float, 3>(GL_DYNAMIC_DRAW, GL_FLOAT, colors);

		GameComponent::update(game, dt);
	}

	void WaterVolume::geometryPass(fuel::Game &game)
	{
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		game.getShaderManager().get("DeferredPositionNormalColor").use();
		game.getShaderManager().get("DeferredPositionNormalColor").getUniform("uWVP").set(game.calculateViewProjectionMatrix());
		game.getShaderManager().get("DeferredPositionNormalColor").getUniform("uWorld").set(glm::mat4(1.0f));
		game.getWindow().renderGeometry<uint32_t>(m_vertices, m_indices, GL_UNSIGNED_INT, GL_TRIANGLES);
		//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		GameComponent::geometryPass(game);
	}
}


