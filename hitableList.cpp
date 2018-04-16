#include "stdafx.h"
#include "hitableList.h"

bool hitableList::hit(const ray & r, float tMin, float tMax, hitRecord & hit) const
{
	hitRecord tempHit;
	bool isHit = false;
	double clossestHitable = tMax;
	for (int i = 0; i < listSize; i++) {
		if (list[i]->hit(r, tMin, clossestHitable, tempHit)) {
			isHit = true;
			clossestHitable = tempHit.t;
			hit = tempHit;
		}
	}
	return isHit;
}
