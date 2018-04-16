#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3 {
	public:
		vec3(){}
		vec3(float x, float y, float z) { e[0] = x; e[1] = y, e[2] = z; }
		inline float x() const { return e[0]; }
		inline float y() const { return e[1]; }
		inline float z() const { return e[2]; }
		inline float r() const { return e[0]; }
		inline float g() const { return e[1]; }
		inline float b() const { return e[2]; }

		inline const vec3& operator+() const { return *this; }
		inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
		inline float operator[](int i) const { return e[i]; }
		inline float& operator[](int i) { return e[i]; }

		inline vec3& operator+=(const vec3 &vr);
		inline vec3& operator-=(const vec3 &vr);
		inline vec3& operator*=(const vec3 &vr);
		inline vec3& operator/=(const vec3 &vr);
		inline vec3& operator*=(const float t);
		inline vec3& operator/=(const float t);

		float e[3];
};


inline vec3& vec3::operator+=(const vec3 &v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline vec3& vec3::operator*=(const vec3 &v) {
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline vec3& vec3::operator/=(const vec3 &v) {
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline vec3& vec3::operator-=(const vec3 &v) {
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

inline vec3& vec3::operator*=(const float t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline vec3& vec3::operator/=(const float t) {
	float k = 1.0f / t;
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}

// Non-member operators

inline std::istream& operator>>(std::istream &is, vec3 &t) {
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}

inline std::ostream& operator>>(std::ostream &os, vec3 &t) {
	os << t.e[0] << t.e[1] << t.e[2];
	return os;
}

inline vec3 operator+(const vec3 &vl, const vec3 &vr) {
	return vec3(vl.e[0] + vr.e[0], vl.e[1] + vr.e[1], vl.e[2] + vr.e[2]);
}

inline vec3 operator-(const vec3 &vl, const vec3 &vr) {
	return vec3(vl.e[0] - vr.e[0], vl.e[1] - vr.e[1], vl.e[2] - vr.e[2]);
}

inline vec3 operator*(const vec3 &vl, const vec3 &vr) {
	return vec3(vl.e[0] * vr.e[0], vl.e[1] * vr.e[1], vl.e[2] * vr.e[2]);
}

inline vec3 operator/(const vec3 &vl, const vec3 &vr) {
	return vec3(vl.e[0] / vr.e[0], vl.e[1] / vr.e[1], vl.e[2] / vr.e[2]);
}

inline vec3 operator*(float t, const vec3 &v) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, float t) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator/(vec3 v, float t) {
	return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

// Math

inline float length(const vec3 &v) {
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

inline vec3 normalize(const vec3 &v)
{
	return v / length(v);
}

inline float dot(const vec3 &vl, const vec3 &vr) {
	return vl.e[0] * vr.e[0] + vl.e[1] * vr.e[1] + vl.e[2] * vr.e[2];
}

inline vec3 cross(const vec3 &vl, const vec3 &vr) {
	return vec3((vl.e[1] * vr.e[2] - vl.e[2] * vr.e[1]),
		(-(vl.e[0] * vr.e[2] - vl.e[2] * vr.e[0])),
		(vl.e[0] * vr.e[1] - vl.e[1] * vr.e[0])
	);
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

// Random utils

float random();

vec3 randomInUnitSphere();

vec3 randomInUnitDisk();

inline float schlick(float cosine, float IOR) {
	float r0 = (1.0f-IOR) / (1.0f+IOR);
	r0 = r0 * r0;
	return r0 + (1.0f-r0) * pow((1.0f-cosine), 5.0f);
}