#include "header.h"

int matching_tag(unsigned int tag, unsigned int set, char which_cache) {
	switch (which_cache) {
	case 'D':							// search the L1 data cache for matching tag
		for (int i = 0; i < DC_ASSOCIAVITY; ++i) {
			if (L1_data[i][set].tag_bits == tag) {
				return i;
			}
		}
		return -1;

	case 'I':							// search the L1 instruction cache for matching tag
		for (int i = 0; i < IC_ASSOCIAVITY; ++i) {
			if (L1_inst[i][set].tag_bits == tag) {
				return i;
			}
		}
		return -1;
	}
	return -1;							// unknown which_cache argument
}
