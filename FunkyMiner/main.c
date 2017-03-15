#include "crew.h"
#include "threading.h"
#include "stdafx.h"

void print_help(const char *arg0) {
	printf("Usage: %s <instance code> <threads> <output file>\n", arg0);
}

int main(int argc, char *argv[])
{
	if (argc < 4) {
		print_help(argv[0]);
		return 0;
	}

	if (strlen(argv[1]) != INSTANCE_SIZE) {
		printf("Bad instance code length (must be %d)\n", INSTANCE_SIZE);
		return 1;
	}

	srand((unsigned int)time(0));
	int threads = atoi(argv[2]);
	if (!threads) {
		printf("Threads count cannot be zero\n");
		return -1;
	}

	while (1) {
		MinersCrew crew;
		create_crew(&crew, threads, argv[1]);

		while (!crew.powerdown) {
			sleep(1000);
			double hashes = 0;
			unsigned register int i;
			for (i = 0; i < crew.count; ++i) {
				hashes += crew.miners[i]->counter;
			}

			if (hashes < 10000) {
				printf("> %d Hash/s\n", (int)hashes);
			}
			else {
				printf("> %.2ff MHash/s\n", hashes / 1000000.0f);
			}
		}

		printf("\nAccepted!\n\n");
		char *code = NULL;
		unsigned register int i;
		for (i = 0; i < crew.count; ++i) {
			if (crew.miners[i]->success) {
				code = crew.miners[i]->data;
				break;
			}
		}

		if (code == NULL) {
			printf("BUG: Mining crew terminated, but no successor was found.\n");
			destroy_crew(&crew);
			return 1;
		}

		FILE *f = fopen(argv[3], "a");
		if (f == NULL) {
			printf("ERROR: Can't open nor create destination file, interrupting.\n");
			printf("The last mined code was %s.\n", code);
			destroy_crew(&crew);
			return 1;
		}

		fprintf(f, "%s\n", code);
		fclose(f);
		destroy_crew(&crew);
	}

	return 0;
}
