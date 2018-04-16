#pragma once

#include "ray.h"
#include "hitable.h"

class material {
	public:
		virtual bool scatter(const ray &in, const hitRecord &hit, vec3& attentuation, ray& scattered) const = 0;
};