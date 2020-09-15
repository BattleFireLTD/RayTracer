#pragma once
#include "Alice.h"
#include "AliceVector3.h"
#include "Perlin.h"
namespace Alice {
	class Texture {
	public:
		virtual Vector3 Sample(float u, float v, const Vector3 & p) const = 0;
	};
	class ConstantTexture : public Texture {
	public:
		Vector3 mColor;
		ConstantTexture(){}
		ConstantTexture(Vector3 color):mColor(color){}
		virtual Vector3 Sample(float u, float v, const Vector3 & p) const {
			return mColor;
		}
	};
	class CheckerTexture : public Texture {
	public:
		Texture *mOdd,*mEven;
		CheckerTexture() {}
		CheckerTexture(Texture*t0,Texture*t1) :mOdd(t0),mEven(t1) {}
		virtual Vector3 Sample(float u, float v, const Vector3 & p)  const {
			float sines = sinf(10.0f*p.x)*sinf(10.0f*p.y)*sinf(10.0f*p.z);
			if (sines<0.0f){
				return mOdd->Sample(u, v, p);
			}
			else {
				return mEven->Sample(u, v, p);
			}
		}
	};
	class NoiseTexture :public Texture {
	public:
		Perlin mPerlinNoise;
		float mScale;
		NoiseTexture():mScale(1.0f){}
		NoiseTexture(float scale):mScale(scale){}
		virtual Vector3 Sample(float u, float v, const Vector3 & p) const {
			return Vector3(1.0f, 1.0f, 1.0f)*mPerlinNoise.NoiseTrilinearHermiteCubic(mScale*p);
			//return Vector3(1.0f, 1.0f, 1.0f)*0.5f*(1.0f+sinf(mScale*p.x)+5.0f*mPerlinNoise.NoiseTrilinearHermiteCubic(p));
		}
	};
}