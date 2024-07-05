#include "Mathf.h"

float Mathf::Lerp(float a, float b, float t)
{
    return a * (1.0f - t) + (b * t);
}
