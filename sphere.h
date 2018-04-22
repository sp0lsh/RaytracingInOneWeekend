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

bool sphere::hit(const ray & r, float tMin, float tMax, hitRecord & hit) const
{
	vec3 oc = r.origin() - center;
	float a = dot(r.dir(), r.dir());
	float b = dot(oc, r.dir());
	float c = dot(oc, oc) - radius * radius;
	float delta = b * b - a * c;
	if (delta > 0.0f) {
		float temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp < tMax && temp > tMin) {
			hit.t = temp;
			hit.point = r.pointAtT(hit.t);
			hit.normal = (hit.point - center) / radius;
			hit.mat = mat;
			return true;
		}
		temp = (-b + sqrt(b*b - a * c)) / a;
		if (temp < tMax && temp > tMin) {
			hit.t = temp;
			hit.point = r.pointAtT(hit.t);
			hit.normal = (hit.point - center) / radius;
			hit.mat = mat;
			return true;
		}
	}
	return false;
}

