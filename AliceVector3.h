#pragma once
#include <math.h>
#include <algorithm>
namespace Alice {
	class Vector3 {
	public:
		union
		{
			struct
			{
				float x, y, z;
			};
			float v[3];
		};
		
	public:
		Vector3() :x(0), y(0), z(0) {}
		Vector3(float x, float y, float z) :x(x), y(y), z(z) {}
		Vector3 operator+(const Vector3&r) const;
		Vector3 operator-(const Vector3&r) const;
		Vector3 operator*(float scalar) const;
		float operator*(const Vector3&r) const;
		Vector3 operator^(const Vector3&r) const;
		float operator[](int index) const { return v[index]; };
		float magnitude();
		void normalize();
		Vector3 ProjectTo(Vector3&v);
		Vector3 PerpendicularTo(Vector3&v);
		float LengthSquared() const;
	};
	Vector3 operator*(float scalar, const Vector3&v);
	Vector3 MultiplyComponentWise(const Vector3&v1, const Vector3&v2);
	Vector3 Reflect(const Vector3&v, const Vector3&n);
	bool Refract(const Vector3 &v, const Vector3&n, const float ri, Vector3&refracted);
	Vector3 random_in_unit_sphere();
	Vector3 random_in_unit_disk();
}