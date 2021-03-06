#pragma once
#include "AliceVector3.h"
#include "AliceRay.h"
#include "LinkedList.h"
#include "Texture.h"
namespace Alice {
	class Material {
	public:
		virtual bool Scatter(const Ray & input_ray, Ray & out_ray,const HitPoint &point) const = 0;
		Vector3 GetColor(const Vector3 & input_color, const Vector3 & attenuation) const;
		virtual Vector3 Emitted(float u, float v, const Vector3 & p) const {
			return Vector3(0.0f,0.0f,0.0f);
		}
	};
	class LambertMaterial :public Material {
	public:
		Texture *mAlbedo;
		LambertMaterial(Texture*albedo) :mAlbedo(albedo) {}
		virtual bool Scatter(const Ray & input_ray, Ray & out_ray, const HitPoint &point) const;
	};
	class MetalMaterial :public Material {
	public:
		Vector3 mAttenuation;
		float mFuzzy;
		MetalMaterial(Vector3 &attenuation,float f) :mAttenuation(attenuation) {
			mFuzzy = f < 1.0f ? f : 1.0f;
		}
		virtual bool Scatter(const Ray & input_ray, Ray & out_ray, const HitPoint &point)  const;
	};
	class DielectricMaterial :public Material {
	public:
		float mRefractIndex;
		DielectricMaterial(float ri) :mRefractIndex(ri) {
		}
		virtual bool Scatter(const Ray & input_ray, Ray & out_ray, const HitPoint &point)  const;
		static float Schlick(float cosine, float reflect_index);
	};
	class DiffuseLightSourceMaterial :public Material {
	public:
		Texture *mAlbedo;
		DiffuseLightSourceMaterial(Texture*albedo) :mAlbedo(albedo) {}
		virtual bool Scatter(const Ray & input_ray, Ray & out_ray, const HitPoint &point) const {
			return false;
		}
		virtual Vector3 Emitted(float u, float v, const Vector3 & p) const {
			return mAlbedo->Sample(u, v, p);
		}
	};
}