/*****************************************************************
 * WaterSurface.h
 *****************************************************************
 * Created on: 08.07.2015
 * Author: HAUSWALD, Tom.
 *****************************************************************
 *****************************************************************/

#ifndef WATERSURFACE_H_
#define WATERSURFACE_H_

#include "WaterWave.h"
#include <vector>
#include <fuel/graphics/GLCalls.h>
#include <fuel/core/GameComponent.h>

namespace fuel
{
	class WaterSurface : public fuel::GameComponent
	{
	private:
		std::vector<WaterWave> m_waves;

	public:
		/**
		 * Instantiates an flat water surface without waves.
		 */
		WaterSurface(void);

		/**
		 * Instantiates a new water surface using the specified number
		 * of randomly paramterized waves.
		 *
		 * @param numWaves
		 * 		Number of initial waves.
		 */
		WaterSurface(unsigned numWaves);

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



#endif // WATERSURFACE_H_
