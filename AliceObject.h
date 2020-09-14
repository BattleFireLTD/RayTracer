#pragma once
#include "LinkedList.h"
#include "AliceMatrix4x4.h"
#include "AliceVector3.h"
namespace Alice {
	class BVHNode;
	class Material;
	class Geometry;
	class Ray;
	struct HitPoint;
	class Object : public LinkedList {
	public:
		Geometry*mGeometry;
		Material*mMaterial;
		Matrix4x4 mWorldTransform;
		char mName[64];
	public:
		Object();
		Object(Geometry*geometry, Material*material);
		~Object();
		void Set(Geometry*geometry, Material*material);
		void SetGeometry(Geometry*geometry);
		void SetMaterial(Material*material);
		void SetName(const char * name);
		void SetPosition(float x, float y, float z);
		void SetPosition(const Vector3 & position);
		bool HitTest(const Ray & input_ray, float t_min, float t_max, HitPoint & hit_point) const;
	};
}