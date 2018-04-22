#pragma once

#include "math.h"

class material {
	public:
		virtual bool scatter(const ray &in, const hitRecord &hit, vec3& attentuation, ray& scattered) const = 0;
};

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

class metal : public material {
public:
	metal(const vec3& a, float f) : albedo(a), fuzz(f) {}

	virtual bool scatter(const ray & in, const hitRecord & hit, vec3 & attentuation, ray & scattered) const override {
		vec3 reflected = reflect(normalize(in.dir()), hit.normal);
		scattered = ray(hit.point, reflected + fuzz * randomInUnitSphere());
		attentuation = albedo;
		return (dot(scattered.dir(), hit.normal) > 0.0f);
	};

	vec3 albedo;
	float fuzz;
};

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
		}
		else {
			outNormal = hit.normal;
			niOverNt = 1.0f / IOR;
			cosine = -dot(in.dir(), hit.normal) / length(in.dir());
		}

		vec3 refracted;
		float reflectProb;
		vec3 reflected = reflect(in.dir(), hit.normal);
		if (refract(in.dir(), outNormal, niOverNt, refracted)) {
			reflectProb = schlick(cosine, IOR);
		}
		else {
			reflectProb = 1.0f;
		}

		if (random() < reflectProb) {
			scattered = ray(hit.point, reflected);
		}
		else {
			scattered = ray(hit.point, refracted);
		}
		return true;
	}

	float IOR;

};