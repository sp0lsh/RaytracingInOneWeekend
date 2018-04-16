#include <math.h>
#include "stdafx.h"
#include "vec3.h"

// Random utils

float random()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

vec3 randomInUnitSphere() {
	vec3 p;
	do {
		p = 2.0f * vec3(random(), random(), random()) - vec3(1.0f, 1.0f, 1.0f);
	} while (dot(p, p) >= 1.0f);
	return p;
}

vec3 randomInUnitDisk() {
	vec3 p;
	do {
		p = 2.0f * vec3(random(), random(), 0.0f) - vec3(1.0f, 1.0f, 0.0f);
	} while (dot(p, p) >= 1.0f);
	return p;
}