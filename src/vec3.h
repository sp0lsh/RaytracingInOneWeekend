#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3 {
	public:
		vec3(){}
		vec3(float x, float y, float z) : x(x), y(y), z(z) {};

		inline const vec3& operator+() const { return *this; }
		inline vec3 operator-() const { return vec3(-x, -y, -z); }

		inline vec3& operator+=(const vec3 &vr);
		inline vec3& operator-=(const vec3 &vr);
		inline vec3& operator*=(const vec3 &vr);
		inline vec3& operator/=(const vec3 &vr);
		inline vec3& operator*=(const float t);
		inline vec3& operator/=(const float t);

		float x, y, z;
};

inline vec3& vec3::operator+=(const vec3 &v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

inline vec3& vec3::operator*=(const vec3 &v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

inline vec3& vec3::operator/=(const vec3 &v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

inline vec3& vec3::operator-=(const vec3 &v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

inline vec3& vec3::operator*=(const float t) {
	x *= t;
	y *= t;
	z *= t;
	return *this;
}

inline vec3& vec3::operator/=(const float t) {
	float k = 1.0f / t;
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

// Non-member operators

inline std::istream& operator>>(std::istream &is, vec3 &t) {
	is >> t.x >> t.y >> t.z;
	return is;
}

inline std::ostream& operator>>(std::ostream &os, vec3 &t) {
	os << t.x << t.y << t.z;
	return os;
}

inline vec3 operator+(const vec3 &vl, const vec3 &vr) {
	return vec3(vl.x + vr.x, vl.y + vr.y, vl.z + vr.z);
}

inline vec3 operator-(const vec3 &vl, const vec3 &vr) {
	return vec3(vl.x - vr.x, vl.y - vr.y, vl.z - vr.z);
}

inline vec3 operator*(const vec3 &vl, const vec3 &vr) {
	return vec3(vl.x * vr.x, vl.y * vr.y, vl.z * vr.z);
}

inline vec3 operator/(const vec3 &vl, const vec3 &vr) {
	return vec3(vl.x / vr.x, vl.y / vr.y, vl.z / vr.z);
}

inline vec3 operator*(float t, const vec3 &v) {
	return vec3(t * v.x, t * v.y, t * v.z);
}

inline vec3 operator*(const vec3 &v, float t) {
	return vec3(t * v.x, t * v.y, t * v.z);
}

inline vec3 operator/(vec3 v, float t) {
	return vec3(v.x / t, v.y / t, v.z / t);
}

// Math

inline float length(const vec3 &v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline vec3 normalize(const vec3 &v)
{
	return v / length(v);
}

inline float dot(const vec3 &vl, const vec3 &vr) {
	return vl.x * vr.x + vl.y * vr.y + vl.z * vr.z;
}

inline vec3 cross(const vec3 &vl, const vec3 &vr) {
	return vec3((vl.y * vr.z - vl.z * vr.y),
		(-(vl.x * vr.z - vl.z * vr.x)),
		(vl.x * vr.y - vl.y * vr.x)
	);
}

inline float lerp(const float a, const float b, const float t)
{
	return (1.0f - t)*a + t * b;
}

inline vec3 lerp(const vec3 & a, const vec3 & b, const float t)
{
	return (1.0f - t)*a + t * b;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2.0f * dot(v, n) * n;
}

inline bool refract(const vec3& v, const vec3& n, float niOverNt, vec3& refracted) {
	vec3 uv = normalize(v);
	float dt = dot(uv, n);
	float delta = 1.0f - niOverNt * niOverNt * (1.f - dt * dt);
	if (delta > 0.0f) {
		refracted = niOverNt * (uv - n * dt) - n * sqrt(delta);
		return true;
	}
	else
	{
		return false;
	}
}