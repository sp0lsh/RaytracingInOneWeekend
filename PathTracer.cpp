// PathTracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <random>

#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hitableList.h"
#include "camera.h"

#define MAXFLOAT 100000.0f

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

vec3 color(const ray& r, hitable *world) {
	hitRecord hit;
	if (world->hit(r, 0.0001f, MAXFLOAT, hit)) {
		vec3 target = hit.point + hit.normal + randomInUnitSphere();
		return 0.5f * color(ray(hit.point, target - hit.point), world);
	}
	else
	{
		vec3 unitDir = normalize(r.dir());
		float k = 0.5f * (unitDir.y() + 1.0f);
		return lerp(vec3(1.0f, 1.0f, 1.0f), vec3(0.5f, 0.7f, 1.0f), k);
	}
}

int main()
{
	std::ofstream out("output.PPM");

	int nx = 200;
	int ny = 100;
	int ns = 100;
	out << "P3\n" << nx << " " << ny << "\n255\n";

	vec3 lowerleftCorner(-2.0f, -1.0f, -1.0f);
	vec3 horizontal(4.0f, 0.0f, 0.0f);
	vec3 vertical(0.0f, 2.0f, 0.0f);
	vec3 origin(0.0f, 0.0f, 0.0f);

	hitable *list[2];
	list[0] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f);
	list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f);
	hitable *world = new hitableList(list, 2);
	
	camera cam;

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < ns; s++) {
				float u = float(i + random()) / float(nx);
				float v = float(j + random()) / float(ny);
				ray r = cam.getRay(u, v);
				col += color(r, world);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			out << ir << " " << ig << " " << ib << "\n";
		}
	}

	out.close();

    return 0;
}