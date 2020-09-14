#pragma once
#include "AliceVector3.h"
#include "AliceRay.h"
#include "LinkedList.h"
namespace Alice {
	class Camera {
		static Camera *mSelf;
	public:
		static Camera *Singleton();
		void Set(float fov, float aspect, float aperture);
		void LookAt(const Vector3 & pos, const Vector3 & center, const Vector3 & up);
		Ray GetRay(float u, float v);
		float mFOV, mAspect,mLensRadius;
		Vector3 mPosition, mCenter, mUp;
		Vector3 mViewPlaneLeftBottom,mHorizontalSpan,mVerticalSpan,mHorizontalDirection,mVerticalDirection;
	};
}