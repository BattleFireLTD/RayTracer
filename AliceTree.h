#pragma once
namespace Alice {
	struct HitPoint;
	class Ray;
	class Object;
	class BoundingVolume;
	class Geometry {
	public:
		virtual bool HitTest(const Ray & input_ray, float t_min, float t_max, HitPoint & hit_point) const = 0;
		virtual bool BuildBoundingVolume(BoundingVolume ** bv, float time0 = 0.0f, float time1 = 0.0f) = 0;
	};
	class BoundingVolume {
	public:
		enum BoundingVolumeType{
			kBoundingVolumeTypeAABB,
			kBoundingVolumeTypeSphere,
			kBoundingVolumeTypeCount
		};
	public:
		BoundingVolumeType mType;
		BoundingVolume(BoundingVolumeType t) :mType(t) {}
		virtual ~BoundingVolume(){}
		virtual bool HitTest(const Ray & input_ray, float t_min, float t_max) const = 0;
		virtual BoundingVolume * Clone() const = 0;
	};
	typedef int(*SortFunction)(const void *, const void*);
	class BVHNode {
	public:
		BoundingVolume * mBoundingVolume;
		Object *mUserData;
		BVHNode*mLeftChild, *mRightChild,*mParent;
		BVHNode():mBoundingVolume(nullptr),mUserData(nullptr),mLeftChild(nullptr),mRightChild(nullptr),mParent(nullptr){}
		virtual ~BVHNode() {
			if (mLeftChild != nullptr) {
				delete mLeftChild;
			}
			if (mRightChild != nullptr) {
				delete mRightChild;
			}
			if (mBoundingVolume!=nullptr){
				delete mBoundingVolume;
			}
		}
		virtual bool BuildBoundingVolumeHierarchy(Object ** objects, int object_count, float time0 = 0.0f, float time1 = 0.0f);
		virtual bool HitTest(const Ray & input_ray, float t_min, float t_max, HitPoint&hit_point);
		void BuildBoundingVolume();
		void SetUserData(Object*user_data);
		static int SortObjectByX(const void * ptr1, const void * ptr2);
		static int SortObjectByY(const void * ptr1, const void * ptr2);
		static int SortObjectByZ(const void * ptr1, const void * ptr2);
	};
}