#include "AliceVector3.h"
#include <math.h>
#include "utils.h"
namespace Alice {
	Vector3 Vector3::operator+(const Vector3&r) const {
		return Vector3(x + r.x, y + r.y, z + r.z);
	}
	Vector3 Vector3::operator-(const Vector3&r) const {
		return Vector3(x - r.x, y - r.y, z - r.z);
	}
	Vector3 Vector3::operator*(float scalar) const {
		return Vector3(x*scalar, y*scalar, z*scalar);
	}
	float Vector3::operator*(const Vector3&r) const {
		return x * r.x + y * r.y + z * r.z;
	}
	Vector3 Vector3::operator^(const Vector3&r) const {
		return Vector3(y*r.z - r.y*z, z*r.x - r.z*x, x*r.y - r.x*y);
	}
	float Vector3::magnitude() {
		return sqrtf(x*x + y * y + z * z);
	}
	void Vector3::normalize() {
		float len = magnitude();
		if (len > 0.000001f) {
			x /= len;
			y /= len;
			z /= len;
		}
		else {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}
	}
	Vector3 Vector3::ProjectTo(Vector3&v) {
		float lenSquared = v.LengthSquared();
		return (*this)*v*v* (1.0f/lenSquared);
	}
	Vector3 Vector3::PerpendicularTo(Vector3&v) {
		Vector3 projP2Q = ProjectTo(v);
		return (*this) - projP2Q;
	}
	float Vector3::LengthSquared() const {
		float len = x * x + y * y + z * z;
		return len != 0.0f ? len : 1.0f;
	}
	Vector3 operator*(float scalar, const Vector3&r) {
		return r * scalar;
	}
	Vector3 MultiplyComponentWise(const Vector3&v1, const Vector3&v2) {
		return Vector3(v1.x*v2.x,v1.y*v2.y,v1.z*v2.z);
	}
	Vector3 Reflect(const Vector3&v, const Vector3&n) {
		return v - 2.0f*(v*n)*n;
	}
	Vector3 random_in_unit_sphere() {
		Alice::Vector3 p;
		do {
			p = 2.0f*Alice::Vector3(randf(), randf(), randf()) - Alice::Vector3(1.0f, 1.0f, 1.0f);
		} while (p.LengthSquared() >= 1.0f);
		return p;
	}
	Vector3 random_in_unit_disk() {
		Alice::Vector3 p;
		do {
			p = 2.0f * Alice::Vector3(randf(), randf(), 0.0f) - Alice::Vector3(1.0f, 1.0f, 0.0f);
		} while (p.LengthSquared() >= 1.0f);
		return p;
	}
	bool Refract(const Vector3 &v, const Vector3&n, float ni_over_nt, Vector3&refracted) {
		float dt = v * n;
		float discriminant = 1.0f - ni_over_nt * ni_over_nt*(1.0f-dt*dt);
		if (discriminant>0.0f){
			refracted = ni_over_nt * (v - n * dt) - n * sqrtf(discriminant);
			return true;
		}
		else {
			return false;
		}
	}
}