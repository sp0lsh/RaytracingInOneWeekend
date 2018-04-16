#pragma once

#include "material.h"
#include "vec3.h"

class metal : public material {
	public: 
		metal(const vec3& a) : albedo(a) {}
		
		virtual bool scatter(const ray & in, const hitRecord & hit, vec3 & attentuation, ray & scattered) const override {
			vec3 reflected = reflect(normalize(in.dir()), hit.normal);
			scattered = ray(hit.point, reflected);
			attentuation = albedo;
			return (dot(scattered.dir(), hit.normal) > 0.0f);
		};

		vec3 albedo;
};