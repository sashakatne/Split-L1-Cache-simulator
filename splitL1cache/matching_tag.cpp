#include "header.h"

int matching_tag(unsigned int tag, unsigned int set, char which_cache) {
	int i = 0;
	switch (which_cache) {
	case ('d'):
	case ('D'):							// if flag is 'D' We're searching for a matching tag in the L1 data cache
		while (L1_data[i][set].tag_bits != tag) {		// search the data cache for matching tags
			i++;
			if (i > 7) {				// We have 8 ways in the data cache 0 through 7
				return -1;				// return -1 to imply that we don't have a matching tag in the data cache. 
			}
		}
		return i;
		break;

	case ('i'):
	case ('I'):						// if flag is 'I' We're searching for a matching tag in the L1 instruction cache
		int i = 0;
		while (L1_inst[i][set].tag_bits != tag) {			// search the instruction cache for matching tags
			i++;
			if (i > 3) {			// We have 4 ways in the instruction cache 0 through 3
				return -1;			// return -1 to imply that we don't have a matching tag in the instruction cache.
			}
		}
	}
	return i;						// If we have a matching tag, return the way in which we have the matching tag. f
}
