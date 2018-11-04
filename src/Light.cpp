#include "Light.h"

Light::Light(LightType lightType)
{

}

Light::~Light() {
    if (m_LightData) delete m_LightData;
}