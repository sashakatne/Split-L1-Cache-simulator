#include "header.h"

void print_stats() {

	//Calculate the hit ratio for the data and the instrucion caches
	float data_hit_r = float(statistics.data_hit) / (float(statistics.data_miss) + float(statistics.data_hit));
	float inst_hit_r = float(statistics.inst_hit) / (float(statistics.inst_miss) + float(statistics.inst_hit));

	statistics.data_hit_ratio = data_hit_r;
	statistics.inst_hit_ratio = inst_hit_r;

	cout << "\n \t ** KEY CACHE USAGE STATISTICS ** " << endl;
	if (statistics.data_miss == 0) {				// If we don't have any misses, then no operations took place!
		cout << "no operations took place on the Data cache" << endl;
	}
	else {
		cout << "\n\t -- DATA CACHE STATISTICS -- " << endl;

		cout << " number of Cache Reads: \t" << dec << statistics.data_read << endl;
		cout << " number of Cache Writes: \t" << dec << statistics.data_write << endl;
		cout << " number of Cache Hits: \t \t" << dec << statistics.data_hit << endl;
		cout << " number of Cache Misses: \t" << dec << statistics.data_miss << endl;
		cout << " Cache Hit Ratio: \t\t" << dec << statistics.data_hit_ratio << endl;
		cout << " Cache Hit Percentage: \t" << dec << (statistics.data_hit_ratio * 100) << " %" << endl;
	}

	if (statistics.inst_miss == 0) {				// If we don't have any misses, then no operations took place!
		cout << "\n The cache instruction was not used/not operated on" << endl;
	}
	else {
		cout << "\n\t -- INSTRUCTION CACHE STATISTICS -- " << endl;

		cout << " number of Cache Reads: \t" << dec << statistics.inst_read << endl;
		cout << " number of Cache Hits: \t\t" << dec << statistics.inst_hit << endl;
		cout << " number of Cache Misses: \t" << dec << statistics.inst_miss << endl;
		cout << " Cache Hit Ratio: \t\t" << dec << statistics.inst_hit_ratio << endl;
		cout << " Cache Hit Percentage: \t" << dec << (statistics.inst_hit_ratio) * 100 << " %" << endl;
	}
	return;
}
