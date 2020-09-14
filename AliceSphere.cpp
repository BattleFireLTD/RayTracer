#include "AliceSphere.h"
#include "AliceAABB.h"
#include "utils.h"
namespace Alice {
	Sphere::Sphere(const Vector3 & origin, float radius) {
		mOrigin = origin;
		mRadius = radius;
	}
	bool Sphere::HitTest(const Ray & input_ray, float t_min, float t_max, HitPoint & hit_point) const {
		Alice::Vector3 oc = input_ray.mOrigin - mOrigin;
		float a = input_ray.Direction().LengthSquared();
		float b = oc* input_ray.Direction();
		float c = oc.LengthSquared() - mRadius * mRadius;
		float discriminant = b * b - a * c;
		if (discriminant>0.0f){
			bool hitted = false;
			float temp = (-b - sqrtf(discriminant)) / a;
			if (temp>t_min&&temp <t_max){
				hitted = true;
			}
			else {
				temp = (-b + sqrtf(discriminant)) / a;
				if (temp > t_min&&temp < t_max) {
					hitted = true;
				}
			}
			if (hitted) {
				hit_point.mTValue = temp;
				hit_point.mPosition = input_ray.PointAt(temp);
				hit_point.mNormal = (hit_point.mPosition - mOrigin) * (1.0f / mRadius);
				return true;
			}
		}
		return false;
	}
	bool Sphere::BuildBoundingVolume(BoundingVolume ** bv, float time0 /* = 0.0f */, float time1 /* = 0.0f */) {
		float radius = mRadius > 0.0f ? mRadius : -mRadius;
		*bv = new AABB(Vector3(mOrigin-Vector3(radius, radius, radius)),Vector3(mOrigin+Vector3(radius, radius, radius)));
		return true;
	}
}