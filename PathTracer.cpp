// PathTracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <random>

#define MAXFLOAT 100000.0f
#define M_PI 3.1415f

#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hitableList.h"
#include "camera.h"
#include "material.h"
#include "lambertian.h"
#include "metal.h"
#include "dialectric.h"


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

hitable* randomScene() {
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new lambertian(vec3(0.5f, 0.5f, 0.5f)));

	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float chooseMat = random();
			vec3 center(a + 0.9f*random(), 0.2f, b + 0.9f*random());
			if (length(center - vec3(4.0f, 0.2f, 0.0f)) > 0.9f) {
				if (chooseMat < 0.8f) { // diffuse
					list[i++] = new sphere(center, 0.2f, new lambertian(vec3(random()*random(), random()*random(), random()*random())));

				} else if (chooseMat < 0.95f) { // metal
					list[i++] = new sphere(center, 0.2f, new metal(vec3(0.5f * (1.0f + random()), 0.5f * (1.0f + random()), 0.5f * (1.0f + random())), 0.5f*random() ));

				} else { // glass
					list[i++] = new sphere(center, 0.2f, new dialectric(lerp(1.2f, 1.7f, random())));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0.0f, 1.0f, 0.0f), 1.0f, new dialectric(1.5f));
	list[i++] = new sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, new lambertian(vec3(0.4f, 0.2f, 0.1f)));
	list[i++] = new sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0f));

	return new hitableList(list, i);
}


int main()
{
	std::ofstream out("output.PPM");

	int nx = 1280;
	int ny = 760;
	int ns = 100;
	out << "P3\n" << nx << " " << ny << "\n255\n";

	vec3 lowerleftCorner(-2.0f, -1.0f, -1.0f);
	vec3 horizontal(4.0f, 0.0f, 0.0f);
	vec3 vertical(0.0f, 2.0f, 0.0f);
	vec3 origin(0.0f, 0.0f, 0.0f);

	hitable *world = randomScene();
	
	float ratio = float(nx) / float(ny);
	vec3 lookFrom(7.0f, 0.55f, 3.5f);
	lookFrom *= 1.5f;
	vec3 lookAt(0.0f, 0.2f, -1.0f);
	float distToFocus = length(lookFrom - lookAt);
	float aperture = 0.05f;
	camera cam(lookFrom, lookAt, vec3(0.0f, 1.0f, 0.0f), 20.0f, ratio, aperture, distToFocus);

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