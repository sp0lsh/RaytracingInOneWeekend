#include "stdafx.h"
#include <math.h>

#include "vec3.h"

inline void vec3::normalize() {
	float k = 1.0 / sqrtf(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
}

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
	return vec3(v.e[0] / t, v.e[1] / t, v.e[2] /t);
}

inline float dot(const vec3 &vl, const vec3 &vr) {
	return vl.e[0] * vr.e[0] + vl.e[1] * vr.e[1] + vl.e[2] * vr.e[2];
}

inline vec3 cross(const vec3 &vl, const vec3 &vr) {
	return vec3(   (vl.e[1] * vr.e[2] - vl.e[2] * vr.e[1]),
				 (-(vl.e[0] * vr.e[2] - vl.e[2] * vr.e[0])),
				   (vl.e[0] * vr.e[1] - vl.e[1] * vr.e[0])
			);
}
