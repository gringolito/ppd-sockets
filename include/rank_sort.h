// rank_sort.h
//
// Copyright (c) 2015 Filipe Utzig. All rights reserved.
//
// Initial version by Filipe Utzig <filipeutzig@gmail.com> on 3/19/15.
//
// The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT",
// "SHOULD", "SHOULD NOT", "RECOMMENDED",  "MAY", and "OPTIONAL" in
// this document are to be interpreted as described in RFC 2119.
//
// Rank sort related inline functions to be expanded in code.
//

#ifndef __FUTZIG_RANK_SORT_H
#define __FUTZIG_RANK_SORT_H

#include <stdlib.h>
#include <string.h>
#include <debug.h>

static inline void
rank_sort (int *fuzzy, int *sort, int size)
{
	int i;
	int j;
	int x;

	for (i = 0; i < size; i++) {
		x = 0;
		for (j = 0; j < size; j++) {
			if (fuzzy[i] > fuzzy[j]) {
				x++;
			}
		}
		sort[x] = fuzzy[i];
	}
}

static inline void
merge_vector (int *vector, const int begin, const int mid, const int end)
{
	int ib = begin;
	int im = mid;
	size_t j;
	size_t size = end - begin;
	int tmp[size];

	for (j = 0; j < size; j++) {
		if (ib < mid && (im >= end || vector[ib] <= vector[im])) {
			tmp[j] = vector[ib];
			ib++;
		} else {
			tmp[j] = vector[im];
			im++;
		}
	}

	memcpy(&vector[begin], tmp, sizeof(tmp));
}

#endif //__FUTZIG_RANK_SORT_H

