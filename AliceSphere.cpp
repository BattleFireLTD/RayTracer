#include "AliceSphere.h"
#include "AliceAABB.h"
#include "utils.h"
namespace Alice {
	Sphere::Sphere(const Vector3 & origin, float radius) {
		mOrigin = origin;
		mRadius = radius;
	}
	void Sphere::GetSphericalUV(Vector3 & p, float & u, float & v) const {
		float phi = atan2f(p.z, p.x);
		float theta = asinf(p.y);
		u = 1.0f - (phi + 3.1415f) / (2.0f*3.1415f);
		v = (theta+3.1415f/2.0f) / 3.1415f;
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
				GetSphericalUV(hit_point.mPosition, hit_point.mTexcoord.x, hit_point.mTexcoord.y);
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
	bool XYPlane::HitTest(const Ray & input_ray, float t_min, float t_max, HitPoint & hit_point) const {
		float t = (mK - input_ray.mOrigin.z) / input_ray.mDirection.z;
		if (t<t_min||t>t_max){
			return false;
		}
		float x = input_ray.mOrigin.x + t * input_ray.mDirection.x;
		float y = input_ray.mOrigin.y + t * input_ray.mDirection.y;
		if (x<mX0||x>mX1||y<mY0||y>mY1){
			return false;
		}
		hit_point.mPosition = input_ray.PointAt(t);
		hit_point.mNormal = Vector3(0.0f,0.0f,1.0f);
		hit_point.mTValue = t;
		hit_point.mTexcoord = Vector3((x-mX0)/(mX1-mX0), (y - mY0) / (mY1 - mY0),0.0f);
		return true;
	}
	bool XYPlane::BuildBoundingVolume(BoundingVolume ** bv, float time0 /* = 0.0f */, float time1 /* = 0.0f */) {
		*bv = new AABB(Vector3(mX0, mY0, mK - 0.0001f), Vector3(mX1, mY1, mK + 0.0001f));
		return true;
	}
}