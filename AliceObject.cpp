#include "AliceObject.h"
#include "AliceTree.h"
#include "AliceMaterial.h"
namespace Alice{
	Object::Object():mGeometry(nullptr),mMaterial(nullptr) {

	}
	Object::Object(Geometry*geometry, Material*material) : mGeometry(geometry), mMaterial(material) {

	}
	Object::~Object() {
		if (mGeometry!=nullptr){
			delete mGeometry;
		}
		if (mMaterial!=nullptr){
			delete mMaterial;
		}
	}
	void Object::Set(Geometry*geometry, Material*material) {
		mGeometry = geometry;
		mMaterial = material;
	}
	void Object::SetGeometry(Geometry*geometry) {
		mGeometry = geometry;
	}
	void Object::SetMaterial(Material*material) {
		mMaterial = material;
	}
	void Object::SetName(const char * name) {
		memset(mName, 0, 64);
		strcpy(mName, name);
	}
	void Object::SetPosition(float x, float y, float z) {
		mWorldTransform.mData[12] = x;
		mWorldTransform.mData[13] = y;
		mWorldTransform.mData[14] = z;
	}
	void Object::SetPosition(const Vector3 & position) {
		mWorldTransform.mData[12] = position.x;
		mWorldTransform.mData[13] = position.y;
		mWorldTransform.mData[14] = position.z;
	}
	bool Object::HitTest(const Ray & input_ray, float t_min, float t_max, HitPoint & hit_point) const {
		if (mGeometry->HitTest(input_ray,t_min,t_max,hit_point)){
			hit_point.mMaterial = mMaterial;
			return true;
		}
		return false;
	}
}