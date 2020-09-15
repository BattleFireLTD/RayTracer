#pragma once
#include "Alice.h"
#include "AliceVector3.h"
namespace Alice {
	class Perlin {
	public:
		float Noise0(const Vector3 & p)const;
		float Noise1(const Vector3 & p)const;
		static float *mRandomFloat;
		static int *mPermX,*mPermY,*mPermZ;
	};
}