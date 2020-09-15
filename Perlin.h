#pragma once
#include "Alice.h"
#include "AliceVector3.h"
namespace Alice {
	class Perlin {
	protected:
		static Perlin * mSelf;
	public:
		static Perlin * Singleton();
		float NoiseOriginal(const Vector3 & p) const;
		float NoiseTrilinear(const Vector3 & p) const;
		float NoiseTrilinearHermiteCubic(const Vector3 & p) const;
		float NoiseVector3TrilinearHermiteCubic(const Vector3 & p) const;
		float Turb(const Vector3 &p, int depth=7)const;
		static Vector3 *mRandomVectors;
		static float *mRandomFloats;
		static int *mPermX,*mPermY,*mPermZ;
	};
}