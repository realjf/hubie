#pragma once

namespace Hubie::Maths
{
	void SetRandomSeed(unsigned seed);
	unsigned GetRandomSeed();
	int Rand();
	float RandStandardNormalised();
}