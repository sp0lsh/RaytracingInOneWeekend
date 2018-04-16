#pragma once

#include "hitable.h"
#include "material.h"

class sphere : public hitable {
	public:
		sphere() {}
		sphere(vec3 cen, float r, material* m) : center(cen), radius(r), mat(m) {};
		virtual bool hit(const ray& r, float tMin, float tMax, hitRecord &hit) const;

		vec3  center;
		float radius;
		material* mat;
};
