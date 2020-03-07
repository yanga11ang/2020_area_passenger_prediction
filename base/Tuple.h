#ifndef TRIPLE_H
#define TRIPLE_H
#include "Setting.h"

struct Tuple {

	INT id, date_hour;
	REAL index;
	static bool cmp(const Tuple &a, const Tuple &b) {
		return (a.id < b.id)||(a.id == b.id && a.date_hour < b.date_hour)||(a.id == b.id && a.date_hour == b.date_hour && a.index < b.index);
	}
};

#endif
