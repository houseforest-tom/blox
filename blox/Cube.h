/*****************************************************************
 * Cube.h
 *****************************************************************
 * Created on: 27.06.2015
 * Author: HAUSWALD, Tom.
 *****************************************************************
 *****************************************************************/

#ifndef CUBE_H_
#define CUBE_H_

#include <fuel/core/GameComponent.h>
#include <fuel/core/Transform.h>
#include <fuel/graphics/GLVertexArray.h>

namespace blox
{
	using namespace std;

	class Cube : public fuel::GameComponent
	{
	private:
		// Name of texture to use
		string m_textureName;

		// VAO
		fuel::GLVertexArray m_vertices;

		// IBO
		fuel::GLBuffer m_indices;

		// Orientation of the cube
		fuel::Transform m_transform;

	public:
		/**
		 * Instantiates a new cube.
		 */
		Cube(void);

		/**
		 * Returns the transform of this cube.
		 *
		 * @return Transform.
		 */
		inline fuel::Transform &getTransform(void){ return m_transform; }

		/**
		 * Animates the cube.
		 *
		 * @param game
		 * 		Parent game.
		 *
		 * 	@param dt
		 * 		Time since last frame in seconds.
		 */
		void update(fuel::Game &game, float dt) override;

		/**
		 * Renders the cube.
		 *
		 * @param game
		 * 		Parent game.
		 */
		void geometryPass(fuel::Game &game) override;
	};
}

#endif // CUBE_H_
