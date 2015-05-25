// merge_sort.c
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
// Merge Sort library functions.
//

#include <merge_sort.h>
#include <rank_sort.h>         // for merge_vector()
#include <debug.h>

void
merge_sort (int *buf, uint32_t begin, uint32_t end)
{
	int mid;

	if (begin == end || begin == end - 1) {
		return;
	}

	mid = (begin + end) / 2;
	merge_sort(buf, begin, mid);
	merge_sort(buf, mid, end);
	merge_vector(buf, begin, mid, end);
}

