#pragma once

#include "ray.h"

class camera {
public:
	camera(vec3 lookFrom, vec3 lookAt, vec3 up, float vfov, float aspect, float aperture, float focusDisk) { // vfov: top-bottom, degrees

		lensRadius = aperture / 2.0f;

		float theta = vfov * M_PI / 180.0f;
		float halfHeight = tan(theta / 2.0f);
		float halfWidth = aspect * halfHeight;
		
		origin = lookFrom;
		w = normalize(lookFrom - lookAt);
		u = normalize(cross(up, w));
		v = cross(w, u);
		
		lowerLeftCorner = origin - halfWidth * focusDisk * u - halfHeight * focusDisk * v - focusDisk * w;
		horizontal = 2.0f * halfWidth * focusDisk * u;
		vertical = 2.0f * halfHeight * focusDisk * v;
	}

	ray getRay(float s, float t) {
		vec3 rd = lensRadius * randomInUnitDisk();
		vec3  offset = u * rd.x() + v * rd.y();
		return ray(origin, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
	}

	vec3 origin;
	vec3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	float lensRadius;
};