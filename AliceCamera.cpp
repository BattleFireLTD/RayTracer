#include "AliceCamera.h"
namespace Alice {
	Camera*Camera::mSelf = nullptr;
	Camera*Camera::Singleton() {
		if (mSelf==nullptr){
			mSelf = new Camera;
		}
		return mSelf;
	}
	void Camera::Set(float fov, float aspect, float aperture) {
		mFOV = fov;
		mAspect = aspect;
		mLensRadius = aperture / 2.0f;
	}
	void Camera::LookAt(const Vector3 & pos, const Vector3 & center, const Vector3 & up) {
		Vector3 w = pos - center;
		float distance = w.magnitude();
		float theta = mFOV * 3.1415926f / 180.0f;
		float half_height = tanf(theta/2.0f) * distance;
		float half_width = mAspect * half_height;
		mPosition = pos;
		Vector3 u = up ^ w;
		Vector3 v = w ^ u;
		u.normalize();
		v.normalize();
		mHorizontalDirection = u;
		mVerticalDirection = v;
		mViewPlaneLeftBottom = center - half_width * u - half_height * v ;
		mHorizontalSpan = 2.0f * half_width * u;
		mVerticalSpan = 2.0f * half_height * v;
	}
	Ray Camera::GetRay(float u, float v) {
		Vector3 rd = mLensRadius * random_in_unit_disk();
		Vector3 offset = mHorizontalDirection * rd.x + mVerticalDirection * rd.y;
		return Ray(mPosition,mViewPlaneLeftBottom+u*mHorizontalSpan +v*mVerticalSpan - mPosition - offset*0.01f,Vector3(1.0f,1.0f,1.0f));
	}
}