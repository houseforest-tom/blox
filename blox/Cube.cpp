/*****************************************************************
 * Cube.cpp
 *****************************************************************
 * Created on: 27.06.2015
 * Author: HAUSWALD, Tom.
 *****************************************************************
 *****************************************************************/

#include "Cube.h"
#include <fuel/core/Game.h>

namespace blox
{
	Cube::Cube(void)
		:m_textureName("grass"),
		 m_vertices(3),
		 m_indices(GL_ELEMENT_ARRAY_BUFFER),
		 m_transform()
	{
		fuel::GLVertexArray::bind(m_vertices);

		// Setup vertices
		m_vertices.getAttributeList(0).write<float, 3>(GL_STATIC_DRAW, GL_FLOAT,
		{
				// Front face
				-1.0f, -1.0f, 1.0f,
				1.0f, -1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				-1.0f, 1.0f, 1.0f,

				// Right face
				1.0f, -1.0f, 1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, 1.0f, -1.0f,
				1.0f, 1.0f, 1.0f,

				// Back face
				1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f, 1.0f, -1.0f,
				1.0f, 1.0f, -1.0f,

				// Left face
				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f, 1.0f,
				-1.0f, 1.0f, 1.0f,
				-1.0f, 1.0f, -1.0f,

				// Top Face
				-1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, -1.0f,
				-1.0f, 1.0f, -1.0f,

				// Bottom Face
				1.0f, -1.0f, 1.0f,
				-1.0f, -1.0f, 1.0f,
				-1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f
		});

		// Setup indices
		m_vertices.getAttributeList(1).write<float, 3>(GL_STATIC_DRAW, GL_FLOAT,
		{
				// Front face
				0, 0, 1,
				0, 0, 1,
				0, 0, 1,
				0, 0, 1,

				// Right face
				1, 0, 0,
				1, 0, 0,
				1, 0, 0,
				1, 0, 0,

				// Back face
				0, 0, -1,
				0, 0, -1,
				0, 0, -1,
				0, 0, -1,

				// Left face
				-1, 0, 0,
				-1, 0, 0,
				-1, 0, 0,
				-1, 0, 0,

				// Top Face
				0, 1, 0,
				0, 1, 0,
				0, 1, 0,
				0, 1, 0,

				// Bottom Face
				0, -1, 0,
				0, -1, 0,
				0, -1, 0,
				0, -1, 0,
		});

		// Setup texture coordinates
		m_vertices.getAttributeList(2).write<float, 2>(GL_STATIC_DRAW, GL_FLOAT,
		{
				0, 0,
				1, 0,
				1, 1,
				0, 1,

				0, 0,
				1, 0,
				1, 1,
				0, 1,

				0, 0,
				1, 0,
				1, 1,
				0, 1,

				0, 0,
				1, 0,
				1, 1,
				0, 1,

				0, 0,
				1, 0,
				1, 1,
				0, 1,

				0, 0,
				1, 0,
				1, 1,
				0, 1
		});

		// Setup indices
		m_indices.write<uint16_t>(GL_STATIC_DRAW,
		{
				// Front face
				0, 1, 2, 2, 3, 0,

				// Right face
				4, 5, 6, 6, 7, 4,

				// Back face
				8, 9, 10, 10, 11, 8,

				// Left face
				12, 13, 14, 14, 15, 12,

				// Top Face
				16, 17, 18, 18, 19, 16,

				// Bottom Face
				20, 21, 22, 22, 23, 20
		});
	}

	void Cube::update(fuel::Game &game, float dt)
	{
		m_transform.getRotation().y += dt * 180.0f;

		glm::vec3 minScale(0.25f, 0.25f, 0.25f);
		glm::vec3 maxScale(1.50f, 1.50f, 1.50f);
		m_transform.setScale(minScale + (maxScale - minScale) * (1 + sinf(glfwGetTime())) / 2.0f);
	}

	void Cube::geometryPass(fuel::Game &game)
	{
		auto world = m_transform.calculateWorldMatrix();
		fuel::GLTexture::bind(0, game.getTextureManager().get(m_textureName));
		game.getShaderManager().get("textured").getUniform("uWorld").set(world);
		game.getShaderManager().get("textured").getUniform("uWVP").set(game.calculateViewProjectionMatrix() * world);
		game.getWindow().renderGeometry<uint16_t>(m_vertices, m_indices, GL_UNSIGNED_SHORT);
	}
}
