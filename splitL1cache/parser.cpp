#include "header.h"

using namespace std;

void parser(const char* filename) {
	char trace_line[1024];
	char trace_operation[2] = {0};
	unsigned int operation;
	unsigned int address;
	FILE* fp;

	fp = fopen(filename, "r");
	if (fp == nullptr) {
		cerr << "\nFailed to open the trace file: " << filename << endl;
		exit(1);
	}

	while (fgets(trace_line, sizeof(trace_line), fp)) {
		if (trace_line[0] == '\n' || trace_line[0] == '\0' || trace_line[0] == '\r') {
			continue;								// skip blank lines
		}

		int parsed = sscanf(trace_line, " %1[0-9] %x", trace_operation, &address);
		if (parsed < 1) {
			continue;								// malformed line: skip
		}
		operation = atoi(trace_operation);

		switch (operation) {
		case 0:	read(address);				break;
		case 1:	write(address);				break;
		case 2:	fetch_inst(address);		break;
		case 3:	invalidate(address);		break;
		case 4:	snoop_invalidate(address);	break;
		case 8:	clear_cache();				break;
		case 9:	print_stats();				break;
		default:
			cout << "\n the value of n (the operation) is not valid \n" << endl;
			break;
		}
	}

	fclose(fp);
}
