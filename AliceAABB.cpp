#include "AliceAABB.h"
#include "AliceTree.h"
namespace Alice {
	bool AABB::HitTest(const Ray & input_ray, float tmin, float tmax) const {
		for (int i=0;i<3;++i){
			float invD = 1.0f / input_ray.mDirection[i];
			float t0 = (mMin[i] - input_ray.mOrigin[i])*invD;
			float t1 = (mMax[i] - input_ray.mOrigin[i])*invD;
			if (invD<0.0f){
				std::swap(t0, t1);
			}
			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;
			if (tmax<=tmin){
				return false;
			}
		}
		return true;
	}
	BoundingVolume*AABB::Clone() const {
		AABB*aabb = new AABB(mMin,mMax);
		return aabb;
	}
}