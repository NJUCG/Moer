/**
 * @file Color.h
 * @author orbitchen
 * @brief Spectrum implemention.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../Core/CoreLayer/Color.h"

double clamp(double source, double low, double high)
{
	if (source < low) return low;
	if (source > high) return high;
	return source;
}

double lerp(double source0, double source1, double ratio)
{
	return (1.0f - ratio) * source0 + ratio * source1;
}