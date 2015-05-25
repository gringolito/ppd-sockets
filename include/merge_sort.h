// merge_sort.h
//
// "THE BEER-WARE LICENSE" (Revision 42):
// <filipeutzig@gmail.com> wrote this file. As long as you retain this
// notice you can do whatever you want with this stuff. If we meet some
// day, and you think this stuff is worth it, you can buy me a beer in
// return.
//
// Initial version by Filipe Utzig <filipeutzig@gmail.com> on 4/9/15.
//
// The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT",
// "SHOULD", "SHOULD NOT", "RECOMMENDED", "MAY", and "OPTIONAL" in
// this document are to be interpreted as described in RFC 2119.
//
// Merge Sort header functions
//

#ifndef __FUTZIG_MERGE_SORT_H
#define __FUTZIG_MERGE_SORT_H

#include <stdint.h>

/**
 * @brief Sort an entire vector using merge sort algorithm.
 *
 * @param buf vector to be sorted
 * @param begin position of first element
 * @param end position of last element
 */
void
merge_sort (int *buf, uint32_t begin, uint32_t end);

#endif //__FUTZIG_MERGE_SORT_H
