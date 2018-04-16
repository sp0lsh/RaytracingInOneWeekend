#pragma once

#include "material.h"

class lambertian : public material {
	public:
		lambertian(const vec3& a) : albedo(a) {}

		virtual bool scatter(const ray & in, const hitRecord& hit, vec3& attentuation, ray& scattered) const override {
			vec3 target = hit.point + hit.normal + randomInUnitSphere();
			scattered = ray(hit.point, target - hit.point);
			attentuation = albedo;
			return true;
		};

		vec3 albedo;
};