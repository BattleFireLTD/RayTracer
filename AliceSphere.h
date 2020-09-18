#pragma once
#include "AliceVector3.h"
#include "AliceRay.h"
#include "LinkedList.h"
#include "AliceMaterial.h"
#include "AliceTree.h"
namespace Alice {
	class Sphere : public Geometry {
	public:
		Vector3 mOrigin;
		float mRadius;
		Sphere(const Vector3 & origin, float radius);
		void GetSphericalUV(Vector3 & p,float & u,float & v) const;
		bool HitTest(const Ray & input_ray, float t_min, float t_max, HitPoint & hit_point) const;
		bool BuildBoundingVolume(BoundingVolume ** bv,float time0 = 0.0f, float time1 = 0.0f);
	};
	class XYPlane :public Geometry {
	public:
		XYPlane() {}
		XYPlane(float x0,float x1,float y0,float y1,float k):mX0(x0),mX1(x1),mY0(y0),mY1(y1),mK(k) {}
		bool HitTest(const Ray & input_ray, float t_min, float t_max, HitPoint & hit_point) const;
		bool BuildBoundingVolume(BoundingVolume ** bv, float time0 = 0.0f, float time1 = 0.0f);
	public:
		float mX0, mX1, mY0, mY1, mK;
	};
}