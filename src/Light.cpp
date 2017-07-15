#include "Light.h"

Light::Light(LightType lightType)
{
	switch(lightType)
	{
	case LightType::Directional:
		m_LightData = new DLightData();
		break;
	case LightType::Point:
		m_LightData = new PLightData();
		break;
	case LightType::Spot:
		m_LightData = new SLightData();
		break;
	default:
		assert(-1); // Invalid lightype passed!
	}

	m_LightType = lightType;
}