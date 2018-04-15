// PathTracer00.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include <fstream>
#include <string>

#include "vec3.h"
#include "ray.h"

bool hitSphere(const vec3& center, const float radius, const ray& r)
{
	vec3 oc = r.origin() - center;
	float a = dot(r.dir(), r.dir());
	float b = 2.0f * dot(oc, r.dir());
	float c = dot(oc, oc) - radius * radius;
	float delta = b * b - 4.0f *a*c;
	return delta > 0.0f;
}


vec3 color(const ray& r) {
	if (hitSphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, r)) 
	{
		return vec3(1.0f, 0.0f, 0.0f);
	}

	vec3 unitDir = normalize(r.dir());
	float t = 0.5f * (unitDir.y() + 1.0f);
	return lerp(vec3(1.0f, 1.0f, 1.0f), vec3(0.5f, 0.7f, 1.0f), t);
}

int main()
{
	std::ofstream out("output.PPM");

	int nx = 200;
	int ny = 100;
	out << "P3\n" << nx << " " << ny << "\n255\n";

	vec3 lowerleftCorner(-2.0f, -1.0f, -1.0f);
	vec3 horizontal(4.0f, 0.0f, 0.0f);
	vec3 vertical(0.0f, 2.0f, 0.0f);
	vec3 origin(0.0f, 0.0f, 0.0f);

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			ray r(origin, lowerleftCorner + u * horizontal + v * vertical);

			vec3 col = color(r);
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			out << ir << " " << ig << " " << ib << "\n";
		}
	}

	out.close();

    return 0;
}

