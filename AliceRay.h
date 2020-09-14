#pragma once
#include "AliceVector3.h"
namespace Alice {
	class Material;
	struct HitPoint {
		Vector3 mPosition;
		Vector3 mNormal;
		Material*mMaterial;
		float mTValue;
		HitPoint() {
			mMaterial = nullptr;
			mTValue = 0.0f;
		}
		void operator=(const HitPoint & r) {
			mPosition = r.mPosition;
			mNormal = r.mNormal;
			mMaterial = r.mMaterial;
			mTValue = r.mTValue;
		}
	};
	class Ray {
	public:
		Vector3 mOrigin, mDirection, mLightingAttenuation;
	public:
		Ray(){}
		Ray(const Vector3 & origin, const Vector3 & direction, const Vector3 & color) {
			Set(origin, direction,color);
		}
		void Set(const Vector3 & origin, const Vector3 & direction, const Vector3 & attenuation) {
			mOrigin = origin;
			mDirection = direction;
			mDirection.normalize();
			mLightingAttenuation = attenuation;
		}
		Vector3 Origin() const { return mOrigin; }
		Vector3 Direction() const { return mDirection; }
		Vector3 PointAt(float t) const {
			return mOrigin + mDirection * t;
		}
	};
}