#pragma once

#include "ray.h"

class material;

struct hitRecord {
	float t;
	vec3 point;
	vec3 normal;
	material *mat;
};

class hitable {
	public:
		virtual bool hit(const ray &r, float tMin, float tMax, hitRecord &hit) const = 0;
};
