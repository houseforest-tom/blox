/*****************************************************************
 * WaterWave.h
 *****************************************************************
 * Created on: 08.07.2015
 * Author: HAUSWALD, Tom.
 *****************************************************************
 *****************************************************************/

#ifndef WATERWAVE_H_
#define WATERWAVE_H_

#include <fuel/graphics/GLCalls.h>

namespace fuel
{
	struct WaterWave
	{
		// Direction the wave is travelling
		glm::vec2 direction;

		// Steepness of the wave [0..1]
		float steepness;

		// Amplitude of the wave
		float amplitude;

		// Wavelength := 2 x PI / frequency
		float wavelength;

		// Wave speed
		float speed;
	};
}


#endif // WATERWAVE_H_
