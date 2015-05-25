// insertion_sort.h
//
// "THE BEER-WARE LICENSE" (Revision 42):
// <filipeutzig@gmail.com> wrote this file. As long as you retain this
// notice you can do whatever you want with this stuff. If we meet some
// day, and you think this stuff is worth it, you can buy me a beer in
// return.
//
// Initial version by Filipe Utzig <filipeutzig@gmail.com> on 3/24/15.
//
// The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT",
// "SHOULD", "SHOULD NOT", "RECOMMENDED", "MAY", and "OPTIONAL" in
// this document are to be interpreted as described in RFC 2119.
//
// Insertion Sort related inline functions to be expanded in code.
//

#ifndef __FUTZIG_INSERTION_SORT_H
#define __FUTZIG_INSERTION_SORT_H

#include <stdlib.h>
#include <debug.h>

/**
 * @brief Insert an element in a vector using insertion sort algorithm, and
 *        return the last element of vector.
 *
 * @param buf vector buffer
 * @param size number of elements of vector
 * @param val[in] element to be inserted
 * @param val[out] last element of vector
 */
static inline void
insertion_sort (int *buf, size_t size, int *val)
{
	size_t i;
	int tmp;

	for (i = 0; i < size; i++) {
		if (*val < buf[i]) {
			tmp = buf[i];
			buf[i] = *val;
			*val = tmp;
		}
	}
}

/**
 * @brief Sort an entire vector using insertion sort algorithm.
 *
 * @param messy vector to be sorted
 * @param sorted sorted vector
 * @param size number of elements of vectors
 */
static inline void
insertion_sortv (const int *messy, int *sorted, size_t size)
{
	size_t i;
	int val;

	for (i = 0; i < size; i++) {
		val = messy[i];
		insertion_sort(sorted, i, &val);
		sorted[i] = val;
	}
}

/**
 * @brief Insert an element in a vector (sorted)
 *
 * @param buf vector buffer
 * @param size number of elementes of buf
 * @param val element to be inserted
 */
static inline void
insert_sorted (int *buf, size_t size, int val)

{
	insertion_sort(buf, size, &val);
	buf[size-1] = val;
}

#endif //__FUTZIG_INSERTION_SORT_H

