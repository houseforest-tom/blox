/*****************************************************************
 * WaterSurface.cpp
 *****************************************************************
 * Created on: 09.07.2015
 * Author: HAUSWALD, Tom.
 *****************************************************************
 *****************************************************************/

#include "WaterSurface.h"
#include <cstdlib>

namespace fuel
{
	WaterSurface::WaterSurface(void)
		:m_waves(0)
	{

	}

	WaterSurface::WaterSurface(unsigned numWaves)
		:m_waves(numWaves)
	{
		for(unsigned wave=0; wave<numWaves; ++wave)
		{
			m_waves[wave].direction = glm::normalize(glm::vec2(rand(), rand()));
			m_waves[wave].steepness = rand() / (float)RAND_MAX;
			m_waves[wave].amplitude = rand() / (float)RAND_MAX;
			m_waves[wave].wavelength = rand() / (float)RAND_MAX * 2;
			m_waves[wave].speed = rand() / (float)RAND_MAX * 5;
		}
	}

	unsigned WaterSurface::getWaveCount(void) const
	{
		return m_waves.size();
	}

	WaterWave &WaterSurface::getWave(unsigned num)
	{
		return m_waves[num];
	}

	void WaterSurface::addWave(const WaterWave &wave)
	{
		m_waves.push_back(wave);
	}

	void WaterSurface::clear(void)
	{
		m_waves.clear();
		m_waves.shrink_to_fit();
	}

	void WaterSurface::update(fuel::Game &game, float dt)
	{
		GameComponent::update(game, dt);
	}

	void WaterSurface::geometryPass(fuel::Game &game)
	{
		GameComponent::geometryPass(game);
	}
}


