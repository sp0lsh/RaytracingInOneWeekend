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
