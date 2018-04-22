#pragma once

#include "hitable.h"

class hitableList : public hitable {
	public:
		hitableList() {}
		hitableList(hitable **l, int n) { list = l; listSize = n; }
		virtual bool hit(const ray & r, float tMin, float tMax, hitRecord & hit) const override;

		hitable** list;
		int listSize;
};

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