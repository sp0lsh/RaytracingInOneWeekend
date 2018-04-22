// PathTracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <stdint.h>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <random>

#include "vec3.h"
#include "math.h"
#include "material.h"

hitable* sceneRandom() {
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

hitable* sceneBook() {
	int n = 5;
	hitable** list = new hitable*[n];
	list[0] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f,	new lambertian(vec3(0.8f, 0.8f, 0.0f)));
	list[1] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f,			new lambertian(vec3(0.8f, 0.3f, 0.3f)));
	list[2] = new sphere(vec3(1.0f, 0.0f, -1.0f), 0.5f,			new metal(vec3(0.8f, 0.6f, 0.2f), 0.3f));
	list[3] = new sphere(vec3(-1.0f, 0.0f, -1.0f), 0.5f,		new dialectric(1.5f));
	list[4] = new sphere(vec3(-1.0f, 0.0f, -1.0f), -0.45f,		new dialectric(1.5f));
	return new hitableList(list, n);
}

hitable* sceneSimple() {
	int n = 7;
	hitable** list = new hitable*[n];
	list[0] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f,	new lambertian(vec3(0.8f, 0.8f, 0.0f)));
	list[1] = new sphere(vec3(0.0f, 0.0f, -1.0f),	0.5f,		new lambertian(vec3(0.8f, 0.3f, 0.3f)));
	list[2] = new sphere(vec3(1.0f, 0.0f, -1.0f),	0.5f,		new metal(vec3(0.8f, 0.6f, 0.2f), 0.3f));
	list[3] = new sphere(vec3(-1.0f, 0.0f, -1.0f),	0.5f,		new dialectric(1.5f));
	list[4] = new sphere(vec3(-1.0f, 0.0f, -1.0f), -0.45f,		new dialectric(1.5f));
	list[5] = new sphere(vec3(-2.0f, 0.0f, -1.0f),	0.5f,		new metal(vec3(0.8f, 0.6f, 0.2f), 0.0f));
	list[6] = new sphere(vec3(2.0f, 0.0f, -1.0f),	0.5f,		new metal(vec3(0.8f, 0.6f, 0.2f), 0.5f));
	return new hitableList(list, n);
}

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
		float k = 0.5f * (unitDir.y + 1.0f);
		return lerp(vec3(1.0f, 1.0f, 1.0f), vec3(0.5f, 0.7f, 1.0f), k);
	}
}

void drawImage(int nx, int ny, int ns, std::ofstream &out)
{
	hitable *world = sceneSimple();

	float ratio = float(nx) / float(ny);
	vec3 lookFrom(3.0f, 1.0f, 0.0f);	// vec3 lookFrom(10.5f, 0.825f, 5.25f);
	vec3 lookAt(0.0f, 0.0f, 0.0f);		// vec3 lookAt(0.0f, 0.2f, -1.0f);
	float distToFocus = length(lookFrom - lookAt);
	float aperture = 0.05f;
	camera cam(lookFrom, lookAt, vec3(0.0f, 1.0f, 0.0f), 40.0f, ratio, aperture, distToFocus);

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
			col = vec3(sqrt(col.x), sqrt(col.y), sqrt(col.z));
			int ir = int(255.99*col.x);
			int ig = int(255.99*col.y);
			int ib = int(255.99*col.z);
			out << ir << " " << ig << " " << ib << "\n";
		}
	}
}

int main()
{
	std::ofstream out("./output/image.PPM");

	int nx = 200;	// 1280;
	int ny = 100;	// 760;
	int ns = 20;	// 100;
	out << "P3\n" << nx << " " << ny << "\n255\n";

	// profile
	LARGE_INTEGER time1;
	QueryPerformanceCounter(&time1);

	float t = float(clock()) / CLOCKS_PER_SEC;
	drawImage(nx, ny, ns, out);

	LARGE_INTEGER time2;
	QueryPerformanceCounter(&time2);
	uint64_t dt = time2.QuadPart - time1.QuadPart;
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	std::ofstream outProfile("./output/profile.txt");
	static char s_Buffer[200];
	double s = double(dt) / double(frequency.QuadPart);
	sprintf_s(s_Buffer, sizeof(s_Buffer), "ms: %.2f FPS %.1f\n", s * 1000.0f, 1.f / s);
	outProfile << s_Buffer;
	outProfile.close();

	// std::time_t result = std::time(nullptr);
	// std::cout << std::asctime(std::localtime(&result))
	// 	<< result << " seconds since the Epoch\n";

	out.close();

    return 0;
}