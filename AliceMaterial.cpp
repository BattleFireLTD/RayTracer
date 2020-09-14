#include "AliceMaterial.h"
#include "utils.h"
namespace Alice {
	Vector3 Material::GetColor(const Vector3 & input_color, const Vector3 & attenuation) const {
		return Vector3(input_color.x*attenuation.x, input_color.y*attenuation.y, input_color.z*attenuation.z);
	}
	bool LambertMaterial::Scatter(const Ray & input_ray, Ray & out_ray, const HitPoint &point) const {
		Alice::Vector3 random_vec = random_in_unit_sphere();
		Alice::Vector3 new_ray_target = point.mPosition + point.mNormal + random_vec;
		Alice::Vector3 new_ray_direction = new_ray_target - point.mPosition;
		out_ray.Set(point.mPosition, new_ray_direction, GetColor(input_ray.mLightingAttenuation,mAttenuation));
		return true;
	}
	bool MetalMaterial::Scatter(const Ray & input_ray, Ray & out_ray, const HitPoint &point)  const {
		Vector3 reflect = Reflect(input_ray.mDirection, point.mNormal);
		out_ray.Set(point.mPosition, reflect + mFuzzy * random_in_unit_sphere(),GetColor(input_ray.mLightingAttenuation,mAttenuation));
		return (reflect*point.mNormal) > 0.0f;
	}
	bool DielectricMaterial::Scatter(const Ray & input_ray, Ray & out_ray, const HitPoint &point)  const {
		Vector3 attenuation = Vector3(1.0f, 1.0f, 1.0f);
		Vector3 outward_normal;
		float ni_over_nt = 0.0f;
		float cosine;
		if (input_ray.mDirection*point.mNormal > 0.0f) {
			outward_normal = Vector3(-point.mNormal.x, -point.mNormal.y, -point.mNormal.z);
			ni_over_nt = mRefractIndex;
			cosine = mRefractIndex * input_ray.mDirection*point.mNormal;
		}
		else {
			outward_normal = point.mNormal;
			ni_over_nt = 1.0f / mRefractIndex;
			cosine = -(input_ray.mDirection*point.mNormal);
		}
		Vector3 refract;
		float reflect_prob = 0.0f;
		if (Refract(input_ray.mDirection, outward_normal, ni_over_nt, refract)) {
			reflect_prob = Schlick(cosine, mRefractIndex);
		}
		else {
			reflect_prob = 1.0f;
		}
		if (randf()<reflect_prob){
			out_ray.Set(point.mPosition, Reflect(input_ray.mDirection, point.mNormal), GetColor(input_ray.mLightingAttenuation, attenuation));
		}
		else {
			out_ray.Set(point.mPosition, refract, GetColor(input_ray.mLightingAttenuation, attenuation));
		}
		return true;
	}
	float DielectricMaterial::Schlick(float cosine, float reflect_index) {
		float r0 = (1.0f - reflect_index) / (1.0f + reflect_index);
		r0 = r0 * r0;
		return r0 + (1.0f - r0)*powf((1.0f-cosine),5);
	}
}