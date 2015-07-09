/*****************************************************************
 * WaterVolume.h
 *****************************************************************
 * Created on: 08.07.2015
 * Author: HAUSWALD, Tom.
 *****************************************************************
 *****************************************************************/

#ifndef WATERVOLUME_H_
#define WATERVOLUME_H_

#include "WaterWave.h"
#include <vector>
#include <fuel/graphics/GLCalls.h>
#include <fuel/core/GameComponent.h>
#include <fuel/graphics/GLVertexArray.h>

#define WATER_SURFACE_TESSELATION 32

namespace fuel
{
	class WaterVolume : public fuel::GameComponent
	{
	private:
		// Center position
		glm::vec3 m_center;

		// Width, height & length
		glm::vec3 m_size;

		// Vertices
		GLVertexArray m_vertices;

		// Indices
		GLBuffer m_indices;

		// List of waves perturbing the surface
		std::vector<WaterWave> m_waves;

		/**
		 * Calculate the 3D-position of a point on the water surface.
		 *
		 * @param u
		 * 		U coordinate.
		 *
		 * @param v
		 * 		V coordinate.
		 *
		 * @param t
		 * 		Time.
		 *
		 * @return Position of the point on the surface.
		 */
		glm::vec3 calculateSurfacePosition(float u, float v, float t);

		/**
		 * Calculate tangent vector at a point on the water surface.
		 *
		 * @param u
		 * 		U coordinate.
		 *
		 * @param v
		 * 		V coordinate.
		 *
		 * @param t
		 * 		Time.
		 *
		 * @return Tangent vector at the point on the surface.
		 */
		glm::vec3 calculateSurfaceTangent(float u, float v, float t);

		/**
		 * Calculate bitangent vector at a point on the water surface.
		 *
		 * @param u
		 * 		U coordinate.
		 *
		 * @param v
		 * 		V coordinate.
		 *
		 * @param t
		 * 		Time.
		 *
		 * @return Bitangent vector at the point on the surface.
		 */
		glm::vec3 calculateSurfaceBitangent(float u, float v, float t);

		/**
		 * Calculate the normal vector at a point on the water surface.
		 *
		 * @param u
		 * 		U coordinate.
		 *
		 * @param v
		 * 		V coordinate.
		 *
		 * @param t
		 * 		Time.
		 *
		 * @return Normal vector at the point on the surface.
		 */
		glm::vec3 calculateSurfaceNormal(float u, float v, float t);

	public:
		/**
		 * Instantiates a flat water volume without waves.
		 *
		 * @param center
		 * 		Center of the water volume.
		 *
		 * @param size
		 * 		Width, height & length of the volume.
		 */
		WaterVolume(const glm::vec3 &center, const glm::vec3 &size);

		/**
		 * Returns the number of waves in this surface.
		 *
		 * @return
		 * 		Wave count.
		 */
		unsigned getWaveCount(void) const;

		/**
		 * Returns a mutable reference to one of the existing waves.
		 *
		 * @param num
		 * 		Number of the wave.
		 *
		 * @return
		 * 		Wave reference.
		 */
		WaterWave &getWave(unsigned num);

		/**
		 * Adds a wave to the water surface.
		 *
		 * @param wave
		 * 		Wave description.
		 */
		void addWave(const WaterWave &wave);

		/**
		 * Removes all waves from this water surface.
		 */
		void clear(void);

		/**
		 * Updates the water surface, animating the height map.
		 *
		 * @param game
		 * 		Game reference.
		 *
		 * @param dt
		 * 		Time passed since last frame in seconds.
		 */
		void update(fuel::Game &game, float dt) override;

		/**
		 * Render the water surface using the heightmap generated
		 * from the wave information.
		 *
		 * @param game
		 *		Game reference.
		 */
		void geometryPass(fuel::Game &game) override;
	};
}



#endif // WATERVOLUME_H_
