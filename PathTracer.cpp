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
#include "material.h"
#include "lambertian.h"
#include "metal.h"
#include "dialectric.h"

#define MAXFLOAT 100000.0f

vec3 color(const ray& r, hitable *world, int depth) {
	hitRecord hit;
	if (world->hit(r, 0.0001f, MAXFLOAT, hit)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && hit.mat->scatter(r, hit, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			vec3(0.0f, 0.0f, 0.0f);
		}
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

	int nx = 800;
	int ny = 400;
	int ns = 100;
	out << "P3\n" << nx << " " << ny << "\n255\n";

	vec3 lowerleftCorner(-2.0f, -1.0f, -1.0f);
	vec3 horizontal(4.0f, 0.0f, 0.0f);
	vec3 vertical(0.0f, 2.0f, 0.0f);
	vec3 origin(0.0f, 0.0f, 0.0f);

	hitable *list[5];
	list[0] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, new lambertian(vec3(0.8f, 0.3f, 0.3f)));
	list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f, new lambertian(vec3(0.8f, 0.8f, 0.0f)));
	list[2] = new sphere(vec3(1.0f, 0.0f, -1.0f), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f), 0.3f));
	list[3] = new sphere(vec3(-1.0f, 0.0f, -1.0f), 0.5f, new dialectric(1.5f));
	list[4] = new sphere(vec3(-1.0f, 0.0f, -1.0f), -0.45f, new dialectric(1.5f));
	hitable *world = new hitableList(list, 5);
	
	camera cam;

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < ns; s++) {
				float u = float(i + random()) / float(nx);
				float v = float(j + random()) / float(ny);
				ray r = cam.getRay(u, v);
				col += color(r, world, 0);
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