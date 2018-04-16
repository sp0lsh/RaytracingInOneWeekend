#include "stdafx.h"
#include <math.h>
#include "sphere.h"

bool sphere::hit(const ray & r, float tMin, float tMax, hitRecord & hit) const
{
	vec3 oc = r.origin() - center;
	float a = dot(r.dir(), r.dir());
	float b = dot(oc, r.dir());
	float c = dot(oc, oc) - radius * radius;
	float delta = b*b - a*c;
	if (delta > 0.0f) {
		float temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp < tMax && temp > tMin) {
			hit.t = temp;
			hit.point = r.pointAtT(hit.t);
			hit.normal = (hit.point - center) / radius;
			return true;
		}
		temp = (-b + sqrt(b*b - a * c)) / a;
		if (temp < tMax && temp > tMin) {
			hit.t = temp;
			hit.point = r.pointAtT(hit.t);
			hit.normal = (hit.point - center) / radius;
			return true;
		}
	}
	return false;
}
