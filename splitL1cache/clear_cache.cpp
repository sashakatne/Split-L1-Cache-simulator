#include "header.h"

void clear_cache() {

	// Reset the data cache
	for (int i = 0; i < DC_ASSOCIAVITY; ++i) {
		for (int j = 0; j < DC_SETS; ++j) {
			L1_data[i][j].MESI_char = 'I';			// Reset the MESI protocol to the Invalid state
			L1_data[i][j].LRU_bits = 0;				// reset the LRU bits to 0
			L1_data[i][j].tag_bits = EMPTY_TAG;		// sentinel for an empty tag (tag is 12 bits, valid range 0-4095)
			L1_data[i][j].set_bits = 0;				// reset the set bits to 0
			L1_data[i][j].address = 0;				// reset the address to 0
		}
	}

	// Reset the instruction cache
	for (int n = 0; n < IC_ASSOCIAVITY; ++n) {
		for (int m = 0; m < IC_SETS; ++m) {
			L1_inst[n][m].MESI_char = 'I';
			L1_inst[n][m].LRU_bits = 0;
			L1_inst[n][m].tag_bits = EMPTY_TAG;
			L1_inst[n][m].set_bits = 0;
			L1_inst[n][m].address = 0;
		}
	}

	// Reset instruction cache statistics
	statistics.inst_read = 0;
	statistics.inst_hit = 0;
	statistics.inst_miss = 0;
	statistics.inst_hit_ratio = 0.0;

	// Reset data cache statistics
	statistics.data_read = 0;
	statistics.data_write = 0;
	statistics.data_hit = 0;
	statistics.data_miss = 0;
	statistics.data_hit_ratio = 0.0;
}
