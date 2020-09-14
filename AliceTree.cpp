#include "AliceTree.h"
#include "utils.h"
#include "AliceObject.h"
#include "AliceAABB.h"
#pragma warning(disable:4018)
namespace Alice{
	bool BVHNode::BuildBoundingVolumeHierarchy(Object ** objects, int object_count, float time0 /* = 0.0f */, float time1 /* = 0.0f */) {
		int axis = int(floorf(randf()*3.0f));
		SortFunction foo[] = { SortObjectByX,SortObjectByY,SortObjectByZ };
		qsort(objects, object_count, sizeof(Object*), foo[axis]);
		if (object_count==1){
			mLeftChild = new BVHNode;
			mRightChild = new BVHNode;
			mLeftChild->SetUserData(objects[0]);
			mRightChild->SetUserData(objects[0]);
		}
		else if (object_count == 2) {
			mLeftChild = new BVHNode;
			mRightChild = new BVHNode;
			mLeftChild->SetUserData(objects[0]);
			mRightChild->SetUserData(objects[1]);
		}
		else {
			mLeftChild = new BVHNode;
			mRightChild = new BVHNode;
			mLeftChild->BuildBoundingVolumeHierarchy(objects, object_count / 2, time0, time1);
			mRightChild->BuildBoundingVolumeHierarchy(objects + object_count/2, object_count - object_count / 2, time0, time1);
		}
		BuildBoundingVolume();
		return true;
	}
	bool BVHNode::HitTest(const Ray & input_ray, float t_min, float t_max, HitPoint&hit_point) {
		if (mBoundingVolume->HitTest(input_ray, t_min, t_max)){
			if (mUserData!=nullptr){
				bool hitted = mUserData->mGeometry->HitTest(input_ray, t_min, t_max, hit_point);
				hit_point.mMaterial = mUserData->mMaterial;
				return hitted;
			}
			HitPoint hit_point_left, hit_point_right;
			bool hit_left = mLeftChild->HitTest(input_ray, t_min, t_max, hit_point_left);
			bool hit_right = mRightChild->HitTest(input_ray, t_min, t_max, hit_point_right);
			if (hit_left&&hit_right){
				if (hit_point_left.mTValue<hit_point_right.mTValue){
					hit_point = hit_point_left;
				}
				else {
					hit_point = hit_point_right;
				}
				return true;
			}else if (hit_left){
				hit_point = hit_point_left;
				return true;
			}else if (hit_right){
				hit_point = hit_point_right;
				return true;
			}
		}
		return false;
	}
	void BVHNode::BuildBoundingVolume() {
		if (mLeftChild!=nullptr&&mRightChild!=nullptr){
			if (mLeftChild->mBoundingVolume->mType==BoundingVolume::kBoundingVolumeTypeAABB&&mRightChild->mBoundingVolume->mType==BoundingVolume::kBoundingVolumeTypeAABB){
				AABB*aabb_left = (AABB*)mLeftChild->mBoundingVolume;
				AABB*aabb_right = (AABB*)mRightChild->mBoundingVolume;
				AABB*aabb = new AABB(Vector3(
					min_of(aabb_left->mMin.x,aabb_right->mMin.x),
					min_of(aabb_left->mMin.y, aabb_right->mMin.y),
					min_of(aabb_left->mMin.z, aabb_right->mMin.z)
				), Vector3(
					max_of(aabb_left->mMax.x, aabb_right->mMax.x),
					max_of(aabb_left->mMax.y, aabb_right->mMax.y),
					max_of(aabb_left->mMax.z, aabb_right->mMax.z)
				));
				mBoundingVolume = aabb;
			}
		}else if (mLeftChild!=nullptr){
			mBoundingVolume = mLeftChild->mBoundingVolume->Clone();
		}else if (mRightChild!=nullptr){
			mBoundingVolume = mRightChild->mBoundingVolume->Clone();
		}
	}
	void BVHNode::SetUserData(Object*user_data) {
		mUserData = user_data;
		mUserData->mGeometry->BuildBoundingVolume(&mBoundingVolume);
	}
	int BVHNode::SortObjectByX(const void * ptr1, const void * ptr2) {
		Object*left = *(Object**)ptr1;
		Object*right = *(Object**)ptr2;
		BoundingVolume *bv_left, *bv_right;
		if (!left->mGeometry->BuildBoundingVolume(&bv_left)||!right->mGeometry->BuildBoundingVolume(&bv_right)){
			printf("[error] build bounding volume failed!");
		}
		if (bv_left->mType==BoundingVolume::kBoundingVolumeTypeAABB&&bv_right->mType==BoundingVolume::kBoundingVolumeTypeAABB){
			AABB*aabb_left = (AABB*)bv_left;
			AABB*aabb_right = (AABB*)bv_right;
			int ret = aabb_left->mMin.x < aabb_right->mMin.x ? -1 : 1;
			delete bv_right;
			delete bv_left;
			return ret;
		}
		return -1;
	}
	int BVHNode::SortObjectByY(const void * ptr1, const void * ptr2) {
		Object*left = *(Object**)ptr1;
		Object*right = *(Object**)ptr2;
		BoundingVolume *bv_left, *bv_right;
		if (!left->mGeometry->BuildBoundingVolume(&bv_left) || !right->mGeometry->BuildBoundingVolume(&bv_right)) {
			printf("[error] build bounding volume failed!");
		}
		if (bv_left->mType == BoundingVolume::kBoundingVolumeTypeAABB&&bv_right->mType == BoundingVolume::kBoundingVolumeTypeAABB) {
			AABB*aabb_left = (AABB*)bv_left;
			AABB*aabb_right = (AABB*)bv_right;
			int ret = aabb_left->mMin.y < aabb_right->mMin.y ? -1 : 1;
			delete bv_right;
			delete bv_left;
			return ret;
		}
		return -1;
	}
	int BVHNode::SortObjectByZ(const void * ptr1, const void * ptr2) {
		Object*left = *(Object**)ptr1;
		Object*right = *(Object**)ptr2;
		BoundingVolume *bv_left, *bv_right;
		if (!left->mGeometry->BuildBoundingVolume(&bv_left) || !right->mGeometry->BuildBoundingVolume(&bv_right)) {
			printf("[error] build bounding volume failed!");
		}
		if (bv_left->mType == BoundingVolume::kBoundingVolumeTypeAABB&&bv_right->mType == BoundingVolume::kBoundingVolumeTypeAABB) {
			AABB*aabb_left = (AABB*)bv_left;
			AABB*aabb_right = (AABB*)bv_right;
			int ret = aabb_left->mMin.z < aabb_right->mMin.z ? -1 : 1;
			delete bv_right;
			delete bv_left;
			return ret;
		}
		return -1;
	}
}