
#define __STDC_WANT_LIB_EXT1__ 1

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

vec3 color(const ray& r, hitable *world, int depth, int& inOutRayCount)
{
	hitRecord hit;
	++inOutRayCount;

	if (world->hit(r, 0.0001f, MAXFLOAT, hit)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && hit.mat->scatter(r, hit, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1, inOutRayCount);
		}
		else {
			return vec3(0.0f, 0.0f, 0.0f);
		}
	}
	else
	{
		vec3 unitDir = normalize(r.dir());
		float k = 0.5f * (unitDir.y + 1.0f);
		return lerp(vec3(1.0f, 1.0f, 1.0f), vec3(0.5f, 0.7f, 1.0f), k);
	}
}

static int s_BackBufferSize;
static float* s_BackBuffer;

void drawImage(float* backBuffer, int nx, int ny, int ns, int& inOutRayCount)
{
	hitable *world = sceneSimple();

	float ratio = float(nx) / float(ny);
	vec3 lookFrom(0.0f, 2.0f, 3.0f);	// vec3 lookFrom(10.5f, 0.825f, 5.25f);
	vec3 lookAt(0.0f, 0.0f, 0.0f);		// vec3 lookAt(0.0f, 0.2f, -1.0f);
	float distToFocus = length(lookFrom - lookAt);
	float aperture = 0.05f;
	camera cam(lookFrom, lookAt, vec3(0.0f, 1.0f, 0.0f), 40.0f, ratio, aperture, distToFocus);

	int b = 0;
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < ns; s++) {
				float u = float(i + random()) / float(nx);
				float v = float(j + random()) / float(ny);
				ray r = cam.getRay(u, v);
				col += color(r, world, 0, inOutRayCount);
			}
			col /= float(ns);
			col = vec3(sqrt(col.x), sqrt(col.y), sqrt(col.z));

			backBuffer[b] = col.x;
			backBuffer[b + 1] = col.y;
			backBuffer[b + 2] = col.z;
			b += 3;
		}
	}
}

void WriteImageToDisk(std::string &timestamp, int nx, int ny)
{
	std::string imageFilename = std::string("./output/" + timestamp + "_image.PPM");
	std::ofstream outImage(imageFilename.c_str());
	outImage << "P3\n" << nx << " " << ny << "\n255\n";
	for (int i = 0; i < s_BackBufferSize;) {
		float r = s_BackBuffer[i];
		float g = s_BackBuffer[i + 1];
		float b = s_BackBuffer[i + 2];
		int ir = int(255.99*r);
		int ig = int(255.99*g);
		int ib = int(255.99*b);
		outImage << ir << " " << ig << " " << ib << "\n";
		i += 3;
	}
	outImage.close();
}

int main()
{
	int nx = 1280;
	int ny = 720;
	int ns = 4;
	
	s_BackBufferSize = nx * ny * 3;
	s_BackBuffer = new float[s_BackBufferSize];

	// profile
	LARGE_INTEGER time1;
	QueryPerformanceCounter(&time1);

	float t = float(clock()) / CLOCKS_PER_SEC;
	int inOutRayCount = 0;
	drawImage(s_BackBuffer, nx, ny, ns, inOutRayCount);

	LARGE_INTEGER time2;
	QueryPerformanceCounter(&time2);
	uint64_t dt = time2.QuadPart - time1.QuadPart;
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	time_t seconds;
	time(&seconds);
	std::string timestamp = std::to_string(seconds);

	static char s_Buffer[200];
	double s = double(dt) / double(frequency.QuadPart);
	sprintf_s(s_Buffer, sizeof(s_Buffer), "Perf: %.2fms (%.1f FPS) %.1fMrays/s %.2fMrays/frame\n", s * 1000.0f, 1.f / s, inOutRayCount / s * 1.0e-6f, inOutRayCount * 1.0e-6f);

	std::string profileFilename = std::string("./output/" + timestamp + "_profile.txt");
	std::ofstream outProfile(profileFilename.c_str());
	outProfile << "Resolution: " << nx << "x" << ny << "\n";
	outProfile << "Samples: " << ns << "\n";
	outProfile << "MaxBounces: 50\n";
	outProfile << "TotalRays: " << inOutRayCount << "\n";
	outProfile << s_Buffer;
	outProfile.close();

	WriteImageToDisk(timestamp, nx, ny);

    return 0;
}