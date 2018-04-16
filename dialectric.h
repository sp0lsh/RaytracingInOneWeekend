#pragma once

#include "material.h"
#include "ray.h"

class dialectric : public material {
	public:
		dialectric(float ior) : IOR(ior) {}

		virtual bool scatter(const ray& in, const hitRecord & hit, vec3 & attentuation, ray & scattered) const override
		{
			attentuation = vec3(1.0f, 1.0f, 1.0f);

			vec3 outNormal;
			float niOverNt;
			float cosine;
			if (dot(in.dir(), hit.normal) > 0.0f) {
				outNormal = -hit.normal;
				niOverNt = IOR;
				cosine = IOR * dot(in.dir(), hit.normal) / length(in.dir());
			} else {
				outNormal = hit.normal;
				niOverNt = 1.0f / IOR;
				cosine = -dot(in.dir(), hit.normal) / length(in.dir());
			}
			
			vec3 refracted;
			float reflectProb;
			vec3 reflected = reflect(in.dir(), hit.normal);
			if (refract(in.dir(), outNormal, niOverNt, refracted)) {
				reflectProb = schlick(cosine, IOR);
			} else {
				reflectProb = 1.0f;
			}

			if (random() < reflectProb) {
				scattered = ray(hit.point, reflected);
			} else {
				scattered = ray(hit.point, refracted);
			}
			return true;
		}

		float IOR;

};