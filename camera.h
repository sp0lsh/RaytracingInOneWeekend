#pragma once

#include "ray.h"

class camera {
	public: 
		camera(vec3 lookFrom, vec3 lookAt, vec3 up, float vfov, float aspect) { // vfov: top-bottom, degrees
			
			float theta = vfov * M_PI / 180.0f;
			float halfHeight = tan(theta / 2.0f);
			float halfWidth = aspect * halfHeight;

			vec3 u, v, w;
			origin = lookFrom;
			w = normalize(lookFrom - lookAt);
			u = normalize(cross(up, w));
			v = cross(w, u);

			lowerLeftCorner = vec3(-halfWidth, -halfHeight, -1.0f);
			lowerLeftCorner = origin - halfWidth * u - halfHeight * v - w;
			horizontal = 2.0f * halfWidth * u;
			vertical =  2.0f * halfHeight * v;
		}

		ray getRay(float u, float v) { return ray(origin, lowerLeftCorner + u*horizontal + v*vertical - origin); }

		vec3 origin;
		vec3 lowerLeftCorner;
		vec3 horizontal;
		vec3 vertical;
};