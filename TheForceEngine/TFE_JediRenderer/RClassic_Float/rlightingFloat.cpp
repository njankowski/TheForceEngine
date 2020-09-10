#include "rlightingFloat.h"
#include "../rmath.h"
#include "../rcommon.h"
#include "../rlimits.h"

namespace TFE_JediRenderer
{

namespace RClassic_Float
{
	#define LIGHT_SCALE_FLOAT 4.0f
	#define LIGHT_ATTEN (3.0f / 32.0f)

	const u8* computeLighting(f32 depth, s32 lightOffset)
	{
		if (s_sectorAmbient >= MAX_LIGHT_LEVEL)
		{
			return nullptr;
		}
		depth = max(depth, 0.0f);
		s32 light = 0;

		// handle camera lightsource
		if (s_worldAmbient < MAX_LIGHT_LEVEL && s_cameraLightSource != 0)
		{
			s32 depthScaled = min(s32(depth * LIGHT_SCALE_FLOAT), 127);
			s32 lightSource = MAX_LIGHT_LEVEL - (s_lightSourceRamp[depthScaled] + s_worldAmbient);
			if (lightSource > 0)
			{
				light += lightSource;
			}
		}

		s32 secAmb = s_sectorAmbient;
		if (light < secAmb) { light = secAmb; }

		// depthScale = 0.09375 (3/32)
		// light = max(light - depth * depthScale, secAmb*0.875)
		s32 depthAtten = s32(depth * LIGHT_ATTEN);		// depth/16 + depth/32
		light = max(light - depthAtten, s_scaledAmbient);

		if (lightOffset != 0)
		{
			light += lightOffset;
		}
		if (light >= MAX_LIGHT_LEVEL) { return nullptr; }
		light = max(light, 0);

		return &s_colorMap[light << 8];
	}
}  // RClassic_Float

}  // TFE_JediRenderer