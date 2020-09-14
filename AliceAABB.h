#pragma once
#include "Alice.h"
#include "AliceVector3.h"
#include "AliceRay.h"
#include "AliceTree.h"
namespace Alice {
	class AABB : public BoundingVolume {
	public:
		AABB(const Vector3 & a, const Vector3 & b) : BoundingVolume(kBoundingVolumeTypeAABB) {
			mMin = a;
			mMax = b;
		}
		bool HitTest(const Ray & input_ray, float t_min, float t_max) const;
		BoundingVolume*Clone()const;
	public:
		Vector3 mMin, mMax;
	};
}