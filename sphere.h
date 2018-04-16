#pragma once

#include "hitable.h"

class sphere : public hitable {
	public:
		sphere() {}
		sphere(vec3 cen, float r) : center(cen), radius(r) {};
		virtual bool hit(const ray& r, float tMin, float tMax, hitRecord &hit) const;

		vec3  center;
		float radius;
};
