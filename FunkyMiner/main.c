#include "crew.h"
#include "threading.h"
#include "stdafx.h"

void print_help(const char *arg0) {
	printf("Usage: %s <instance code> <threads> [output file]\n", arg0);
}

int main(int argc, char *argv[])
{
	if (argc < 3) {
		print_help(argv[0]);
		return 0;
	}

	if (strlen(argv[1]) != INSTANCE_SIZE) {
		printf("Bad instance code length (must be %d)\n", INSTANCE_SIZE);
		return 1;
	}

	int i; for (i = 0; i < INSTANCE_SIZE; i++) {
		if(!isalpha(argv[1][i]) && !isdigit(argv[1][i])) {
			printf("Invalid characters in the instance code\n");
			return 1;
		}
	}

	char *out_file = argc == 3 ? "out.txt" : argv[3];
	int t = (int)time(NULL) ^ (int)clock();
	RAND_seed(&t, sizeof(int));
	int threads = atoi(argv[2]);
	if (!threads) {
		printf("Threads count cannot be zero\n");
		return -1;
	}

	while (1) {
		MinersCrew crew;
		create_crew(&crew, threads, argv[1]);

		while (crew.result == NULL) {
			go_sleep(1000);
			double hashes = 0;
			unsigned register int i;
			for (i = 0; i < crew.count; ++i) {
				hashes += crew.miners[i]->counter;
				crew.miners[i]->counter = 0;
			}

			if (hashes < 10000) {
				printf("> %d Hash/s\n", (int)hashes);
			}
			else {
				printf("> %.2f MHash/s\n", hashes / 1000000.0f);
			}
		}

		printf("~ Accepted!\n");
		FILE *f = fopen(out_file, "a");
		if (f == NULL) {
			printf("ERROR: Can't open nor create destination file, interrupting.\n");
			printf("The last mined code was %s.\n", crew.result);
			destroy_crew(&crew);
			return 1;
		}

		fprintf(f, "%s\n", crew.result);
		fclose(f);
		destroy_crew(&crew);
	}

	return 0;
}
