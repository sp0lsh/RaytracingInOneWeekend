// PathTracer00.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include <fstream>
#include <string>

#include "vec3.h"
#include "ray.h"

int main()
{
	std::ofstream out("output.PPM");

	int nx = 200;
	int ny = 100;
	out << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(float(i) / float(nx), float(j) / float(ny), 0.2f);
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			out << ir << " " << ig << " " << ib << "\n";
		}
	}

	out.close();

    return 0;
}

